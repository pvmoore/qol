# Clang specific notes and links

## Links

[Language Extensions](https://clang.llvm.org/docs/LanguageExtensions.html)

## Alignment

Type __builtin_align_up(Type value, size_t alignment);
Type __builtin_align_down(Type value, size_t alignment);
bool __builtin_is_aligned(Type value, size_t alignment);

## Attributes

[[clang::optnone]] void foo() {}
[[clang::always_inline]]

## Pragmas

#pragma clang loop vectorize(enable)
#pragma clang loop unroll(enable)

## SIMD

```c
typedef float float2 __attribute__((ext_vector_type(2)));
typedef float float3 __attribute__((ext_vector_type(3)));
typedef float float4 __attribute__((ext_vector_type(4)));
typedef float float8 __attribute__((ext_vector_type(8)));


```
