
// Note: There is no include guard for this file because it is expected to be included per QOL_ARRAY_TYPE.
//       This means you will get compilation errors if you try to include this file twice for the same type.
//       You can avoid this by putting the array includes in a separate header file that has an include guard
//       and then including that into your main c file.

// Note2: Requires QOL_ARRAY_TYPE to be defined before including.
// eg.
//  #define QOL_ARRAY_TYPE u32
//  #include "qol_array.h"
//
//  Will create a u32_array struct and corresponding functions

#include "qol.h"

#ifndef QOL_ARRAY_TYPE
#warning "QOL_ARRAY_TYPE is not defined"
#define QOL_ARRAY_TYPE u32
#endif

#define STRUCT_TYPE TOKENPASTE2(QOL_ARRAY_TYPE, _array)
#define FUNC_NAME(n) TOKENPASTE2(QOL_ARRAY_TYPE, _##n)

//----------------------------------------------------------------------------------

#define CREATE_ARRAY_STRUCT typedef struct {    \
    QOL_ARRAY_TYPE* ptr;                        \
    u32 length;                                 \
    u32 capacity;                               \
} STRUCT_TYPE;

CREATE_ARRAY_STRUCT

//----------------------------------------------------------------------------------
STRUCT_TYPE FUNC_NAME(array_of)(u32 capacity);                      // eg. u32_array u32_array_of(100)

void        FUNC_NAME(array_free)(STRUCT_TYPE* a);                  // eg. void      u32_array_free(&a)
void FUNC_NAME(array_set_length)(STRUCT_TYPE* a, u32 length);       // eg. void      u32_array_set_length(&a, 100)
void FUNC_NAME(array_append)(STRUCT_TYPE* a, QOL_ARRAY_TYPE value); // eg. void      u32_array_append(&a, 100)
void FUNC_NAME(array_pack)(STRUCT_TYPE* a);                         // eg. void      u32_array_pack(&a)

//----------------------------------------------------------------------------------

static void FUNC_NAME(array_ensure_capacity)(STRUCT_TYPE* a, u32 length);

STRUCT_TYPE FUNC_NAME(array_of_u32)(u32 capacity) {
    STRUCT_TYPE a = {
        .ptr = capacity == 0 ? nullptr : calloc(capacity, sizeof(QOL_ARRAY_TYPE)),
        .length = 0,
        .capacity = capacity
    };
    return a;
}

STRUCT_TYPE FUNC_NAME(array_of)(u32 capacity) {
    return FUNC_NAME(array_of_u32)(capacity);
}

void FUNC_NAME(array_free)(STRUCT_TYPE* a) {
    free(a->ptr);
    a->ptr = NULL;
    a->length = 0;
    a->capacity = 0;
}
void FUNC_NAME(array_pack)(STRUCT_TYPE* a) {
    if(a->capacity > a->length) {
        a->capacity = a->length;

        if(a->length == 0) {
            free(a->ptr);
            a->ptr = nullptr;
        } else {
            a->ptr = realloc(a->ptr, a->capacity * sizeof(QOL_ARRAY_TYPE));
        }
    }
}
void FUNC_NAME(array_set_length)(STRUCT_TYPE* a, u32 length) {
    FUNC_NAME(array_ensure_capacity)(a, length);
    a->length = length;
}

void FUNC_NAME(array_append)(STRUCT_TYPE* a, QOL_ARRAY_TYPE value) {
    FUNC_NAME(array_ensure_capacity)(a, a->length + 1);
    a->ptr[a->length++] = value;
}

//----------------------------------------------------------------------------------

static void FUNC_NAME(array_ensure_capacity)(STRUCT_TYPE* a, u32 length) {
    if(length > a->capacity) {
        u32 oldCapacity = a->capacity;
        if(a->capacity < 4) a->capacity = 4;
        while(a->capacity < length) a->capacity *= 2;
        a->ptr = realloc(a->ptr, a->capacity * sizeof(QOL_ARRAY_TYPE));

        // Zero the new elements
        memset(a->ptr + oldCapacity, 0, (a->capacity - oldCapacity) * sizeof(QOL_ARRAY_TYPE));
    }
}
