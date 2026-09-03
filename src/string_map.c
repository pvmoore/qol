#include "string_map.h"

typedef struct {
    u32 capacity;
    u32 numKeys;                // Current number of populated keys in the map
    u32 mask;                   // capacity - 1
    u32 numKeysThreshold;       // numKeys value which will trigger a rehash
    float loadFactor;

    string* keys;               // (length = capacity)
    u32* flags;                 // Bit flags for key slots. 1 = occupied, 0 = free
    string* values;             // (length = capacity)
} string_map_impl;

u32     getSlot                     (string_map_impl* m, string key);
u32     nextSlot                    (string_map_impl* m, u32 slot);
bool    isOccupied                  (string_map_impl* m, u32 slot);
void    setOccupied                 (string_map_impl* m, u32 slot);
void    setFree                     (string_map_impl* m, u32 slot);
i32     findSlotForKey              (string_map_impl* m, string key);
void    setKeyValue                 (string_map_impl* m, u32 slot, string key, string value);
void    expand                      (string_map_impl* m);
u32     calculateLoadFactorThreshold(u32 capacity, float loadFactor);

string_map_impl* toImpl(string_map* m) {
    return (string_map_impl*)m;
}

/** Allocate memory */
void alloc(string_map_impl* m) {
    if(!m->keys) {
        m->keys = (string*)calloc(m->capacity, sizeof(string));
        m->flags = (u32*)calloc(m->capacity / 32 + 1, sizeof(u32));
        m->values = (string*)calloc(m->capacity, sizeof(string));
    }
}

string_map* string_map_of(Arena* arena, u32 capacity, float loadFactor) {

    assert(popcnt(capacity) == 1 && "capacity must be a power of 2");
    assert(loadFactor > 0 && "loadFactor must be > 0");
    assert(loadFactor < 1 && "loadFactor must be < 1");

    // Zero initialise the map. Allocate memory later if/when a key is added
    string_map_impl* m = arena_alloc(arena, sizeof(string_map_impl), 8);

    *m = (string_map_impl) {
        .capacity = capacity,
        .loadFactor = loadFactor,
        .numKeys = 0,
        .mask = capacity - 1,
        .numKeysThreshold = calculateLoadFactorThreshold(capacity, loadFactor),
        .keys = nullptr,
        .flags = nullptr,
        .values = nullptr
    };

    return (string_map*)m;
}

void string_map_delete(string_map* map) {
    string_map_impl* m = toImpl(map);
    free(m->keys);
    free(m->flags);
    free(m->values);
    m->numKeys = 0;
}

bool string_map_is_empty(string_map* m) {
    return ((string_map_impl*)m)->numKeys == 0;
}

u32 string_map_size(string_map* m) {
    return ((string_map_impl*)m)->numKeys;
}

u32 string_map_capacity(string_map* m) {
    return ((string_map_impl*)m)->capacity;
}

float string_map_load_factor(string_map *m) {
    return ((string_map_impl*)m)->loadFactor;
}

void string_map_dump(string_map* map) {
    string_map_impl* m = toImpl(map);
    printf("string_map {\n");
    for(u32 slot = 0; slot < m->capacity; slot++) {
        if(m->keys && isOccupied(m, slot)) {
            string key = m->keys[slot];
            string value = m->values[slot];

            printf("  [%2u] %s = %s\n", slot, key.buffer, value.length == 0 ? "null" : value.buffer);
        } else {
            printf("  [%2u] -\n", slot);
        }
    }
    printf("   size = %u/%u, load = %.2f, threshold = %u\n",
        m->numKeys,
        m->capacity,
        (float)m->numKeys / m->capacity,
        m->numKeysThreshold);
    printf("}\n");
}

/**
 * Add or replace a Key,Value in the map
 */
void string_map_insert(string_map* map, string key, string value) {
    string_map_impl* m = toImpl(map);
    alloc(m);
    u32 slot = getSlot(m, key);

    // Find a free slot for this key. Or update the value in an existing slot
    while(true) {
        if(!isOccupied(m, slot)) {
            // This slot is free
            setKeyValue(m, slot, key, value);
            return;
        }

        // This slot is occupied

        if(string_equals_string(m->keys[slot], key)) {
            // Update the value in this slot
            m->values[slot] = value;
            return;
        }

        // Continue looping
        slot = nextSlot(m, slot);
    }
}

/**
 * Get a value from the map. Return nullptr if not found
 */
string* string_map_get(string_map* map, string key) {
    string_map_impl* m = toImpl(map);
    i32 slot = findSlotForKey(m, key);
    if(slot != -1) {
        return &m->values[slot];
    }
    return nullptr;
}

bool string_map_contains_key(string_map* map, string key) {
    return findSlotForKey(toImpl(map), key) != -1;
}

/**
 * Remove a key from the map. Returns true if the key was found and removed
 */
