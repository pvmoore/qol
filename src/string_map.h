#pragma once

#include "qol.h"
#include "qstring.h"
#include "arena.h"

// Opaque struct
typedef struct {} string_map;

string_map* string_map_of           (Arena* arena, u32 capacity, float loadFactor);

bool        string_map_is_empty     (string_map* m);
u32         string_map_size         (string_map* m);
u32         string_map_capacity     (string_map* m);
float       string_map_load_factor  (string_map* m);

void        string_map_insert       (string_map* m, string key, string value);
string*     string_map_get          (string_map* m, string key);
bool        string_map_contains_key (string_map* m, string key);
bool        string_map_remove       (string_map* m, string key);
void        string_map_clear        (string_map* m, bool freeMemory);
u32         string_map_keys         (string_map* m, string* keysOut);
u32         string_map_values       (string_map* m, string* valuesOut);
void        string_map_dump         (string_map* m);
