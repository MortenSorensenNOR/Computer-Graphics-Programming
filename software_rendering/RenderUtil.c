#include "RenderUtil.h"
#include <wchar.h>

/*=============== COLOR ===============*/
int color_to_int(const struct color_t c) {
    return c.r << 16 | c.g << 8 | c.b;
}

struct vec3 color_to_vec3(const struct color_t c) {
    return (struct vec3){c.r, c.g, c.b};
}

struct color_t color_interpolate(const struct color_t a, const struct color_t b, double t) {
    return (struct color_t){lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t)};
}

/*=============== MATH-STUFF ===============*/
struct vec3 get_barycentric_coordinate(const struct triangle_t* t, const struct vec3* p) {
    struct vec3 v0 = vec3_sub(&t->vert[1], &t->vert[0]);
    struct vec3 v1 = vec3_sub(&t->vert[2], &t->vert[0]);
    struct vec3 v2 = vec3_sub(p, &t->vert[0]);

    double d00 = vec3_dot(&v0, &v0);
    double d01 = vec3_dot(&v0, &v1);
    double d11 = vec3_dot(&v1, &v1);
    double d20 = vec3_dot(&v2, &v0);
    double d21 = vec3_dot(&v2, &v1);

    double det = (d00 * d11 - d01 * d01);
    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0f - v - w;
    return (struct vec3){u, v, w};
}

/*=============== RENDER-STUFF ===============*/
void bresenham(struct GAEDisplay* disp, int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1; 
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;

    for (;;) {
        GAEDisplay_setPixel(disp, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_triangle_wireframe(struct GAEDisplay* disp, struct triangle_t* tri, int color) {
    bresenham(disp, (int)tri->vert[0].x, (int)tri->vert[0].y, (int)tri->vert[1].x, (int)tri->vert[1].y, color);
    bresenham(disp, (int)tri->vert[1].x, (int)tri->vert[1].y, (int)tri->vert[2].x, (int)tri->vert[2].y, color);
    bresenham(disp, (int)tri->vert[2].x, (int)tri->vert[2].y, (int)tri->vert[0].x, (int)tri->vert[0].y, color);
}

void drawTriangle_filled(struct GAEDisplay* disp, struct triangle_t* tri) {
    struct vec3 p0 = tri->vert[0];
    struct vec3 p1 = tri->vert[1];
    struct vec3 p2 = tri->vert[2];

    if (p1.y < p0.y) swap_vec3(&p0, &p1);
    if (p2.y < p0.y) swap_vec3(&p2, &p0);
    if (p2.y < p1.y) swap_vec3(&p2, &p1);

    int total_height = (int)(p2.y - p0.y);
    for (int i = 0; i < total_height; i++) {
        if (p0.y + i < 0 || p0.y + i >= disp->height) continue;
        int second_half = i > p1.y - p0.y || p1.y == p0.y;
        int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height;

        struct vec2 A, B;
        A = (struct vec2){(int)(p0.x + (p2.x - p0.x) * alpha), (int)(p0.y + i)};
        
        if (second_half) {
            B = (struct vec2){(int)(p1.x + (p2.x - p1.x) * beta), (int)(p1.y + (i - (p1.y - p0.y)))};
        } else {
            B = (struct vec2){(int)(p0.x + (p1.x - p0.x) * beta), (int)( p0.y + i)};
        }

        if ((int)A.x > (int)B.x) {
            int t = A.x;
            A.x = B.x;
            B.x = t;
        };

        for (int x = (int)A.x; x < (int)B.x; x++) {
            if (x < 0 || x >=disp->width) continue;
            struct vec3 p = (struct vec3){x, (int)p0.y + i, 0};
            struct vec3 coords = get_barycentric_coordinate(tri, &p);
            struct color_t color = (struct color_t){
                coords.x * tri->color[0].r + coords.y * tri->color[1].r + coords.z * tri->color[2].r,
                coords.x * tri->color[0].g + coords.y * tri->color[1].g + coords.z * tri->color[2].g,
                coords.x * tri->color[0].b + coords.y * tri->color[1].b + coords.z * tri->color[2].b,
            };
            int color_int = color_to_int(color);

            GAEDisplay_setPixel(disp, p.x, p.y, color_int);
        }
    }
}
