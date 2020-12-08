#include "fast_math.h"
#include <algorithm>

float fast_sqrt(float x)
{
    unsigned int i = *(unsigned int*)&x;
    // adjust bias
    i += 127 << 23;
    // approximation of square root
    i >>= 1;
    return *(float*)&i;
}

float fast_pow(float x, int y) {
    float temp;
    if (y == 0) return 1; //base case

    //recursion
    temp = fast_pow(x, y / 2);
    if (y % 2 == 0) return temp * temp;
    else return x * temp * temp;
}

void vec3_cull(vec3 r) {
    r[0] = std::min(1.0f, r[0]);
    r[1] = std::min(1.0f, r[1]);
    r[2] = std::min(1.0f, r[2]);
}

void vec3_fraction(vec3 r, const vec3 a, const vec4 b) {
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
}

void rgb_to_grayscale(vec3 r, const vec3 rgb) {
    for (int i = 0; i < 3; i++) {
        r[i] = 0.3f * rgb[0] + 0.59f * rgb[1] + 0.11f * rgb[2];
    }
}

float sqr(float x) { return x * x; }

void vec3_copy(vec3 i, vec3 n) {
    i[0] = n[0];
    i[1] = n[1];
    i[2] = n[2];
}

void vec3_div(vec3 out, vec3 i, float n) {
    out[0] = i[0] / n;
    out[1] = i[1] / n;
    out[2] = i[2] / n;
}

