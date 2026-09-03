#include "qol.h"
#include "arena.h"
#include "qstring.h"
#include "freelist.h"

#include "test_array.c"
#include "test_u8_array.c"
#include "test_string_map.c"

void testArena();
void testString();
void testFreeList();
void testStringMap();

int main(int argc, char** argv) {
	testArena();
    testString();
    testFreeList();
    testArray();
    test_u8_array();
    testStringMap();

    printf("All tests passed!\n");
	return 0;
}

void testArena() {
    printf("Testing Arena...\n");
    Arena arena = arena_of(1024 * 1024);
    assert(arena.buffer);
    assert(arena.pos == 0);
    assert(arena.capacity == 1024 * 1024);

    void* p1 = arena_alloc(&arena, 10);
    assert(p1);
    assert(p1 == arena.buffer);
    assert(arena.pos == 10);
    assert(arena.capacity == 1024 * 1024);

    void* p2 = arena_alloc(&arena, 10);
    assert(p2);
    assert(p2 == arena.buffer + 10);
    assert(arena.pos == 20);
    assert(arena.capacity == 1024 * 1024);

    void* p3 = arena_alloc_aligned(&arena, 10, 16);
    assert(p3);
    assert(p3 == arena.buffer + 32);
    assert(arena.pos == 42);
    assert(arena.capacity == 1024 * 1024);

    void* p4 = arena_alloc(&arena, 3);
    assert(p4);
    assert(p4 == arena.buffer + 42);
    assert(arena.pos == 45);
    assert(arena.capacity == 1024 * 1024);

    void* p5 = arena_alloc_aligned(&arena, 4, 4);
    assert(p5);
    assert(p5 == arena.buffer + 48);
    assert(arena.pos == 52);
    assert(arena.capacity == 1024 * 1024);

    arena_delete(&arena);

    assert(arena.buffer == NULL);
    assert(arena.pos == 0);
    assert(arena.capacity == 0);
}
void testString() {
    printf("Testing String...\n");
    Arena arena = arena_of(1024 * 1024);

    {   // string_of
        string s1 = string_of("hello world");
        assert(s1.buffer);
        assert(s1.length == 11);

        const char* src = "hello";
        string s2 = string_of(src);
        assert(s2.buffer);
        assert(s2.buffer == src);
        assert(s2.length == 5);

        string s3 = string_of(NULL);
        assert(s3.buffer == NULL);
        assert(s3.length == 0);
    }
    {   // string_of_fmt
        string s1 = string_of_fmt(&arena, "hello %s", "world");
        assert(s1.buffer);
        assert(s1.length == 11);
        assert(memcmp(s1.buffer, "hello world", 11) == 0);

        string s2 = string_of_fmt(&arena, "%.3f", 123.456);
        assert(s2.buffer);
        assert(s2.length == 7);
        assert(memcmp(s2.buffer, "123.456", 7) == 0);

        string s3 = string_of_fmt(&arena, "%d", 1234567890);
        assert(s3.buffer);
        assert(s3.length == 10);
        assert(memcmp(s3.buffer, "1234567890", 10) == 0);

        string s4 = string_of_fmt(&arena, "%lld", -1234567890LL);
        assert(s4.buffer);
        assert(s4.length == 11);
        assert(memcmp(s4.buffer, "-1234567890", 11) == 0);
    }
    {   // string_of_char
        string s1 = string_of_char('a', &arena);
        assert(s1.buffer);
        assert(s1.length == 1);
        assert(memcmp(s1.buffer, "a", 1) == 0);
    }
    {   // cstr_of
        string s1 = string_of("hello world");
        const char* cstr = cstr_of(s1, &arena);
        assert(cstr);
        assert(strlen(cstr) == 11);
        assert(memcmp(cstr, "hello world\0", 12) == 0);

        string s2 = string_of(NULL);
        const char* cstr2 = cstr_of(s2, &arena);
        assert(cstr2);
        assert(strlen(cstr2) == 0);
        assert(memcmp(cstr2, "\0", 1) == 0);

        string s3 = string_of("");
        const char* cstr3 = cstr_of(s3, &arena);
        assert(cstr3);
        assert(strlen(cstr3) == 0);
        assert(memcmp(cstr3, "\0", 1) == 0);

        char* p = "hello";
        string s4 = string_of(p);
        const char* cstr4 = cstr_of(s4, &arena);
        assert(cstr4);
        assert(strlen(cstr4) == 5);
        assert(memcmp(cstr4, "hello", 5) == 0);
    }
    {   // string_substring
        string s1 = string_of("hello world");
        string s2 = string_substring(s1, 0, 5);
        assert(s2.buffer);
        assert(s2.length == 5);
        assert(memcmp(s2.buffer, "hello", 5) == 0);

        string s3 = string_substring(s1, 6, 11);
        assert(s3.buffer);
        assert(s3.length == 5);
        assert(memcmp(s3.buffer, "world", 5) == 0);

        string s4 = string_substring(s1, 0, 0);
        assert(s4.buffer);
        assert(s4.length == 0);
    }
    {   // string_append_string
        string s1 = string_of("hello ");
        string s2 = string_of("world");
        string s3 = string_append(s1, s2, &arena);
        assert(s3.buffer);
        assert(s3.length == 11);
        assert(memcmp(s3.buffer, "hello world", 11) == 0);

        string s4 = string_append(s1, string_of(""), &arena);
        assert(s4.buffer);
        assert(s4.length == 6);
        assert(memcmp(s4.buffer, "hello ", 6) == 0);
    }
    {   // string_append_cstr
        string s1 = string_of("hello ");
        string s2 = string_append_cstr(s1, "world", &arena);
        assert(s2.buffer);
        assert(s2.length == 11);
        assert(memcmp(s2.buffer, "hello world", 11) == 0);
    }
    {   // string_append_char
        string s1 = string_of("hello ");
        string s2 = string_append(s1, 'w', &arena);
        assert(s2.buffer);
        assert(s2.length == 7);
        assert(memcmp(s2.buffer, "hello w", 7) == 0);
    }
    {   // string_equals_string
        const char p[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
        string s1 = string_of("hello");
        string s2 = string_of(p);
        string s3 = string_of("hellO");

        assert(string_equals(s1, s2) == true);
        assert(string_equals(s1, s3) == false);
        assert(string_equals(s2, s3) == false);
        assert(string_equals_string(s1, s2) == true);
        assert(string_equals_string(s1, s3) == false);
        assert(string_equals_string(s2, s3) == false);
    }
    {   // string_equals_cstr
        const char p[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
        string s1 = string_of("hello");
        string s2 = string_of(p);

        assert(string_equals(s1, "hello") == true);
        assert(string_equals(s1, "hellO") == false);
        assert(string_equals(s2, "hellO") == false);
        assert(string_equals_cstr(s1, "hello") == true);
        assert(string_equals_cstr(s1, "hellO") == false);
        assert(string_equals_cstr(s2, "hellO") == false);
    }
    {   // string_contains_char
        string s1 = string_of("hello");

        assert(string_contains(s1, 'h') == true);
        assert(string_contains(s1, 'o') == true);
        assert(string_contains(s1, 'H') == false);
        assert(string_contains(s1, '\0') == false);
    }
    {   // string_contains_cstr
        string s1 = string_of("hello");

        assert(string_contains(s1, "") == false);
        assert(string_contains(s1, (char*)NULL) == false);
        assert(string_contains(s1, "H") == false);

        assert(string_contains(s1, "h") == true);
        assert(string_contains(s1, "hel") == true);
        assert(string_contains(s1, "o") == true);
        assert(string_contains(s1, "hello") == true);
    }
    {   // string_contains_string
        string s1 = string_of("hello");
        string s2 = string_of("he");
        string s3 = string_of("ll");
        string s4 = string_of("o");
        string s5 = string_of("lo");

        assert(string_contains(s1, string_of("")) == false);
        assert(string_contains(s1, string_of((char*)NULL)) == false);
        assert(string_contains(s1, string_of("H")) == false);

        assert(string_contains(s1, s2) == true);
        assert(string_contains(s1, s3) == true);
        assert(string_contains(s1, s4) == true);
        assert(string_contains(s1, s5) == true);
    }
    {   // string_find_char
        string s1 = string_of("hello");

        assert(string_find(s1, 'h') == 0);
        assert(string_find(s1, 'o') == 4);
        assert(string_find(s1, 'H') == -1);
        assert(string_find(s1, '\0') == -1);
    }
    {   // string_find_string
        string s1 = string_of("hello");
        string s2 = string_of("he");
        string s3 = string_of("ll");
        string s4 = string_of("o");
        string s5 = string_of("lo");

        assert(string_find(s1, s2) == 0);
        assert(string_find(s1, s3) == 2);
        assert(string_find(s1, s4) == 4);
        assert(string_find(s1, s5) == 3);
        assert(string_find(s1, string_of("")) == -1);
        assert(string_find(s1, string_of((char*)NULL)) == -1);
        assert(string_find(s1, string_of("H")) == -1);
    }
    {   // string_find_cstr
        string s1 = string_of("hello");

        assert(string_find(s1, "he") == 0);
        assert(string_find(s1, "ll") == 2);
        assert(string_find(s1, "o") == 4);
        assert(string_find(s1, "lo") == 3);
        assert(string_find(s1, "") == -1);
        assert(string_find(s1, (char*)NULL) == -1);
        assert(string_find(s1, "H") == -1);
    }
}
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

