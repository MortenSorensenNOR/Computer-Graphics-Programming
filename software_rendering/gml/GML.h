/* File: Graphics Math Library */

#pragma once
#include <stdio.h>
#include "math.h"

typedef struct {
    double x, y;
} vec2;

vec2 vec2_add(const vec2* v, const vec2* w);
vec2 vec2_sub(const vec2* v, const vec2* w);
vec2 vec2_mul(const vec2* v, double k);
vec2 vec2_div(const vec2* v, double k);
double vec2_dot(const vec2* v, const vec2* w);
double vec2_length_square(const vec2* v);
double vec2_length(const vec2* v);
vec2 vec2_normalize(const vec2* v);

typedef struct {
    double x, y, z;
} vec3;

vec3 vec3_add(const vec3* v, const vec3* w);
vec3 vec3_sub(const vec3* v, const vec3* w);
vec3 vec3_mul(const vec3* v, double k);
vec3 vec3_div(const vec3* v, double k);
vec3 vec3_cross(const vec3* v, const vec3* w);
double vec3_dot(const vec3* v, const vec3* w);
double vec3_length_square(const vec3* v);
double vec3_length(const vec3* v);
vec3 vec3_normalize(const vec3* v);
vec3 vec3_clamp(const vec3* v, float min, float max);

typedef struct {
    double x, y, z, w;
} vec4;

vec4 vec4_add(const vec4* v, const vec4* w);
vec4 vec4_sub(const vec4* v, const vec4* w);
vec4 vec4_mul(const vec4* v, double k);
vec4 vec4_div(const vec4* v, double k);
double vec4_dot(const vec4* v, const vec4* w);
double vec4_length_square(const vec4* v);
double vec4_length(const vec4* v);
vec4 vec4_normalize(const vec4* v);

void swap_vec2(vec2* v, vec2* w);
void swap_vec3(vec3* v, vec3* w);
void swap_vec4(vec4* v, vec4* w);

typedef struct {
    double m[9];
} mat3;

mat3 mat3_identity();
void mat3_mat3_add(const mat3*  a, const mat3*  b, mat3*  c);
void mat3_mat3_sub(const mat3*  a, const mat3*  b, mat3*  c);
void mat3_mat3_mul(const mat3*  a, const mat3*  b, mat3*  c);
mat3 mat3_mat3_add_ret(const mat3*  a, const mat3*  b);
mat3 mat3_mat3_sub_ret(const mat3*  a, const mat3*  b);
mat3 mat3_mat3_mul_ret(const mat3*  a, const mat3*  b);
vec3 mat3_vec3_mul(const mat3*  a, const vec3*  v);

typedef struct {
    double m[16];
} mat4;

mat4 mat4_identity();
void mat4_mat4_add(const mat4*  a, const mat4*  b, mat4*  c);
void mat4_mat4_sub(const mat4*  a, const mat4*  b, mat4*  c);
void mat4_mat4_mul(const mat4*  a, const mat4*  b, mat4*  c);
mat4 mat4_mat4_add_ret(const mat4*  a, const mat4*  b);
mat4 mat4_mat4_sub_ret(const mat4*  a, const mat4*  b);
mat4 mat4_mat4_mul_ret(const mat4*  a, const mat4*  b);
vec4 mat4_vec4_mul(const mat4*  a, const vec4*  v);

/*=============== GENERAL MATH ===============*/
double lerp(double v, double w, double t);
vec2 vec2_lerp(const vec2* v, const vec2* w, double t);
vec3 vec3_lerp(const vec3* v, const vec3* w, double t);
vec4 vec4_lerp(const vec4* v, const vec4* w, double t);

vec2 vec3_to_vec2(const vec3* v);
vec3 vec4_to_vec3(const vec4* v);
vec3 vec2_to_vec3(const vec2* v);
vec4 vec3_to_vec4(const vec3* v);

/*=============== GRAPHICS MATH ===============*/
mat4 mat4_translate(double tx, double ty, double tz);
mat4 mat4_rotate_x(float angle);
mat4 mat4_rotate_y(float angle);
mat4 mat4_rotate_z(float angle);
mat4 mat4_rotate(double pitch, double yaw, double roll); 
mat4 mat4_perspective(float fov, float aspect, float znear, float zfar);
vec3 perspective_divide(const vec4*  v);
vec3 viewport_transform(const vec3*  ndc, int width, int height, double z_near, double z_far);
vec3 transform_vertex(const vec4*  v, const mat4*  model, const mat4*  view, const mat4*  projection, int s_width, int s_height, double z_near, double z_far);
mat4 transformation_matrix(const mat4*  model, const mat4*  view, const mat4*  projection);
mat4 mat4_lookAt(vec3 eye, vec3 center, vec3 up);
