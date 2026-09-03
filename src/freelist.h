#pragma once

#include "qol.h"

// Keep track of slot index usage. Acquire and release as necessary.
// Allows for fast reuse of slots.
//
// Usage:
//
// FreeList freelist = freelist_of(1000);
// u32 index = freelist_acquire(&freelist);
// freelist_release(&freelist, index);
// freelist_reset(&freelist);
//
// u32 size    = freelist.size;
// u32 numUsed = freelist.numUsed;
// u32 numFree = freelist.size - freelist.numUsed;
//
// freelist_delete(freelist);

//----------------------------------------------------------------------------------
typedef struct {
    u32 size;
    u32 numUsed;

    u32  _next;
    u32* _data;
} FreeList;

static_assert(sizeof(FreeList) == 24);

//----------------------------------------------------------------------------------
FreeList freelist_of        (u32 size);
void     freelist_delete    (FreeList* f);
void     freelist_reset     (FreeList* f);
u32      freelist_acquire   (FreeList* f);
void     freelist_release   (FreeList* f, u32 index);
//----------------------------------------------------------------------------------

