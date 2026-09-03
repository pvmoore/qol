#include "qol.h"
#include "string_map.h"

void empty();
void insert1();
void insert2();
void rehash();
void remove1();
void clear1();
void clear2();
void keys();
void values();

void testStringMap() {
    printf("Testing string_map\n");

    empty();
    insert1();
    insert2();
    rehash();
    remove1();
    clear1();
    clear2();
    keys();
    values();

    printf("Done\n");
}

void empty() {
    printf("  empty\n");

    string_map m0 = {};
    string_map_dump(&m0);
    assert(string_map_is_empty(&m0));
    assert(string_map_size(&m0) == 0);
    assert(m0.capacity == 0);
    assert(m0.loadFactor == 0);

    string_map m = string_map_of(8, 0.75f);
    string_map_dump(&m);
    assert(string_map_is_empty(&m));
    assert(string_map_size(&m) == 0);
    assert(m.capacity == 8);
    assert(m.loadFactor == 0.75f);
}

void insert1() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string A = string_of("A");

    string_map_insert(&m, a, A);
    string_map_dump(&m);

    assert(string_map_size(&m) == 1);
    assert(string_map_contains_key(&m, a));
    assert(string_map_get(&m, a) != nullptr);
    assert(string_equals_string(*string_map_get(&m, a), A));

    string_map_insert(&m, string_of(nullptr), string_of("null"));
    string_map_dump(&m);

    assert(string_map_size(&m) == 2);
    assert(string_map_contains_key(&m, string_of(nullptr)));
    assert(string_map_get(&m, string_of(nullptr)) != nullptr);
    assert(string_equals_string(*string_map_get(&m, string_of(nullptr)), string_of("null")));
}

void insert2() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("aa");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);
    string_map_dump(&m);

    assert(string_map_size(&m) == 4);
    assert(string_map_contains_key(&m, a));
    assert(string_map_contains_key(&m, b));
    assert(string_map_contains_key(&m, c));
    assert(string_map_contains_key(&m, d));
    assert(string_map_get(&m, a) != nullptr);
    assert(string_map_get(&m, b) != nullptr);
    assert(string_map_get(&m, c) != nullptr);
    assert(string_map_get(&m, d) != nullptr);
    assert(string_equals_string(*string_map_get(&m, a), A));
    assert(string_equals_string(*string_map_get(&m, b), B));
    assert(string_equals_string(*string_map_get(&m, c), C));
    assert(string_equals_string(*string_map_get(&m, d), D));
}

void rehash() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string e = string_of("e");
    string f = string_of("f");

    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");
    string E = string_of("E");
    string F = string_of("F");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);
    string_map_insert(&m, e, E);

    assert(string_map_size(&m) == 5);
    assert(m.capacity == 8);
    assert(m.loadFactor == 0.75f);

    // The 6th insert will trigger a rehash
    string_map_insert(&m, f, F);
    string_map_dump(&m);

    assert(string_map_size(&m) == 6);
    assert(m.capacity == 16);
    assert(m.loadFactor == 0.75f);

    assert(string_map_contains_key(&m, a));
    assert(string_map_contains_key(&m, b));
    assert(string_map_contains_key(&m, c));
    assert(string_map_contains_key(&m, d));
    assert(string_map_contains_key(&m, e));
    assert(string_map_contains_key(&m, f));
    assert(string_map_get(&m, a) != nullptr);
    assert(string_map_get(&m, b) != nullptr);
    assert(string_map_get(&m, c) != nullptr);
    assert(string_map_get(&m, d) != nullptr);
    assert(string_map_get(&m, e) != nullptr);
    assert(string_map_get(&m, f) != nullptr);
    assert(string_equals_string(*string_map_get(&m, a), A));
    assert(string_equals_string(*string_map_get(&m, b), B));
    assert(string_equals_string(*string_map_get(&m, c), C));
    assert(string_equals_string(*string_map_get(&m, d), D));
    assert(string_equals_string(*string_map_get(&m, e), E));
    assert(string_equals_string(*string_map_get(&m, f), F));

    assert(!string_map_contains_key(&m, string_of("g")));
}

void remove1() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);

    assert(string_map_size(&m) == 4);

    assert(string_map_remove(&m, b));
    assert(string_map_size(&m) == 3);
    assert(!string_map_contains_key(&m, b));

    assert(!string_map_remove(&m, b));
    assert(string_map_size(&m) == 3);

    assert(string_map_remove(&m, a));
    assert(string_map_size(&m) == 2);
    assert(!string_map_contains_key(&m, a));

    assert(string_map_remove(&m, d));
    assert(string_map_size(&m) == 1);

    assert(string_map_remove(&m, c));
    assert(string_map_size(&m) == 0);

    string_map_dump(&m);
}

void clear1() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);

    string_map_clear(&m, true);
    assert(string_map_size(&m) == 0);
    assert(m.keys == nullptr);
    assert(m.flags == nullptr);
    assert(m.values == nullptr);

    string_map_insert(&m, b, B);
    assert(string_map_size(&m) == 1);
    assert(string_map_contains_key(&m, b));
}

void clear2() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);

    string_map_clear(&m, false);
    assert(string_map_size(&m) == 0);
    assert(m.keys != nullptr);
    assert(m.flags != nullptr);
    assert(m.values != nullptr);

    string_map_insert(&m, b, B);
    assert(string_map_size(&m) == 1);
    assert(string_map_contains_key(&m, b));
}

void keys() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);

    string* keys = calloc(4, sizeof(string));

    u32 numKeys = string_map_keys(&m, keys);
    assert(numKeys == 4);

    printf("Keys:\n");
    for(u32 i = 0; i < numKeys; i++) {
        printf("  %s\n", keys[i].buffer);
    }

    bool found[4] = {};

    for(u32 i = 0; i < numKeys; i++) {
        u32 j = keys[i].buffer[0] - 'a';
        assert(!found[j]);
        found[j] = true;
    }
    assert(found[0]);
    assert(found[1]);
    assert(found[2]);
    assert(found[3]);
}

void values() {
    string_map m = string_map_of(8, 0.75f);

    string a = string_of("a");
    string b = string_of("b");
    string c = string_of("c");
    string d = string_of("d");
    string A = string_of("A");
    string B = string_of("B");
    string C = string_of("C");
    string D = string_of("D");

    string_map_insert(&m, a, A);
    string_map_insert(&m, b, B);
    string_map_insert(&m, c, C);
    string_map_insert(&m, d, D);

    string* values = calloc(4, sizeof(string));

    u32 numValues = string_map_values(&m, values);
    assert(numValues == 4);

    printf("Values:\n");
    for(u32 i = 0; i < numValues; i++) {
        printf("  %s\n", values[i].buffer);
    }

    bool found[4] = {};

    for(u32 i = 0; i < numValues; i++) {
        u32 j = values[i].buffer[0] - 'A';
        assert(!found[j]);
        found[j] = true;
    }
    assert(found[0]);
    assert(found[1]);
    assert(found[2]);
    assert(found[3]);
}
