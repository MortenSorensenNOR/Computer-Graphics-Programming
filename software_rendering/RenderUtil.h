#pragma once
#include "display/GAEDisplay.h"
#include "gml/GML.h"
#include <wctype.h>

struct color_t {
    int r, g, b;   
};

struct uv_coord_t {
    double u, v;
};

struct triangle_t {
    struct dvec3_t vert[3];
    struct uv_coord_t uv[3];
    struct color_t color[3];
};

int color_to_int(const struct color_t c);
struct ivec3_t color_to_vec3(const struct color_t c);
struct dvec2_t uv_coord_to_vec2(const struct uv_coord_t c);

struct color_t color_interpolate(const struct color_t a, const struct color_t b, double t);
struct uv_coord_t uv_coord_interpolate(const struct uv_coord_t a, const struct uv_coord_t b, double t);

struct dvec3_t get_barycentric_coordinate(const struct triangle_t* t, const struct dvec3_t* p);

void bresenham(struct GAEDisplay* disp, int x0, int y0, int x1, int y1, int color);
void draw_triangle_wireframe(struct GAEDisplay* disp, struct triangle_t* tri, int color);

// TODO: FIX THESE AND CLEAN UP

struct dvec3_t barycentricCoordinate_ivec3(struct ivec3_t p, struct ivec3_t p0, struct ivec3_t p1, struct ivec3_t p2);
struct dvec3_t barycentricCoordinate_dvec2(struct dvec2_t p, struct dvec2_t p0, struct dvec2_t p1, struct dvec2_t p2);

struct uv_coord_t uv_interpolate(struct ivec3_t p, struct triangle_t* tri);

void drawHorizontalLine(struct GAEDisplay* disp, int x0, int x1, int y, int color);

void drawTriangle_filled(struct GAEDisplay* disp, struct triangle_t* tri, int color);
