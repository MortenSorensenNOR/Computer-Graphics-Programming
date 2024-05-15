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

double lerp(double v, double w, double t) {
    return v + t * (w - v);
}

void swap_ivec3(struct ivec3_t* v, struct ivec3_t* w) {
    struct ivec3_t tmp = (struct ivec3_t){v->x, v->y, v->z};
    v->x = w->x;
    v->y = w->y;
    v->z = w->z;
    w->x = tmp.x;
    w->y = tmp.y;
    w->z = tmp.z;
}

void swap_dvec3(struct dvec3_t* v, struct dvec3_t* w) {
    struct dvec3_t tmp = (struct dvec3_t){v->x, v->y, v->z};
    v->x = w->x;
    v->y = w->y;
    v->z = w->z;
    w->x = tmp.x;
    w->y = tmp.y;
    w->z = tmp.z;
}

struct ivec3_t ivec2_to_ivec3(const struct ivec2_t v) {
    return (struct ivec3_t){v.x, v.y, 0};
}

struct dvec3_t dvec2_to_dvec3(const struct dvec2_t v) {
    return (struct dvec3_t){v.x, v.y, 0};
}

struct dvec3_t ivec3_to_dvec3(const struct ivec3_t v) {
    return (struct dvec3_t){v.x, v.y, v.z};
}

struct ivec3_t dvec3_to_ivec3(const struct dvec3_t v) {
    return (struct ivec3_t){v.x, v.y, v.z};
}

struct ivec3_t add_ivec3(const struct ivec3_t v, const struct ivec3_t w) {
    return (struct ivec3_t){v.x + w.x, v.y + w.y, v.z + w.z};
}

struct dvec3_t add_dvec3(const struct dvec3_t v, const struct dvec3_t w) {
    return (struct dvec3_t){v.x + w.x, v.y + w.y, v.z + w.z};
}

struct ivec2_t add_ivec2(const struct ivec2_t v, const struct ivec2_t w) {
    return (struct ivec2_t){v.x + w.x, v.y + w.y};
}

struct dvec2_t add_dvec2(const struct dvec2_t v, const struct dvec2_t w) {
    return (struct dvec2_t){v.x + w.x, v.y + w.y};
}

struct ivec3_t sub_ivec3(const struct ivec3_t v, const struct ivec3_t w) {
    return (struct ivec3_t){v.x - w.x, v.y - w.y, v.z - w.z};
}

struct dvec3_t sub_dvec3(const struct dvec3_t v, const struct dvec3_t w) {
    return (struct dvec3_t){v.x - w.x, v.y - w.y, v.z - w.z};
}

struct ivec2_t sub_ivec2(const struct ivec2_t v, const struct ivec2_t w) {
    return (struct ivec2_t){v.x - w.x, v.y - w.y};
}

struct dvec2_t sub_dvec2(const struct dvec2_t v, const struct dvec2_t w) {
    return (struct dvec2_t){v.x - w.x, v.y - w.y};
}

struct dvec2_t mul_dvec2_d(const struct dvec2_t v, double k) {
    return (struct dvec2_t){v.x * k, v.y * k};
}

struct dvec3_t mul_dvec3_d(const struct dvec3_t v, double k) {
    return (struct dvec3_t){v.x * k, v.y * k, v.z * k};
}

double dot_dvec3(const struct dvec3_t v, const struct dvec3_t w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

int dot_ivec3(const struct ivec3_t v, const struct ivec3_t w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

int dot_ivec2(const struct ivec2_t v, const struct ivec2_t w) {
    return v.x * w.x + v.y * w.y;
}

double dot_dvec2(const struct dvec2_t v, const struct dvec2_t w) {
    return v.x * w.x + v.y * w.y;
}
