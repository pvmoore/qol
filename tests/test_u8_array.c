
#include "qol.h"
#include "u8_array.h"

void test_u8_array();

//----------------------------------------------------------------------------------------------------- private

static void test_u8_array_of() {
    {   // u8_array_of_u32
        u8_array a = u8_array_of(0);
        assert(a.ptr == nullptr);
        assert(a.length == 0);
        assert(a.capacity == 0);

        u8_array b = u8_array_of(5);
        assert(b.ptr != nullptr);
        assert(b.length == 0);
        assert(b.capacity == 5);
    }
    {   // u8_array_of_u8_array
        u8_array z1 = u8_array_of(0);
        u8_array z2 = u8_array_of(0);
        u8_array z3 = u8_array_of(0);
        u8_array_append(&z2, 3);
        u8_array_append(&z3, 3);
        u8_array_append(&z3, 4);

        // z1 = []
        // z2 = [3]
        // z3 = [3, 4]

        u8_array a = u8_array_of(z1);
        assert(a.ptr == nullptr);
        assert(a.length == 0);
        assert(a.capacity == 0);

        u8_array b = u8_array_of(z2);
        assert(b.ptr != nullptr);
        assert(b.length == 1);
        assert(b.capacity == 1);
        assert(b.ptr[0] == 3);

        u8_array c = u8_array_of(z3);
        assert(c.ptr != nullptr);
        assert(c.length == 2);
        assert(c.capacity == 2);
        assert(c.ptr[0] == 3);
        assert(c.ptr[1] == 4);
    }
    {   // u8_array_of_u8_ptr
        u8_array a = u8_array_of(nullptr, 0);
        assert(a.ptr == nullptr);
        assert(a.length == 0);
        assert(a.capacity == 0);

        u8 z1[5] = {1, 2, 3, 4, 5};
        u8_array b = u8_array_of(z1, 5);

        assert(b.ptr != nullptr);
        assert(b.length == 5);
        assert(b.capacity == 5);
        assert(b.ptr[0] == 1);
        assert(b.ptr[1] == 2);
        assert(b.ptr[2] == 3);
        assert(b.ptr[3] == 4);
        assert(b.ptr[4] == 5);
    }
}
static void test_u8_array_free() {
    u8_array a = u8_array_of(5);

    u8_array_free(&a);
    assert(a.ptr == nullptr);
    assert(a.length == 0);
    assert(a.capacity == 0);
}
static void test_u8_array_set_length() {
    {   // u8_array_set_length
        u8_array a = u8_array_of(5);

        // Increase length
        u8_array_set_length(&a, 100);
        assert(a.length == 100);
        assert(a.capacity == 160); // 5*2 = 10, 10*2 = 20, 20*2 = 40, 40*2 = 80, 80*2 = 160

        // Decrease length
        u8_array_set_length(&a, 50);
        assert(a.length == 50);
        assert(a.capacity == 160);

        // Decrease again
        u8_array_set_length(&a, 0);
        assert(a.length == 0);
        assert(a.capacity == 160);

        // pack
        u8_array_set_length(&a, 4);
        u8_array_pack(&a);
        assert(a.length == 4);
        assert(a.capacity == 4);
        assert(a.ptr);

        // pack to zero
        u8_array_set_length(&a, 0);
        u8_array_pack(&a);
        assert(a.length == 0);
        assert(a.capacity == 0);
        assert(a.ptr == nullptr);
    }
}
static void test_u8_array_append() {
    {   // u8_array_append_u8
        u8_array a = u8_array_of(0);
        u8_array_append_u8(&a, 1);
        assert(a.length == 1);
        assert(a.capacity == 4);    // 4 is the minimum
        assert(a.ptr[0] == 1);
        assert(a.ptr[1] == 0);
        assert(a.ptr[2] == 0);
        assert(a.ptr[3] == 0);

        u8_array_append(&a, 2);
        assert(a.length == 2);
        assert(a.capacity == 4);
        assert(a.ptr[0] == 1);
        assert(a.ptr[1] == 2);
        assert(a.ptr[2] == 0);
        assert(a.ptr[3] == 0);
    }
    {   // u8_array_append_u8_array
        u8_array a = u8_array_of(0);
        assert(a.length == 0);
        assert(a.capacity == 0);
        assert(a.ptr == nullptr);

        // append an empty u8_array
        u8_array b = u8_array_of(0);
        u8_array_append_u8_array(&a, b);

        assert(a.length == 0);
        assert(a.capacity == 0);
        assert(a.ptr == nullptr);

        // append a u8_array with 1 element
        u8_array c = u8_array_of(0);
        u8_array_append(&c, 7);

        u8_array_append_u8_array(&a, c);
        assert(a.length == 1);
        assert(a.capacity == 4);
        assert(a.ptr[0] == 7);
        assert(a.ptr[1] == 0);
        assert(a.ptr[2] == 0);
        assert(a.ptr[3] == 0);

        // Pack a back to empty
        u8_array_set_length(&a, 0);
        u8_array_pack(&a);
        assert(a.length == 0);
        assert(a.capacity == 0);
        assert(a.ptr == nullptr);

        // append a u8_array with 5 elements
        u8_array d = u8_array_of(0);
        u8_array_append(&d, 1);
        u8_array_append(&d, 2);
        u8_array_append(&d, 3);
        u8_array_append(&d, 4);
        u8_array_append(&d, 5);
        u8_array_append_u8_array(&a, d);
        assert(a.length == 5);
        assert(a.capacity == 8);   // 4*2 = 8
        assert(a.ptr[0] == 1);
        assert(a.ptr[1] == 2);
        assert(a.ptr[2] == 3);
        assert(a.ptr[3] == 4);
        assert(a.ptr[4] == 5);
        assert(a.ptr[5] == 0);
        assert(a.ptr[6] == 0);
        assert(a.ptr[7] == 0);
    }
    {   // u8_array_append_u8_ptr
        u8_array a = u8_array_of(0);

        // append an empty u8_ptr
        u8_array_append_u8_ptr(&a, nullptr, 0);
        assert(a.length == 0);
        assert(a.capacity == 0);
        assert(a.ptr == nullptr);

        // append a u8_ptr with 1 element
        u8 b = 7;
        u8_array_append(&a, &b, 1);
        assert(a.length == 1);
        assert(a.capacity == 4);
        assert(a.ptr[0] == 7);
        assert(a.ptr[1] == 0);
        assert(a.ptr[2] == 0);
        assert(a.ptr[3] == 0);

        // Pack a back to empty
        u8_array_set_length(&a, 0);
        u8_array_pack(&a);
        assert(a.length == 0);
        assert(a.capacity == 0);
        assert(a.ptr == nullptr);

        // append a u8_ptr with 5 elements
        u8 c[5] = {1, 2, 3, 4, 5};
        u8_array_append_u8_ptr(&a, c, 5);
        assert(a.length == 5);
        assert(a.capacity == 8);   // 4*2 = 8
        assert(a.ptr[0] == 1);
        assert(a.ptr[1] == 2);
        assert(a.ptr[2] == 3);
        assert(a.ptr[3] == 4);
        assert(a.ptr[4] == 5);
        assert(a.ptr[5] == 0);
        assert(a.ptr[6] == 0);
        assert(a.ptr[7] == 0);
    }
}

void test_u8_array() {
    printf("Testing u8_array...\n");
    test_u8_array_of();
    test_u8_array_free();
    test_u8_array_set_length();
    test_u8_array_append();
}
