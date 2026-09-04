
#include "test_arena.c"
#include "test_array.c"
#include "test_freelist.c"
#include "test_simd.c"
#include "test_string.c"
#include "test_string_map.c"
#include "test_u8_array.c"

int main(int argc, char** argv) {
	testArena();
    testString();
    testFreeList();
    testArray();
    test_u8_array();
    testStringMap();
    testSimd();

    printf("All tests passed!\n");

    #ifdef __clang__

    printf("Clang version %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);

    #endif

	return 0;
}


