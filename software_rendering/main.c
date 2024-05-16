#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/GAEDisplay.h"
#include "RenderUtil.h"

int main(int argc, char** argv) {
    srand(time(0));

    GAEDisplay_t disp;
    GAEDisplay_init(&disp);
    GAEDisplay_clear(&disp, 0x1C1D1E);

    triangle_t tri;
    tri.vert[0] = (vec4){0.0f, 0.5f, 0.0f, 1.0f};
    tri.vert[1] = (vec4){-0.5f, -0.5f, 0.0f, 1.0f};
    tri.vert[2] = (vec4){0.5f, -0.5f, 0.0f, 1.0f};
    tri.uv[0] = (vec2){0, 0};
    tri.uv[1] = (vec2){1, 0};
    tri.uv[2] = (vec2){0.5, 1};
    tri.color[0] = (color_t){125, 0, 255};
    tri.color[1] = (color_t){255, 60, 0};
    tri.color[2] = (color_t){0, 255, 40};

    mat4 model = mat4_translate(0, 0, 5);
    mat4 rotation = mat4_rotate(M_PI/4, M_PI/4, M_PI/4);
    mat4 model_rotated = mat4_mat4_mul_ret(&model, &rotation);

    vec3 up = { 0.0f, 1.0f, 0.0f };
    vec3 eye = { 0.0f, 0.0f, 10.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    mat4 view = mat4_lookAt(eye, center, up);

    float fov = M_PI / 3;
    float aspect = (double)disp.width / (double)disp.height;
    float znear = 0.1f;
    float zfar = 100.0f;
    mat4 projection = mat4_perspective(fov, aspect, znear, zfar);

    clock_t start, end;
    while (1) {
        start = clock();
        GAEDisplay_clear(&disp, 0x1C1D1E);

        vec2 tri_vert_trans[3];
        transform_triangle(tri.vert, tri_vert_trans, &model_rotated, &view, &projection, disp.width, disp.height);
        draw_triangle_filled(&disp, tri_vert_trans, tri.uv, tri.color);

        GAEDisplay_update(&disp);
        end = clock();
        printf("FPS: %f\n", 1/((double)(end - start)/CLOCKS_PER_SEC));
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
