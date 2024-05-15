#include "RenderUtil.h"

int color_to_int(const struct color_t c) {
    return c.r << 16 | c.g << 8 | c.b;
}

struct ivec3_t color_to_vec3(const struct color_t c) {
    return (struct ivec3_t){c.r, c.g, c.b};
}

struct dvec2_t uv_coord_to_vec2(const struct uv_coord_t c) {
    return (struct dvec2_t){c.u, c.v};
}

struct color_t color_interpolate(const struct color_t a, const struct color_t b, double t) {
    return (struct color_t){lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t)};
}

struct uv_coord_t uv_coord_interpolate(const struct uv_coord_t a, const struct uv_coord_t b, double t) {
    return (struct uv_coord_t){lerp(a.u, b.u, t), lerp(a.v, b.v, t)};    
}

struct dvec3_t get_barycentric_coordinate(const struct triangle_t* t, const struct dvec3_t* p) {
    struct dvec3_t v0 = sub_dvec3(t->vert[1], t->vert[0]);
    struct dvec3_t v1 = sub_dvec3(t->vert[2], t->vert[0]);
    struct dvec3_t v2 = sub_dvec3(*p, t->vert[0]);

    double d00 = dot_dvec3(v0, v0);
    double d01 = dot_dvec3(v0, v1);
    double d11 = dot_dvec3(v1, v1);
    double d20 = dot_dvec3(v2, v0);
    double d21 = dot_dvec3(v2, v1);

    double det = (d00 * d11 - d01 * d01);
    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0f - v - w;
    return (struct dvec3_t){u, v, w};
}

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

// TODO: FIX THESE AND CLEAN UP

struct dvec3_t barycentricCoordinate_ivec3(struct ivec3_t p, struct ivec3_t p0, struct ivec3_t p1, struct ivec3_t p2) {
    struct ivec3_t v0 = sub_ivec3(p1, p0);
    struct ivec3_t v1 = sub_ivec3(p2, p0);
    struct ivec3_t v2 = sub_ivec3(p, p0);

    int d00 = dot_ivec3(v0, v0);
    int d01 = dot_ivec3(v0, v1);
    int d11 = dot_ivec3(v1, v1);
    int d20 = dot_ivec3(v2, v0);
    int d21 = dot_ivec3(v2, v1);
    int denom = d00 * d11 - d01 * d01;

    struct dvec3_t res;
    res.y = (double)(d11 * d20 - d01 * d21) / denom;
    res.z = (double)(d00 * d21 - d01 * d20) / denom;
    res.x = 1.0 - res.y - res.z;
    return res;
}

struct dvec3_t barycentricCoordinate_dvec2(struct dvec2_t p, struct dvec2_t p0, struct dvec2_t p1, struct dvec2_t p2) {
    struct dvec2_t v0 = sub_dvec2(p1, p0);
    struct dvec2_t v1 = sub_dvec2(p2, p0);
    struct dvec2_t v2 = sub_dvec2(p, p0);

    int d00 = dot_dvec2(v0, v0);
    int d01 = dot_dvec2(v0, v1);
    int d11 = dot_dvec2(v1, v1);
    int d20 = dot_dvec2(v2, v0);
    int d21 = dot_dvec2(v2, v1);
    int denom = d00 * d11 - d01 * d01;

    struct dvec3_t res;
    res.y = (double)(d11 * d20 - d01 * d21) / denom;
    res.z = (double)(d00 * d21 - d01 * d20) / denom;
    res.x = 1.0 - res.y - res.z;
    return res;
}

struct uv_coord_t uv_interpolate(struct ivec3_t p, struct triangle_t* tri) {
    struct dvec2_t tUV0 = (struct dvec2_t){tri->uv[0].u, tri->uv[0].v};
    struct dvec2_t tUV1 = (struct dvec2_t){tri->uv[1].u, tri->uv[1].v};
    struct dvec2_t tUV2 = (struct dvec2_t){tri->uv[2].u, tri->uv[2].v};

    struct dvec3_t bPos = barycentricCoordinate_ivec3(p, dvec3_to_ivec3(tri->vert[0]), dvec3_to_ivec3(tri->vert[1]), dvec3_to_ivec3(tri->vert[2]));
    struct dvec2_t bUV = add_dvec2(add_dvec2(mul_dvec2_d(tUV0, bPos.x), mul_dvec2_d(tUV1, bPos.y)), mul_dvec2_d(tUV2, bPos.z));

    return (struct uv_coord_t){bUV.x, bUV.y};
}


void drawHorizontalLine(struct GAEDisplay* disp, int x0, int x1, int y, int color) {
    for (int x = x0; x < x1; x++) {
        GAEDisplay_setPixel(disp, x, y, color);
    }
}


void drawTriangle_filled(struct GAEDisplay* disp, struct triangle_t* tri, int color) {
    struct dvec2_t tUV0 = (struct dvec2_t){tri->uv[0].u, tri->uv[0].v};
    struct dvec2_t tUV1 = (struct dvec2_t){tri->uv[1].u, tri->uv[1].v};
    struct dvec2_t tUV2 = (struct dvec2_t){tri->uv[2].u, tri->uv[2].v};

    struct dvec3_t C0 = (struct dvec3_t){tri->color[0].r, tri->color[0].g, tri->color[0].b};
    struct dvec3_t C1 = (struct dvec3_t){tri->color[1].r, tri->color[1].g, tri->color[1].b};
    struct dvec3_t C2 = (struct dvec3_t){tri->color[2].r, tri->color[2].g, tri->color[2].b};

    struct ivec3_t p0 = dvec3_to_ivec3(tri->vert[0]);
    struct ivec3_t p1 = dvec3_to_ivec3(tri->vert[1]);
    struct ivec3_t p2 = dvec3_to_ivec3(tri->vert[2]);

    if (p1.y < p0.y) swap_ivec3(&p0, &p1);
    if (p2.y < p0.y) swap_ivec3(&p2, &p0);
    if (p2.y < p1.y) swap_ivec3(&p2, &p1);

    int total_height = (int)(p2.y - p0.y);
    for (int i = 0; i < total_height; i++) {
        int second_half = i > p1.y - p0.y || p1.y == p0.y;
        int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height;

        struct ivec3_t A, B;
        A = (struct ivec3_t){p0.x + (p2.x - p0.x) * alpha, p0.y + i, 1};
        
        if (second_half) {
            B = (struct ivec3_t){p1.x + (p2.x - p1.x) * beta, p1.y + (i - (p1.y - p0.y))};
        } else {
            B = (struct ivec3_t){p0.x + (p1.x - p0.x) * beta, p0.y + i};
        }

        if (A.x > B.x) {
            int t = A.x;
            A.x = B.x;
            B.x = t;
        };

        for (int x = A.x; x < B.x; x++) {
            struct ivec3_t p;
            p.x = x;
            p.y = p0.y + i;
            struct uv_coord_t uv = uv_interpolate(p, tri);
            struct dvec2_t uv_vec = {uv.u, uv.v};
            struct dvec3_t bUV = barycentricCoordinate_dvec2(uv_vec, tUV0, tUV1, tUV2);
            struct dvec3_t c_vec = add_dvec3(add_dvec3(mul_dvec3_d(C0, bUV.x), mul_dvec3_d(C1, bUV.y)), mul_dvec3_d(C2, bUV.z));
            struct color_t c = (struct color_t){c_vec.x, c_vec.y, c_vec.z};
            int c_val = color_to_int(c);

            GAEDisplay_setPixel(disp, p.x, p.y, c_val);
        }
    }
}
