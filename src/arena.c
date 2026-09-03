#include "arena.h"

Arena arena_of(u32 capacity) {
    assert(capacity > 0 && "capacity must be > 0");

    return (Arena){ .buffer = calloc(1, capacity), .pos = 0, .capacity = capacity };
}

/**
 * Returns a const char*  representation of the arena object for debugging.
 * Note: The returned string will be overwritten by subsequent calls.
 */
const char* arena_getDebugCstr(Arena* arena) {
    static char buffer[1024];
    [[maybe_unused]] u32 count = snprintf(buffer, sizeof(buffer), "Arena{ .pos = %u, .capacity = %u }", arena->pos, arena->capacity);
    return buffer;
}

void* arena_alloc(Arena* arena, u32 size, u32 alignment) {
    assert(arena);
    assert(alignment > 0 && "alignment must be > 0");

    if(alignment > 1) {
        arena->pos += (alignment - 1);
        arena->pos &= ~(alignment - 1);
    }
    u32 start = arena->pos;
    arena->pos += size;
    assert(arena->pos <= arena->capacity && "arena_allocate_aligned: Out of Arena memory");

    return arena->buffer + start;
}

void arena_delete(Arena* arena) {
    assert(arena);

    free(arena->buffer);
    arena->buffer = nullptr;
    arena->pos = 0;
    arena->capacity = 0;
}

void arena_reset(Arena* arena) {
    memset(arena->buffer, 0, arena->pos);
    arena->pos = 0;
}
