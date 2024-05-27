#pragma once
#include "../display/Display.h"
#include "../gml/GML.h"

typedef struct {
    int r, g, b;   
} color_t;

typedef struct {
    vec4 vert[3];
    vec2 uv[3];
    color_t color[3];
} triangle_t;

typedef struct {
    vec4 pos;

    int* prim;
    vec4* vert;
    vec3* norm;
    vec2* uv;
    color_t* colors;

    int vert_buffer_size;
    int prim_buffer_size;
    int uv_buffer_size;
    int color_buffer_size;
} mesh_t;

typedef struct {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light_t;

int color_to_int(const color_t c);
vec3 color_to_vec3(const color_t c);

vec3 get_barycentric_coordinate2d(const vec2 vert[3], const vec2* p);
vec3 get_barycentric_coordinate3d(const vec3 vert[3], const vec3* p);
