
#include "qol.h"

#define QOL_ARRAY_TYPE u16
#include "array.h"

void testArray();

static void test_array_of() {
    // array_of_u32
    u16_array a = u16_array_of(0);
    assert(a.length == 0);
    assert(a.capacity == 0);
    assert(a.ptr == nullptr);

    u16_array b = u16_array_of(5);
    assert(b.length == 0);
    assert(b.capacity == 5);
    assert(b.ptr);
    assert(b.ptr[0] == 0);
    assert(b.ptr[1] == 0);
    assert(b.ptr[2] == 0);
    assert(b.ptr[3] == 0);
    assert(b.ptr[4] == 0);

    // array_of_array

    // array_of_ptr
}
static void test_array_free() {
    u16_array a = u16_array_of(5);
    u16_array_free(&a);
    assert(a.length == 0);
    assert(a.capacity == 0);
    assert(a.ptr == nullptr);

    u16_array b = u16_array_of(0);
    u16_array_free(&b);
    assert(b.length == 0);
    assert(b.capacity == 0);
    assert(b.ptr == nullptr);
}
static void test_array_pack() {
    u16_array a = u16_array_of(0);
    u16_array_append(&a, 1);
    u16_array_append(&a, 2);
    assert(a.length == 2);
    assert(a.capacity == 4);
    assert(a.ptr[0] == 1);
    assert(a.ptr[1] == 2);

    u16_array_pack(&a);
    assert(a.length == 2);
    assert(a.capacity == 2);
    assert(a.ptr[0] == 1);
    assert(a.ptr[1] == 2);

    u16_array b = u16_array_of(0);
    u16_array_append(&b, 1);
    assert(b.length == 1);
    assert(b.capacity == 4);
    assert(b.ptr[0] == 1);
    assert(b.ptr[1] == 0);
    assert(b.ptr[2] == 0);
    assert(b.ptr[3] == 0);

    u16_array_set_length(&b, 0);
    assert(b.length == 0);
    assert(b.capacity == 4);

    // packing an empty array will deallocate the ptr
    u16_array_pack(&b);
    assert(b.length == 0);
    assert(b.capacity == 0);
    assert(b.ptr == nullptr);
}
static void test_array_set_length() {
    u16_array a = u16_array_of(0);
    assert(a.length == 0);
    assert(a.capacity == 0);
    assert(a.ptr == nullptr);

    u16_array_set_length(&a, 5);
    assert(a.length == 5);
    assert(a.capacity == 8);    // 4*2 = 8
    assert(a.ptr);
    assert(a.ptr[0] == 0);
    assert(a.ptr[1] == 0);
    assert(a.ptr[2] == 0);
    assert(a.ptr[3] == 0);
    assert(a.ptr[4] == 0);

    u16_array_append(&a, 10);
    assert(a.length == 6);
    assert(a.capacity == 8);
    assert(a.ptr[0] == 0);
    assert(a.ptr[1] == 0);
    assert(a.ptr[2] == 0);
    assert(a.ptr[3] == 0);
    assert(a.ptr[4] == 0);
    assert(a.ptr[5] == 10);

    // increase length
    u16_array_set_length(&a, 10);
    assert(a.length == 10);
    assert(a.capacity == 16);   // 8*2 = 16
    assert(a.ptr[0] == 0);
    assert(a.ptr[1] == 0);
    assert(a.ptr[2] == 0);
    assert(a.ptr[3] == 0);
    assert(a.ptr[4] == 0);
    assert(a.ptr[5] == 10);
    assert(a.ptr[6] == 0);
    assert(a.ptr[7] == 0);
    assert(a.ptr[8] == 0);
    assert(a.ptr[9] == 0);

    // reduce length
    u16_array_set_length(&a, 5);
    assert(a.length == 5);
    assert(a.capacity == 16);
    assert(a.ptr[0] == 0);
    assert(a.ptr[1] == 0);
    assert(a.ptr[2] == 0);
    assert(a.ptr[3] == 0);
    assert(a.ptr[4] == 0);

    // reduce length to 0
    u16_array_set_length(&a, 0);
    assert(a.length == 0);
    assert(a.capacity == 16);
    assert(a.ptr);
}
static void test_array_append() {
    {   // array_append_u16
        u16_array a = u16_array_of(5);
        assert(a.length == 0);
        assert(a.capacity == 5);
        assert(a.ptr);

        // [|0,0,0,0,0]

        // array_append
        u16_array_append(&a, 1);
        u16_array_append(&a, 2);
        u16_array_append(&a, 3);
        u16_array_append(&a, 4);
        u16_array_append(&a, 5);

        assert(a.length == 5);
        assert(a.capacity == 5);
        assert(a.ptr[0] == 1);
        assert(a.ptr[1] == 2);
        assert(a.ptr[2] == 3);
        assert(a.ptr[3] == 4);
        assert(a.ptr[4] == 5);

        // [1,2,3,4,5|]

        // This one should trigger the array to grow to capacity * 2 = 10
        u16_array_append(&a, 6);
        assert(a.length == 6);
        assert(a.capacity == 10);
        assert(a.ptr[0] == 1);
        assert(a.ptr[1] == 2);
        assert(a.ptr[2] == 3);
        assert(a.ptr[3] == 4);
        assert(a.ptr[4] == 5);
        assert(a.ptr[5] == 6);
        assert(a.ptr[6] == 0);
        assert(a.ptr[7] == 0);
        assert(a.ptr[8] == 0);
        assert(a.ptr[9] == 0);
    }
}

void testArray() {
    printf("Testing Array...\n");

    test_array_of();
    test_array_free();
    test_array_pack();
    test_array_set_length();
    test_array_append();
}
