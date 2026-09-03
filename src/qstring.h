#pragma once

#include "qol.h"
#include "arena.h"

// Wrapper for a const char*
//
// This is not modifiable.
// eg.
// string s = string_of("hello");

 //----------------------------------------------------------------------------------
typedef struct {
    const char* buffer;       // Note: Don't assume this points to a zero terminated string
    u32 length;
} string;
static_assert(sizeof(string) == 16);

//----------------------------------------------------------------------------------
string string_of(const char* buffer);
string string_of_fmt(Arena* arena, const char* fmt, ...);
string string_of_char(char ch, Arena* arena);
const char* cstr_of(string s, Arena* arena);
string string_substring(string s, u32 start, u32 end);

// string string_append(string a, [ string | int | char*], Arena* arena);
string string_append_string(string a, string b, Arena* arena);
string string_append_cstr(string a, const char* b, Arena* arena);
string string_append_char(string a, char b, Arena* arena);

// i32 string_find(string haystack, [ string | int | char*]);
i32 string_find_char(string haystack, int c);
i32 string_find_string(string haystack, string needle);
i32 string_find_cstr(string haystack, const char* needle);

// bool string_contains(string haystack, [ string | int | char*]);
bool string_contains_char(string haystack, int c);
bool string_contains_string(string haystack, string needle);
bool string_contains_cstr(string haystack, const char* needle);

// bool string_equals(string a, [ string | int | char*]);
bool string_equals_cstr(string a, const char* b);
bool string_equals_string(string a, string b);

#define string_append(a,b,c) _Generic((b), string: string_append_string, \
                                           int: string_append_char, \
                                           char*: string_append_cstr \
                                           )(a, b, c)

#define string_equals(a,b) _Generic((b), string:       string_equals_string, \
                                         char*:        string_equals_cstr \
                                   )(a, b)

#define string_contains(a,b) _Generic((b), int:        string_contains_char, \
                                           char*:      string_contains_cstr, \
                                           string:     string_contains_string \
                                     )(a, b)

#define string_find(a,b) _Generic((b), int:     string_find_char, \
                                       char*:   string_find_cstr, \
                                       string:  string_find_string \
                                       )(a, b)
