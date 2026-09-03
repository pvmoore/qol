#pragma once

#include "qol.h"

typedef struct {
    u8* buffer;
    u32 pos;
    u32 capacity;
} Arena;

static_assert(sizeof(Arena) == 16);

/**
 *  Creates an Arena with the specified capacity
 *  Note: This will eagerly allocate capacity bytes
 */
Arena arena_of(u32 capacity);

/** Frees the memory allocated by an Arena */
void arena_delete(Arena* arena);

/** Allocates aligned memory from the arena and returns a pointer to it */
void* arena_alloc(Arena* arena, u32 size, u32 alignment);

/** Resets the arena. Zeroes out the memory */
void arena_reset(Arena* arena);

/**
 * Returns a const char*  representation of the arena object for debugging
 * Note: The returned string will be overwritten by subsequent calls.
 */
const char* arena_getDebugCstr(Arena* arena);
