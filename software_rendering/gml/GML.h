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

struct ivec2_t {
    int x, y;
};

struct dvec2_t {
    double x, y;
};

struct ivec3_t {
    int x, y, z;
};

struct dvec3_t {
    double x, y, z;
};

void swap_ivec3(struct ivec3_t* v, struct ivec3_t* w);
void swap_dvec3(struct dvec3_t* v, struct dvec3_t* w);
void swap_ivec2(struct ivec2_t* v, struct ivec2_t* w);
void swap_dvec2(struct dvec2_t* v, struct dvec2_t* w);

struct ivec3_t ivec2_to_ivec3(const struct ivec2_t v);
struct dvec3_t dvec2_to_dvec3(const struct dvec2_t v);
struct dvec3_t ivec3_to_dvec3(const struct ivec3_t v);
struct ivec3_t dvec3_to_ivec3(const struct dvec3_t v);

struct ivec3_t add_ivec3(const struct ivec3_t v, const struct ivec3_t w);
struct dvec3_t add_dvec3(const struct dvec3_t v, const struct dvec3_t w);
struct ivec2_t add_ivec2(const struct ivec2_t v, const struct ivec2_t w);
struct dvec2_t add_dvec2(const struct dvec2_t v, const struct dvec2_t w);

struct ivec3_t sub_ivec3(const struct ivec3_t v, const struct ivec3_t w);
struct dvec3_t sub_dvec3(const struct dvec3_t v, const struct dvec3_t w);
struct ivec2_t sub_ivec2(const struct ivec2_t v, const struct ivec2_t w);
struct dvec2_t sub_dvec2(const struct dvec2_t v, const struct dvec2_t w);

int dot_ivec3(const struct ivec3_t v, const struct ivec3_t w);
int dot_ivec2(const struct ivec2_t v, const struct ivec2_t w);
double dot_dvec3(const struct dvec3_t v, const struct dvec3_t w);
double dot_dvec2(const struct dvec2_t v, const struct dvec2_t w);

struct dvec3_t mul_dvec3_d(const struct dvec3_t v, double k);
struct dvec2_t mul_dvec2_d(const struct dvec2_t v, double k);

double lerp(double v, double w, double t);
