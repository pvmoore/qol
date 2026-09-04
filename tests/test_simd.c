#include "qol.h"

typedef float float2 __attribute__((ext_vector_type(2)));
typedef float float3 __attribute__((ext_vector_type(3)));
typedef float float4 __attribute__((ext_vector_type(4)));
typedef float float8 __attribute__((ext_vector_type(8)));
typedef float float8b __attribute__((vector_size(8)));          // what is the difference between float8 and float8b?

typedef int int4 __attribute__((ext_vector_type(4)));
typedef int int8 __attribute__((ext_vector_type(8)));

typedef bool bool4 __attribute__((ext_vector_type(4)));
typedef bool bool8 __attribute__((ext_vector_type(8)));

bool bool8_equal(bool8 a, bool8 b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] && a[4] == b[4] && a[5] == b[5] && a[6] == b[6] && a[7] == b[7];
}
bool float2_equal(float2 a, float2 b) {
    return a[0] == b[0] && a[1] == b[1];
}
bool float4_equal(float4 a, float4 b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}
bool float8_equal(float8 a, float8 b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] && a[4] == b[4] && a[5] == b[5] && a[6] == b[6] && a[7] == b[7];
}
bool int4_equal(int4 a, int4 b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}
void dump(const char* prefix, int8 a) {
    printf("%s%d %d %d %d %d %d %d %d\n", prefix, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
}

void simpleExamples() {
    float4 z = {0.0f, 1.0f, 2.0f, 3.0f};
    assert(z.x == 0.0f);
    assert(z.y == 1.0f);
    assert(z.z == 2.0f);
    assert(z.w == 3.0f);

    float3 y = z.xyw;
    assert(y.x == 0.0f);
    assert(y.y == 1.0f);
    assert(y.z == 3.0f);

    float2 a;
    a.x = 1.0f;
    a.y = 2.0f;
    assert(a.x == 1.0f);
    assert(a.y == 2.0f);

    float2 b = a + a;
    assert(b.x == 2.0f);
    assert(b.y == 4.0f);

    // c = {1,1,1,1,1,1,1,1}
    float8 c = 1;
    assert(c.x == 1);
    assert(c[0] == 1);
    assert(float8_equal(c, (float8){1,1,1,1,1,1,1,1}));

    // Component-wise equality
    bool8 c2 = c == 1;
    assert(bool8_equal(c2, (bool8){1,1,1,1,1,1,1,1}));

    float8 d = {1,1,1,1,1,1,1,1};
    assert(float8_equal(c, d));


    bool4 m     = { true, false, true, false };
    int4 select = m ? (int4){1, 2, 3, 4} : (int4){5, 6, 7, 8};
    assert(int4_equal(select, (int4){1,6,3,8}));

    float4 o = __builtin_convertvector(select, float4);

    float4 p = __builtin_shufflevector(o, o, 0, 1, 2, 3);
}

void floatOperations() {
    float4 a = {1.0f, -2.0f, 3.0f, -4.0f};

    float4 b = __builtin_elementwise_abs(a);
    assert(float4_equal(b, (float4){1.0f, 2.0f, 3.0f, 4.0f}));

    float4 c = __builtin_elementwise_fma(a, b, (float4){1,2,3,4});
    assert(float4_equal(c, (float4){2, -2, 12, -12}));


    float d = __builtin_reduce_max(a);
    assert(d == 3.0f);

}

void integerOperations() {
    int8 a = {1,2,3,4,5,6,7,8};

    int b = __builtin_reduce_max(a);
    assert(b == 8);

    int c = __builtin_reduce_add(a);
    assert(c == 36);


}

int find(int8 haystack, int needle) {
    // This will produce result where the element is -1 if it matches
    int8 b = haystack == needle;

    int8 c = b * (int8){8,7,6,5,4,3,2,1};

    int d = __builtin_reduce_min(c);

    if(d != 0) {
        return 8+d;
    }

    return -1;
}

void testSimd() {
    simpleExamples();
    floatOperations();
    integerOperations();

    int8 haystack = {1,2,3,4,5,6,2,8};
    //assert(find(haystack, 5) == 4);
    printf("needle = %d\n", find(haystack, 2));
}
