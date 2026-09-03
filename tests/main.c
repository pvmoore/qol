
#include "qol.h"
#include "arena.h"
#include "qstring.h"

int main(int argc, char** argv) {
	printf("Hello, QOL!\n");

	Arena arena = arena_of(1024 * 1024);


	arena_delete(&arena);
	return 0;
}
