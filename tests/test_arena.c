#include "qol.h"
#include "arena.h"

void testArenaAlloc() {
    printf("Testing Arena...\n");
    Arena arena = arena_of(1024 * 1024);
    assert(arena.buffer);
    assert(arena.pos == 0);
    assert(arena.capacity == 1024 * 1024);

    void* p1 = arena_alloc(&arena, 10, 1);
    assert(p1);
    assert(p1 == arena.buffer);
    assert(arena.pos == 10);
    assert(arena.capacity == 1024 * 1024);

    void* p2 = arena_alloc(&arena, 10, 1);
    assert(p2);
    assert(p2 == arena.buffer + 10);
    assert(arena.pos == 20);
    assert(arena.capacity == 1024 * 1024);

    void* p3 = arena_alloc(&arena, 10, 16);
    assert(p3);
    assert(p3 == arena.buffer + 32);
    assert(arena.pos == 42);
    assert(arena.capacity == 1024 * 1024);

    void* p4 = arena_alloc(&arena, 3, 1);
    assert(p4);
    assert(p4 == arena.buffer + 42);
    assert(arena.pos == 45);
    assert(arena.capacity == 1024 * 1024);

    void* p5 = arena_alloc(&arena, 4, 4);
    assert(p5);
    assert(p5 == arena.buffer + 48);
    assert(arena.pos == 52);
    assert(arena.capacity == 1024 * 1024);

    arena_delete(&arena);

    assert(arena.buffer == NULL);
    assert(arena.pos == 0);
    assert(arena.capacity == 0);
}

void testArenaReset() {
    printf("Testing Arena Reset...\n");
    Arena arena = arena_of(1024 * 1024);

    void* p1 = arena_alloc(&arena, 10, 1);
    memset(p1, 1, 10);

    arena_reset(&arena);

    assert(arena.pos == 0);
    for(u32 i = 0; i<arena.capacity; i++) assert(arena.buffer[i] == 0);
}

void testArena() {
    testArenaAlloc();
    testArenaReset();
}
