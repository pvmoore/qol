# Notes

## C Language Version Changes

### C29 (C2y) Features

Octal literals (0o00, 0O00)
_Countof, countof() <stdcountof.h>
defer
if statement with variable initialiser
switch case ranges eg case 1...3:
named labels for break and continue
__COUNTER__ macro
strnlen()
stdc_memreverse8()
stdc_rotate_left_*()
stdc_rotate_right_*()
(where * = uc,us,ui,ul,ull)
stdc_load8_le*()
stdc_store8_le*()
(where * = u/s N eg. stdc_load8_leu16)

### C23 Features

nullptr
' digit separator
Binary literal conatnts 0b000 0B000
enum underlying types
true and false keywords
alignas,alignof, bool, static_assert, thread_load keywords
typeof and typeof_unqual
constexpr
Checked integer operations ckd_add(), ckd_sub(), ckd_mul
auto type inference
#embed
#warning
#elifdef #elifndef
<stdbit.h> 
    stdc_count_ones_*() stdc_count_zeroes_*() 
    stdc_leading_ones_*() stdc_leading_zeroes_*()
    stdc_trailing_ones_*() stdc_trailing_zeroes_*()
    stdc_first_leading_one_*() stdc_first_leading_zero_*()
    stdc_first_trailing_one_*() stdc_first_trialing_zero_*()
    atdc_has_single_bit_*()
    stdc_bit_floor_*()
    stdc_bit_ceil_*()
    stdc_bit_width_*()
    (where * = uc,us,ui,ul,ull)
timegm()

### C17 Features

This is C11 with a few fixes. No new features introduced.

### C11 Features

_Alignas
_Alignof
aligned_alloc()
<stdalign.h>
_Noreturn
<stdnoreturn.h>
_Generic
_Thread_local
<threads.h>
<stdatomic.h>
<uchar.h>
u,U and u8 string prefixes eg u8"unicode utf-8 string"
gets() removed, replaced with fgets()
Anonymous structures and unions
Bounds checking interfaces
Static assertions
quick_exit()
timespec_get()
