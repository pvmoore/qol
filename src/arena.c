#include "arena.h"

/**
Creates an Arena with the specified capacity
Note: This will eagerly allocate capacity bytes
 */
Arena arena_of(u32 capacity) {
    assert(capacity > 0 && "capacity must be > 0");

    return (Arena){ .buffer = malloc(capacity), .pos = 0, .capacity = capacity };
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

/** Allocates aligned memory from the arena and returns a pointer to it */
void* arena_alloc_aligned(Arena* arena, u32 size, u32 alignment) {
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
/** Allocates memory from an Arena using alignment of 1 and returns a pointer to it */
void* arena_alloc(Arena* arena, u32 size) {
    assert(size > 0 && "size == 0");

    return arena_alloc_aligned(arena, size, 1);
}
/** Frees the memory allocated by an Arena */
void arena_delete(Arena* arena) {
    assert(arena);

    free(arena->buffer);
    arena->buffer = NULL;
    arena->pos = 0;
    arena->capacity = 0;
}
