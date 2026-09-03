#include "qol.h"
#include "arena.h"
#include "qstring.h"
#include "freelist.h"
#include <time.h>

#define QOL_ARRAY_TYPE u64
#include "array.h"

void benchmarkString(Arena*);
void benchmarkFreeList();
void benchmarkArray();

// Note: rand() only produces random numbers in the range 0..INT_MAX
u32 randomUint(u32 exclusiveMax) { return (u32)rand() % exclusiveMax; }
char randomChar() { return (char)('a' + (randomUint(26))); }

u64 nanos() {
	struct timespec ts;
    timespec_get(&ts, TIME_UTC);
	return (u64)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

int main(int argc, char** argv) {

    Arena arena = arena_of(1024 * 1024 * 10);
    benchmarkString(&arena);
    benchmarkFreeList();
    benchmarkArray();

	return 0;
}
void benchmarkString(Arena* arena) {
    printf("Benchmarking String...\n");

    //srand(0);
    srand(time(NULL));

    const u32 N    = 10'000;
    const u32 HLEN = 1024*1024;
    const u32 NLEN = 10;

    char* buffer = (char*)arena_alloc(arena, HLEN);
    for(u32 i=0; i<HLEN; i++) buffer[i] = randomChar();
    buffer[HLEN-1] = '\0';

    string haystack = string_of(buffer);
    string needle = string_of(buffer + (HLEN-1) - NLEN);
    u32 count = 0;

    u64 start = nanos();
    for(u32 i = 0; i < N; i++) {
        if(string_contains_string(haystack, needle)) count++;
    }
    u64 end = nanos();

    printf("  count = %u\n", count);
    printf("  Took %.3f seconds\n", (end - start) / 1'000'000'000.0);
}
void benchmarkFreeList() {
    printf("Benchmarking FreeList...\n");

    //srand(0);
    srand(time(NULL));

    const u32 N = 1'000'000;

    u32* R = malloc(N * sizeof(u32));
    for(u32 i = 0; i < N; i++) R[i] = randomUint(100);

    u32* acquired = malloc(N * sizeof(u32));
    u32 numAcquired = 0;
    u32 numReleased = 0;

    FreeList f = freelist_of(N);

    u64 start = nanos();

    for(u32 i = 0; i < N; i++) {
        if(R[i] < 80) {
            acquired[numAcquired++] = freelist_acquire(&f);
        } else if(numReleased < numAcquired) {
            u32 a = acquired[--numAcquired];
            freelist_release(&f, a);
            numReleased++;
        }
    }
    u64 end = nanos();

    printf("  numAcquired: %u, numReleased: %u\n", numAcquired, numReleased);
    printf("  Took %.3f seconds\n", (end - start) / 1'000'000'000.0);
}
void benchmarkArray() {
    printf("Benchmarking Array...\n");

    const u32 N = 1'000'000;

    u64_array a = u64_array_of(0);

    u64 start = nanos();
    for(u32 i = 0; i < N; i++) {
        u64_array_append(&a, i);
    }
    u64 end = nanos();

    printf("  Took %.3f seconds\n", (end - start) / 1'000'000'000.0);
}
