/* File: Graphics Math Library */

#pragma once
#include "math.h"

struct vec2 {
    double x, y;
};

struct vec2 vec2_add(const struct vec2* v, const struct vec2* w);
struct vec2 vec2_sub(const struct vec2* v, const struct vec2* w);
struct vec2 vec2_mul(const struct vec2* v, double k);
struct vec2 vec2_div(const struct vec2* v, double k);
double vec2_dot(const struct vec2* v, const struct vec2* w);
double vec2_length_square(const struct vec2* v);
double vec2_length(const struct vec2* v);
struct vec2 vec2_normalize(const struct vec2* v);

struct vec3 {
    double x, y, z;
};

struct vec3 vec3_add(const struct vec3* v, const struct vec3* w);
struct vec3 vec3_sub(const struct vec3* v, const struct vec3* w);
struct vec3 vec3_mul(const struct vec3* v, double k);
struct vec3 vec3_div(const struct vec3* v, double k);
double vec3_dot(const struct vec3* v, const struct vec3* w);
double vec3_length_square(const struct vec3* v);
double vec3_length(const struct vec3* v);
struct vec3 vec3_normalize(const struct vec3* v);

struct vec4 {
    double x, y, z, w;
};

struct vec4 vec4_add(const struct vec4* v, const struct vec4* w);
struct vec4 vec4_sub(const struct vec4* v, const struct vec4* w);
struct vec4 vec4_mul(const struct vec4* v, double k);
struct vec4 vec4_div(const struct vec4* v, double k);
double vec4_dot(const struct vec4* v, const struct vec4* w);
double vec4_length_square(const struct vec4* v);
double vec4_length(const struct vec4* v);
struct vec4 vec4_normalize(const struct vec4* v);

struct mat3 {
    double d[9];
};

struct mat4 {
    double d[16];
};

void swap_vec2(struct vec2* v, struct vec2* w);
void swap_vec3(struct vec3* v, struct vec3* w);
void swap_vec4(struct vec4* v, struct vec4* w);

/*=============== GENERAL MATH ===============*/
double lerp(double v, double w, double t);
struct vec2 vec2_lerp(const struct vec2* v, const struct vec2* w, double t);
struct vec3 vec3_lerp(const struct vec3* v, const struct vec3* w, double t);
struct vec4 vec4_lerp(const struct vec4* v, const struct vec4* w, double t);

