#pragma once
#include "display/GAEDisplay.h"
#include "gml/GML.h"

struct color_t {
    int r, g, b;   
};

struct triangle_t {
    struct vec3 vert[3];
    struct vec2 uv[3];
    struct color_t color[3];
};

struct mesh_t {
    struct vec4 pos;
    int* prim;
    struct vec3* vert;
    struct vec2* uv;
    struct color_t* colors;
};

int color_to_int(const struct color_t c);
struct vec3 color_to_vec3(const struct color_t c);
struct color_t color_interpolate(const struct color_t a, const struct color_t b, double t);

struct vec3 get_barycentric_coordinate(const struct triangle_t* t, const struct vec3* p);
struct vec2 get_uv_coord(const struct triangle_t* t, const struct vec3* p);

void bresenham(struct GAEDisplay* disp, int x0, int y0, int x1, int y1, int color);
void draw_triangle_wireframe(struct GAEDisplay* disp, struct triangle_t* tri, int color);

void drawTriangle_filled(struct GAEDisplay* disp, struct triangle_t* tri);
