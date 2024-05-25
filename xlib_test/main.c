#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GAEDisplay.h"

struct vec3_t {
    int x, y, z;
};

struct triangle_t {
    struct vec3_t vert[3];
};

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

void drawHorizontalLine(struct GAEDisplay* disp, int x0, int x1, int y, int color) {
    for (int x = x0; x < x1; x++) {
        GAEDisplay_setPixel(disp, x, y, color);
    }
}

void vec3_swap(struct vec3_t* v, struct vec3_t* w) {
    struct vec3_t tmp = (struct vec3_t){v->x, v->y, v->z};
    v->x = w->x;
    v->y = w->y;
    v->z = w->z;
    w->x = tmp.x;
    w->y = tmp.y;
    w->z = tmp.z;
}

void drawTriangle_wireframe(struct GAEDisplay* disp, struct triangle_t* tri, int color) {
    bresenham(disp, (int)tri->vert[0].x, (int)tri->vert[0].y, (int)tri->vert[1].x, (int)tri->vert[1].y, color);
    bresenham(disp, (int)tri->vert[1].x, (int)tri->vert[1].y, (int)tri->vert[2].x, (int)tri->vert[2].y, color);
    bresenham(disp, (int)tri->vert[2].x, (int)tri->vert[2].y, (int)tri->vert[0].x, (int)tri->vert[0].y, color);
}

void drawTriangle_filled(struct GAEDisplay* disp, struct triangle_t* tri, int color) {
    struct vec3_t p0 = tri->vert[0];
    struct vec3_t p1 = tri->vert[1];
    struct vec3_t p2 = tri->vert[2];

    if (p1.y < p0.y) vec3_swap(&p0, &p1);
    if (p2.y < p0.y) vec3_swap(&p2, &p0);
    if (p2.y < p1.y) vec3_swap(&p2, &p1);

    int total_height = (int)(p2.y - p0.y);
    for (int i = 0; i < total_height; i++) {
        int second_half = i > p1.y - p0.y || p1.y == p0.y;
        int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height;

        struct vec3_t A, B;
        A = (struct vec3_t){p0.x + (p2.x - p0.x) * alpha, p0.y + i, 1};
        
        if (second_half) {
            B = (struct vec3_t){p1.x + (p2.x - p1.x) * beta, p1.y + (i - (p1.y - p0.y))};
        } else {
            B = (struct vec3_t){p0.x + (p1.x - p0.x) * beta, p0.y + i};
        }

        if (A.x > B.x) {
            int t = A.x;
            A.x = B.x;
            B.x = t;
        };

        drawHorizontalLine(disp, A.x, B.x, p0.y + i, color);
    }

}

int main(int argc, char** argv) {
    srand(time(0));

    struct GAEDisplay disp;
    GAEDisplay_init(&disp);
    GAEDisplay_clear(&disp, 0xFFFFFFFF);

    struct triangle_t tri;
    struct vec3_t c = {disp.width / 4, disp.height / 4, 1};
    tri.vert[0] = (struct vec3_t){disp.width / 2, disp.height / 4, 1};
    tri.vert[1] = (struct vec3_t){3 * disp.width / 4, 3 * disp.height / 4, 1};
    tri.vert[2] = (struct vec3_t){disp.width / 4, 3 * disp.height / 4, 1};

    drawTriangle_filled(&disp, &tri, 0x00FF00);
    drawTriangle_wireframe(&disp, &tri, 0);

    while (1) {
        GAEDisplay_update(&disp);
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
