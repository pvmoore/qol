#include "qol.h"
#include "freelist.h"

void testFreeList() {
    printf("Testing FreeList...\n");
    FreeList f = freelist_of(1000);
    assert(f._data);
    assert(f.size == 1000);
    assert(f.numUsed == 0);

    u32 index = freelist_acquire(&f);
    assert(index == 0);
    assert(f.numUsed == 1);

    u32 index2 = freelist_acquire(&f);
    assert(index2 == 1);
    assert(f.numUsed == 2);

    freelist_release(&f, index);
    assert(f.numUsed == 1);

    u32 index3 = freelist_acquire(&f);
    assert(index3 == index);
    assert(f.numUsed == 2);

    assert(f.size == 1000);
    freelist_delete(&f);
}
