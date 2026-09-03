#pragma once

#include "qol.h"

//---------------------------------------------------------------------------------- public types
typedef struct {
    u8* const ptr;
    const u32 length;
    const u32 capacity;
} u8_array;

//---------------------------------------------------------------------------------- public functions

// u8_array u8_array_of(u32 capacity);
// u8_array u8_array_of(u8_array);
// u8_array u8_array_of(u8*, u32);
u8_array u8_array_of_u32        (u32 capacity);
u8_array u8_array_of_u8_array   (u8_array array);
u8_array u8_array_of_u8_ptr     (u8* ptr, u32 length);

void     u8_array_free      (u8_array* a);
void     u8_array_set_length(u8_array* a, u32 length);
void     u8_array_pack      (u8_array* a);

// void u8_array_append(u8_array*, u8);
// void u8_array_append(u8_array*, u8_array);
// void u8_array_append(u8_array*, u8*, u32);
void     u8_array_append_u8         (u8_array* a, u8 value);
void     u8_array_append_u8_array   (u8_array* a, u8_array array);
void     u8_array_append_u8_ptr     (u8_array* a, u8* value, u32 length);

//---------------------------------------------------------------------------------- private struct u8_array_mutable
typedef struct {
    u8* ptr;
    u32 length;
    u32 capacity;
} u8_array_mutable;
static_assert(sizeof(u8_array_mutable) == sizeof(u8_array));
//---------------------------------------------------------------------------------- private u8_array_of
static void u8_array_ensure_capacity(u8_array_mutable* a, u32 length);

u8_array u8_array_of_u32(u32 capacity) {
    u8_array a = {
        .ptr = capacity == 0 ? nullptr : calloc(capacity, 1),
        .length = 0,
        .capacity = capacity
    };
    return a;
}
u8_array u8_array_of_u8_array(u8_array array) {
    u8_array a = u8_array_of_u32(array.length);
    u8_array_append_u8_array(&a, array);
    return a;
}
u8_array u8_array_of_u8_ptr(u8* ptr, u32 length) {
    u8_array a = u8_array_of_u32(length);
    u8_array_append_u8_ptr(&a, ptr, length);
    return a;
}

#define u8_array_of1(a) _Generic((a), int:        u8_array_of_u32 \
                                    ,  u8_array:   u8_array_of_u8_array \
                              )(a)

#define u8_array_of2(a, b) u8_array_of_u8_ptr(a, b)

#define u8_array_ofN(_1,_0,N,...) u8_array_of##N

#define u8_array_of(...) u8_array_ofN(__VA_ARGS__,2,1)(__VA_ARGS__)

//---------------------------------------------------------------------------------- private u8_array_free
void u8_array_free(u8_array* a) {
    u8_array_mutable* m = (u8_array_mutable*)a;
    free(m->ptr);
    m->ptr = nullptr;
    m->length = 0;
    m->capacity = 0;
}
//---------------------------------------------------------------------------------- private u8_array_set_length
void u8_array_set_length(u8_array* a, u32 length) {
    u8_array_mutable* m = (u8_array_mutable*)a;
    u8_array_ensure_capacity(m, length);
    m->length = length;
}
//---------------------------------------------------------------------------------- private u8_array_pack
void u8_array_pack(u8_array* a) {
    if(a->capacity > a->length) {
        u8_array_mutable* m = (u8_array_mutable*)a;
        m->capacity = a->length;
        if(a->length == 0) {
            free(m->ptr);
            m->ptr = nullptr;
        } else {
            m->ptr = realloc(a->ptr, a->capacity);
        }
    }
}
//---------------------------------------------------------------------------------- private u8_array_append
void u8_array_append_u8(u8_array* a, u8 value) {
    u8_array_mutable* m = (u8_array_mutable*)a;
    u8_array_ensure_capacity(m, m->length + 1);
    m->ptr[m->length++] = value;
}
void u8_array_append_u8_array(u8_array* a, u8_array array) {
    if(array.length == 0) return;
    u8_array_mutable* m = (u8_array_mutable*)a;
    u8_array_ensure_capacity(m, m->length + array.length);
    memcpy(m->ptr + m->length, array.ptr, array.length);
    m->length += array.length;
}
void u8_array_append_u8_ptr(u8_array* a, u8* values, u32 length) {
    assert(a);
    assert(values || length == 0);
    if(length == 0) return;

    u8_array_mutable* m = (u8_array_mutable*)a;
    u8_array_ensure_capacity(m, m->length + length);
    memcpy(m->ptr + m->length, values, length);
    m->length += length;
}

#define u8_array_append2(a,b) _Generic((b), int:        u8_array_append_u8 \
                                          , u8_array:   u8_array_append_u8_array \
                              )(a, b)

#define u8_array_append3(a,b,c) _Generic((b), u8*:        u8_array_append_u8_ptr \
                                )(a, b, c)

#define u8_array_appendN(_2,_1,_0,N,...) u8_array_append##N

#define u8_array_append(...) u8_array_appendN(__VA_ARGS__,3,2)(__VA_ARGS__)

//---------------------------------------------------------------------------------- private u8_array_ensure_capacity
static void u8_array_ensure_capacity(u8_array_mutable* a, u32 length) {
    if(length > a->capacity) {
        u32 oldCapacity = a->capacity;
        if(a->capacity < 4) a->capacity = 4;
        while(a->capacity < length) a->capacity *= 2;
        a->ptr = realloc(a->ptr, a->capacity);

        // Zero the new elements
        memset(a->ptr + oldCapacity, 0, a->capacity - oldCapacity);
    }
}
