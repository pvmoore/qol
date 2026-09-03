#include "freelist.h"

FreeList freelist_of(u32 size) {
    FreeList f = {
        .size = size,
        .numUsed = 0,
        ._next = 0,
        ._data = malloc(size * sizeof(u32))
    };
    freelist_reset(&f);
    return f;
}
void freelist_delete(FreeList* f) {
    free(f->_data);
    f->_data = nullptr;
}
void freelist_reset(FreeList* f) {
    f->numUsed = 0;
    f->_next = 0;
    for(u32 i = 0; i < f->size; i++) {
        f->_data[i] = i + 1;
    }
}
u32 freelist_acquire(FreeList* f) {
    assert(f->numUsed < f->size);

    u32 index = f->_next;
    f->_next = f->_data[index];
    f->numUsed++;
    return index;
}
void freelist_release(FreeList* f, u32 index) {
    assert(index < f->size);
    assert(f->numUsed > 0);

    f->_data[index] = f->_next;
    f->_next = index;
    f->numUsed--;
}
