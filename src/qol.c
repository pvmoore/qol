#include "qol.h"

#include "arena.c"
#include "string.c"
#include "freelist.c"
#include "string_map.c"

/**
 * Returns the number of characters that would be written by vsnprintf (excluding the null terminator).
 * This is defined in stdio.h on Windows (as _vscprintf) but is non standard
 */
int vscprintf(const char* format, va_list pargs) {
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}

u32 popcnt(u32 x) {
    // Note: This may be GCC and Clang specific
    return __builtin_popcount(x);
}
