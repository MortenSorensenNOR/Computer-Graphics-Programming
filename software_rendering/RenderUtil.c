#include "RenderUtil.h"

/*=============== COLOR ===============*/
int color_to_int(const color_t c) {
    return c.r << 16 | c.g << 8 | c.b;
}

vec3 color_to_vec3(const color_t c) {
    return (vec3){c.r, c.g, c.b};
}

color_t color_interpolate(const color_t a, const color_t b, double t) {
    return (color_t){lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t)};
}

/*=============== MATH-STUFF ===============*/
vec3 get_barycentric_coordinate(const vec2 vert[3], const vec2* p) {
    vec2 v0 = vec2_sub(&vert[1], &vert[0]);
    vec2 v1 = vec2_sub(&vert[2], &vert[0]);
    vec2 v2 = vec2_sub(p, &vert[0]);

    double d00 = vec2_dot(&v0, &v0);
    double d01 = vec2_dot(&v0, &v1);
    double d11 = vec2_dot(&v1, &v1);
    double d20 = vec2_dot(&v2, &v0);
    double d21 = vec2_dot(&v2, &v1);

    double det = (d00 * d11 - d01 * d01);
    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0f - v - w;
    return (vec3){u, v, w};
}

void transform_triangle(const vec4 vert[3], vec2 transformed_vert[3], const mat4* model, const mat4* view, const mat4* projection, int s_width, int s_height) {
    for (int i = 0; i < 3; ++i) {
        transformed_vert[i] = transform_vertex(&vert[i], model, view, projection, s_width, s_height);
    }
}

/*=============== RENDER-STUFF ===============*/
void bresenham(GAEDisplay_t* disp, int x0, int y0, int x1, int y1, int color) {
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

void draw_triangle_wireframe(GAEDisplay_t* disp, const vec2 vert[3], int color) {
    bresenham(disp, (int)vert[0].x, vert[0].y, (int)vert[1].x, (int)vert[1].y, color);
    bresenham(disp, (int)vert[1].x, vert[1].y, (int)vert[2].x, (int)vert[2].y, color);
    bresenham(disp, (int)vert[2].x, vert[2].y, (int)vert[0].x, (int)vert[0].y, color);
}

void draw_triangle_filled(GAEDisplay_t* disp, const vec2 vert[3], const vec2 uv[3], const color_t vert_color[3]) {
    vec2 p0 = vert[0];
    vec2 p1 = vert[1];
    vec2 p2 = vert[2];

    if (p1.y < p0.y) swap_vec2(&p0, &p1);
    if (p2.y < p0.y) swap_vec2(&p2, &p0);
    if (p2.y < p1.y) swap_vec2(&p2, &p1);

    int total_height = (int)(p2.y - p0.y);
    for (int i = 0; i < total_height; i++) {
        if (p0.y + i < 0 || p0.y + i >= disp->height) continue;
        int second_half = i > p1.y - p0.y || p1.y == p0.y;
        int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height;

        vec2 A, B;
        A = (vec2){(int)(p0.x + (p2.x - p0.x) * alpha), (int)(p0.y + i)};
        
        if (second_half) {
            B = (vec2){(int)(p1.x + (p2.x - p1.x) * beta), (int)(p1.y + (i - (p1.y - p0.y)))};
        } else {
            B = (vec2){(int)(p0.x + (p1.x - p0.x) * beta), (int)( p0.y + i)};
        }

        if ((int)A.x > (int)B.x) {
            int t = A.x;
            A.x = B.x;
            B.x = t;
        };

        if ((int)A.x < 0) A.x = 0;
        if ((int)B.x > disp->width) B.x = disp->width;
        for (int x = (int)A.x; x < (int)B.x; x++) {
            if (x < 0 || x >=disp->width) continue;
            vec2 p = (vec2){x, (int)p0.y + i};
            vec3 coords = get_barycentric_coordinate(vert, &p);
            color_t color = (color_t){
                coords.x * vert_color[0].r + coords.y * vert_color[1].r + coords.z * vert_color[2].r,
                coords.x * vert_color[0].g + coords.y * vert_color[1].g + coords.z * vert_color[2].g,
                coords.x * vert_color[0].b + coords.y * vert_color[1].b + coords.z * vert_color[2].b,
            };
            int color_int = color_to_int(color);

            GAEDisplay_setPixel(disp, p.x, p.y, color_int);
        }
    }
}
