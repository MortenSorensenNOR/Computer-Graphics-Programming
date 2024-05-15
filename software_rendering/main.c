#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/GAEDisplay.h"
#include "RenderUtil.h"

int main(int argc, char** argv) {
    srand(time(0));

    struct GAEDisplay disp;
    GAEDisplay_init(&disp);
    GAEDisplay_clear(&disp, 0x1C1D1E);

    struct vec3 a, b, c;
    a = (struct vec3){(double)disp.width / 2, (double)disp.height / 4, 1};
    b = (struct vec3){3 * (double)disp.width / 4, 3 * (double)disp.height / 4, 1};
    c = (struct vec3){(double)disp.width / 4, 3 * (double)disp.height / 4, 1};

    struct triangle_t tri;
    tri.vert[0] = a;
    tri.vert[1] = b;
    tri.vert[2] = c;
    tri.uv[0] = (struct vec2){0, 0};
    tri.uv[1] = (struct vec2){1, 0};
    tri.uv[2] = (struct vec2){0.5, 1};
    tri.color[0] = (struct color_t){0, 0, 255};
    tri.color[1] = (struct color_t){255, 0, 0};
    tri.color[2] = (struct color_t){0, 255, 0};

    int h = 0;
    int dir = 1;
    while (1) {
        h += dir;
        if (h > 250 || h < 0) dir = -dir;
        tri.vert[0].y = a.y + h;
        tri.vert[1].y = b.y + h;
        tri.vert[2].y = c.y + h;

        GAEDisplay_clear(&disp, 0x1C1D1E);
        drawTriangle_filled(&disp, &tri);
        draw_triangle_wireframe(&disp, &tri, 0);
        GAEDisplay_update(&disp);
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
