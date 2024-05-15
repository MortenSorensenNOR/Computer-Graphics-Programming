#include "GML.h"

/*=============== VEC2 ===============*/
struct vec2 vec2_add(const struct vec2* v, const struct vec2* w) {
    return (struct vec2){v->x + w->x, v->y + w->y};
}

struct vec2 vec2_sub(const struct vec2* v, const struct vec2* w) {
    return (struct vec2){v->x - w->x, v->y - w->y};
}

struct vec2 vec2_mul(const struct vec2* v, double k) {
    return (struct vec2){v->x * k, v->y * k};
}

struct vec2 vec2_div(const struct vec2* v, double k) {
    return (struct vec2){v->x / k, v->y / k};
}

double vec2_dot(const struct vec2* v, const struct vec2* w) {
    return v->x * w->x + v->y * w->y;
}

double vec2_length_square(const struct vec2* v) {
    return v->x*v->x + v->y*v->y;
}

double vec2_length(const struct vec2* v) {
    return sqrt(v->x*v->x + v->y*v->y);
}

struct vec2 vec2_normalize(const struct vec2* v) {
    return vec2_div(v, vec2_length(v));
}

void swap_vec2(struct vec2* v, struct vec2* w) {
    struct vec2 tmp = *v;
    *v = *w;
    *w = tmp;
}

/*=============== VEC3 ===============*/
struct vec3 vec3_add(const struct vec3* v, const struct vec3* w) {
    return (struct vec3){v->x + w->x, v->y + w->y, v->z + w->z};
}

struct vec3 vec3_sub(const struct vec3* v, const struct vec3* w) {
    return (struct vec3){v->x - w->x, v->y - w->y, v->z - w->z};
}

struct vec3 vec3_mul(const struct vec3* v, double k) {
    return (struct vec3){v->x * k, v->y * k, v->z * k};
}

struct vec3 vec3_div(const struct vec3* v, double k) {
    return (struct vec3){v->x / k, v->y / k, v->z / k};
}

double vec3_dot(const struct vec3* v, const struct vec3* w) {
    return v->x * w->x + v->y * w->y + v->z * w->z;
}

double vec3_length_square(const struct vec3* v) {
    return v->x*v->x + v->y*v->y + v->z*v->z;
}

double vec3_length(const struct vec3* v) {
    return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

struct vec3 vec3_normalize(const struct vec3* v) {
    return vec3_div(v, vec3_length(v));
}

void swap_vec3(struct vec3* v, struct vec3* w) {
    struct vec3 tmp = *v;
    *v = *w;
    *w = tmp;
}

/*=============== VEC4 ===============*/
struct vec4 vec4_add(const struct vec4* v, const struct vec4* w) {
    return (struct vec4){v->x + w->x, v->y + w->y, v->z + w->z, v->w + w->w};
}

struct vec4 vec4_sub(const struct vec4* v, const struct vec4* w) {
    return (struct vec4){v->x - w->x, v->y - w->y, v->z - w->z, v->w - w->w};
}

struct vec4 vec4_mul(const struct vec4* v, double k) {
    return (struct vec4){v->x * k, v->y * k, v->z * k, v->w * k};
}

struct vec4 vec4_div(const struct vec4* v, double k) {
    return (struct vec4){v->x / k, v->y / k, v->z / k, v->w / k};
}

double vec4_dot(const struct vec4* v, const struct vec4* w) {
    return v->x * w->x + v->y * w->y + v->z * w->z + v->w * w->w;
}

double vec4_length_square(const struct vec4* v) {
    return v->x*v->x + v->y*v->y + v->z*v->z + v->w*v->w;
}

double vec4_length(const struct vec4* v) {
    return sqrt(v->x*v->x + v->y*v->y + v->z*v->z + v->w*v->w);
}

struct vec4 vec4_normalize(const struct vec4* v) {
    return vec4_div(v, vec4_length(v));
}

void swap_vec4(struct vec4* v, struct vec4* w) {
    struct vec4 tmp = *v;
    *v = *w;
    *w = tmp;
}

/*=============== GENERAL MATH ===============*/
double lerp(double v, double w, double t) {
    return v + t * (w - v);
}

struct vec2 vec2_lerp(const struct vec2* v, const struct vec2* w, double t) {
    return (struct vec2){lerp(v->x, w->x, t), lerp(v->y, w->y, t)};
}

struct vec3 vec3_lerp(const struct vec3* v, const struct vec3* w, double t) {
    return (struct vec3){lerp(v->x, w->x, t), lerp(v->y, w->y, t), lerp(v->z, w->z, t)};
}

struct vec4 vec4_lerp(const struct vec4* v, const struct vec4* w, double t) {
    return (struct vec4){lerp(v->x, w->x, t), lerp(v->y, w->y, t), lerp(v->z, w->z, t), lerp(v->w, w->w, t)};
}

