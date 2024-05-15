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
    GAEDisplay_clear(&disp, 0xFFFFFFFF);

    struct triangle_t tri;
    tri.vert[0] = (struct dvec3_t){(double)disp.width / 2, (double)disp.height / 4, 1};
    tri.vert[1] = (struct dvec3_t){3 * (double)disp.width / 4, 3 * (double)disp.height / 4, 1};
    tri.vert[2] = (struct dvec3_t){(double)disp.width / 4, 3 * (double)disp.height / 4, 1};
    tri.uv[0] = (struct uv_coord_t){0, 0};
    tri.uv[1] = (struct uv_coord_t){1, 0};
    tri.uv[2] = (struct uv_coord_t){0.5, 1};
    tri.color[0] = (struct color_t){255, 0, 0};
    tri.color[1] = (struct color_t){0, 255, 0};
    tri.color[2] = (struct color_t){0, 0, 255};

    drawTriangle_filled(&disp, &tri, 0x00FF00);
    draw_triangle_wireframe(&disp, &tri, 0);

    while (1) {
        GAEDisplay_update(&disp);
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