bool string_map_remove(string_map* map, string key) {
    string_map_impl* m = toImpl(map);
    i32 foundSlot = findSlotForKey(m, key);
    if(foundSlot == -1) {
        // Key not found
        return false;
    }
    m->numKeys--;
    u32 freeSlot = foundSlot;
    setFree(m, freeSlot);

    // We need to adjust slots below this one until we find an unoccupied slot
    u32 slot = nextSlot(m, freeSlot);
    i32 distanceFromFreeSlot = 1;
    while(true) {
        if(isOccupied(m, slot)) {
            // If the key in this slot hashes to a slot that is equal to the free slot or before the free
            // slot then we need to move it into the free slot. Repeat this process for the newly freed slot
            // until we reach an unoccupied slot
            u32 keySlot = getSlot(m, m->keys[slot]);
            i32 distance = (i32)slot - (i32)keySlot;
            distance = distance < 0 ? (distance + m->capacity) : distance;

            if(distance >= distanceFromFreeSlot) {
                // This key can be moved into the free slot
                setOccupied(m, freeSlot);
                m->keys[freeSlot] = m->keys[slot];
                m->values[freeSlot] = m->values[slot];

                // This slot is now the new free slot
                // m->keys[slot] = nullptr;
                // m->values[slot] = nullptr;
                setFree(m, slot);
                freeSlot = slot;
                distanceFromFreeSlot = 0;
            }

            // Keep looping
            slot = nextSlot(m, slot);
            distanceFromFreeSlot++;
        } else {
            // This slot is unoccupied. We are done
            break;
        }
    }

    return true;
}

void string_map_clear(string_map* map, bool freeMemory) {
    string_map_impl* m = toImpl(map);
    if(freeMemory) {
        free(m->keys);
        free(m->flags);
        free(m->values);
        m->keys = nullptr;
        m->flags = nullptr;
        m->values = nullptr;
    } else {
        memset(m->keys, 0, m->capacity * sizeof(string));
        memset(m->flags, 0, (m->capacity / 32 + 1) * sizeof(u32));
        memset(m->values, 0, m->capacity * sizeof(string));
    }
    m->numKeys = 0;
}

u32 string_map_keys(string_map* map, string* keysOut) {
    string_map_impl* m = toImpl(map);
    assert(m->numKeys == 0 || keysOut);

    u32 count = 0;
    for(u32 i = 0; i < m->capacity; i++) {
        if(isOccupied(m, i)) {
            keysOut[count++] = m->keys[i];
        }
    }
    return count;
}

u32 string_map_values(string_map* map, string* valuesOut) {
    string_map_impl* m = toImpl(map);
    assert(m->numKeys == 0 || valuesOut);

    u32 count = 0;
    for(u32 i = 0; i < m->capacity; i++) {
        if(isOccupied(m, i)) {
            valuesOut[count++] = m->values[i];
        }
    }
    return count;
}

u32 djb2_hash(string s) {
    u32 hash = 5381;
    for(u32 i = 0; i < s.length; i++) {
        char c = s.buffer[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

u32 getSlot(string_map_impl* m, string key) {
    return (djb2_hash(key) & m->mask);
}

/** Get the next slot, wrapping around if necessary */
u32 nextSlot(string_map_impl* m, u32 slot) {
    return (slot+1) & m->mask;
}

bool isOccupied(string_map_impl* m, u32 slot) {
    assert(m->flags);
    u32 u = slot >> 5;
    u32 r = slot & 31;
    return ((m->flags[u] >> r) & 1) == 1;
}
void setOccupied(string_map_impl* m, u32 slot) {
    assert(m->flags);
    u32 u = slot >> 5;
    u32 r = slot & 31;
    m->flags[u] |= (1 << r);
}
void setFree(string_map_impl* m, u32 slot) {
    assert(m->flags);
    u32 u = slot >> 5;
    u32 r = slot & 31;
    m->flags[u] &= ~(1 << r);
}
void setKeyValue(string_map_impl* m, u32 slot, string key, string value) {
    assert(!isOccupied(m, slot));

    m->keys[slot] = key;
    m->values[slot] = value;
    m->numKeys++;
    setOccupied(m, slot);

    if(m->numKeys >= m->numKeysThreshold) {
        expand(m);
    }
}
/**
 * Find the slot for a given key
 * Returns -1 if not found
 */
i32 findSlotForKey(string_map_impl* m, string key) {
    u32 slot = getSlot(m, key);

    // Use equality to check each slot until we find the key or an unoccupied slot
    while(isOccupied(m, slot) && !string_equals_string(m->keys[slot], key)) {
        slot = nextSlot(m, slot);
    }

    return isOccupied(m, slot) ? slot : -1;
}

u32 calculateLoadFactorThreshold(u32 capacity, float loadFactor) {
    u32 threshold = (u32)(capacity * loadFactor);
    if(threshold == 0) {
        threshold = 1;
    }
    if(threshold > capacity) {
        threshold = capacity;
    }
    return threshold;
}

/**
 * Double the capacity of the map.
 */
void expand(string_map_impl* m) {
    string* oldKeys     = m->keys;
    u32* oldFlags       = m->flags;
    string* oldValues   = m->values;
    u32 length          = m->capacity * 2;

    m->capacity         = length;
    m->mask             = length - 1;
    m->keys             = calloc(length, sizeof(string));
    m->flags            = calloc(length / 32 + 1, sizeof(u32));
    m->values           = calloc(length, sizeof(string));
    m->numKeysThreshold = calculateLoadFactorThreshold(length, m->loadFactor);

    u32 u   = 0;
    u32 bit = 1;
    u32 f   = oldFlags[u];

    for(u32 oldSlot = 0; oldSlot < m->capacity; oldSlot++) {
        if(f & bit) {
            // this slot is occupied
            u32 newSlot = getSlot(m, oldKeys[oldSlot]);
            while(isOccupied(m, newSlot)) newSlot = nextSlot(m, newSlot);

            setOccupied(m, newSlot);
            m->keys[newSlot] = oldKeys[oldSlot];
            m->values[newSlot] = oldValues[oldSlot];
        }

        bit <<= 1;
        if(!bit) {
            u++;
            bit = 1;
            f = oldFlags[u];
        }
    }

    free(oldKeys);
    free(oldFlags);
    free(oldValues);
}
