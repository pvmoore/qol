#include "qstring.h"

//#################################################################################### initialisers

/** Returns a string that wraps a char* */
string string_of(const char* buffer) {
    return (string){
        .buffer = buffer,
        .length = buffer ? (u32)strlen(buffer) : 0
    };
}
/** Returns a string that wraps a formatted char* */
string string_of_fmt(Arena* arena, const char* fmt, ...) {
    va_list	ap;
    va_start(ap, fmt);
    int count = vscprintf(fmt, ap) + 1;
    char* buffer = (char*)arena_alloc(arena, count);
    vsnprintf(buffer, count, fmt, ap);
    va_end(ap);
    return string_of(buffer);
}

/** Returns a string that wraps a char */
string string_of_char(char ch, Arena* arena) {
    char* buffer = arena_alloc(arena, 1);
    buffer[0] = ch;
    return (string){ .buffer = buffer, .length = 1 };
}

//#################################################################################### create cstr

/**
 * Returns a zero terminated copy of the string buffer as a const char*
 */
const char* cstr_of(string s, Arena* arena) {
    assert(arena);
    if(s.length == 0) return "";

    char* cstr = (char*)arena_alloc(arena, s.length + 1);
    memcpy(cstr, s.buffer, s.length);
    cstr[s.length] = '\0';
    return s.buffer;
}

//#################################################################################### substring

/**
 * Returns a new string that is a substring view  of s[start..end).
 * Note: s is not modified.
 */
string string_substring(string s, u32 start, u32 end) {
    assert(start <= end);
    assert(end <= s.length);

    return (string){ .buffer = s.buffer + start, .length = end - start };
}

//#################################################################################### append

/**
 * Returns a new string that is a copy of the combination of a + b.
 * Note: a and b are not modified.
 */
string string_append_string(string a, string b, Arena* arena) {
    assert(arena);

    u32 new_length = a.length + b.length;
    char* buffer = arena_alloc(arena, new_length);

    memcpy(buffer, a.buffer, a.length);
    memcpy(buffer + a.length, b.buffer, b.length);
    return (string){ .buffer = buffer, .length = new_length };
}

/** Returns a new string that is a copy of the combination of a + b */
string string_append_cstr(string a, const char* b, Arena* arena) {
    return string_append_string(a, string_of(b), arena);
}

string string_append_char(string a, char b, Arena* arena) {
    return string_append_string(a, string_of_char(b, arena), arena);
}



//#################################################################################### find

/** Returns the first index of the character c in the string haystack, or -1 if not found */
i32 string_find_char(string haystack, int c) {
    const char* pos = (char*)memchr(haystack.buffer, c, haystack.length);
    if(!pos) return -1;

    return (i32)(pos - haystack.buffer);
}
/** Returns the first index of the string needle in the string haystack, or -1 if not found */
i32 string_find_string(string haystack, string needle) {
    if(haystack.length == 0 || needle.length == 0) return -1;
    if(needle.length > haystack.length) return -1;
    if(needle.length == 1) return string_find_char(haystack, needle.buffer[0]);

    u32 end = haystack.length - needle.length;

    for(u32 pos = 0; pos <= end; pos++) {
        if(haystack.buffer[pos] != needle.buffer[0]) continue;

        if(memcmp(haystack.buffer + pos + 1, needle.buffer + 1, needle.length - 1) == 0) {
            return pos;
        }
    }
    return -1;
}
i32 string_find_cstr(string haystack, const char* needle) {
    return string_find_string(haystack, string_of(needle));

}

//#################################################################################### contains

/** Returns true if the string haystack contains the character c */
bool string_contains_char(string haystack, int c) {
    return string_find_char(haystack, c) != -1;
}

/** Returns true if the string haystack contains the string needle */
bool string_contains_string(string haystack, string needle) {
    return string_find_string(haystack, needle) != -1;
}
/** Returns true if the string haystack contains the cstr needle */
bool string_contains_cstr(string s, const char* needle) {
    return string_find_cstr(s, needle) != -1;
}

//#################################################################################### equals

/** Returns true if the strings a and b are equal */
bool string_equals_string(string a, string b) {
    return a.length == b.length && memcmp(a.buffer, b.buffer, a.length) == 0;
}
/** Returns true if the string a and the cstr b are equal */
bool string_equals_cstr(string a, const char* b) {
    if(!b) return a.length == 0;
    return a.length == strlen(b) && memcmp(a.buffer, b, a.length) == 0;
}


