#pragma once

#include "qol.h"

//----------------------------------------------------------------------------------
typedef struct {
    u8* buffer;
    u32 pos;
    u32 capacity;
} Arena;
static_assert(sizeof(Arena) == 16);
//----------------------------------------------------------------------------------
Arena       arena_of(u32 capacity);
const char* arena_getDebugCstr(Arena* arena);
void*       arena_alloc_aligned(Arena* arena, u32 size, u32 alignment);
void*       arena_alloc(Arena* arena, u32 size);
void        arena_delete(Arena* arena);
//----------------------------------------------------------------------------------

