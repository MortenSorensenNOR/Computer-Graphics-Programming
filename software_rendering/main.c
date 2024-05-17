#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/GAEDisplay.h"
#include "RenderUtil.h"

vec4 vert_buf[] = {
    {-0.5f, -0.5f, -0.5f, 1.0f}, { 0.5f, -0.5f, -0.5f, 1.0f}, { 0.5f,  0.5f, -0.5f, 1.0f},
    { 0.5f,  0.5f, -0.5f, 1.0f}, {-0.5f,  0.5f, -0.5f, 1.0f}, {-0.5f, -0.5f, -0.5f, 1.0f},
    {-0.5f, -0.5f,  0.5f, 1.0f}, { 0.5f, -0.5f,  0.5f, 1.0f}, { 0.5f,  0.5f,  0.5f, 1.0f},
    { 0.5f,  0.5f,  0.5f, 1.0f}, {-0.5f,  0.5f,  0.5f, 1.0f}, {-0.5f, -0.5f,  0.5f, 1.0f},
    {-0.5f,  0.5f,  0.5f, 1.0f}, {-0.5f,  0.5f, -0.5f, 1.0f}, {-0.5f, -0.5f, -0.5f, 1.0f},
    {-0.5f, -0.5f, -0.5f, 1.0f}, {-0.5f, -0.5f,  0.5f, 1.0f}, {-0.5f,  0.5f,  0.5f, 1.0f},
    { 0.5f,  0.5f,  0.5f, 1.0f}, { 0.5f,  0.5f, -0.5f, 1.0f}, { 0.5f, -0.5f, -0.5f, 1.0f},
    { 0.5f, -0.5f, -0.5f, 1.0f}, { 0.5f, -0.5f,  0.5f, 1.0f}, { 0.5f,  0.5f,  0.5f, 1.0f},
    {-0.5f, -0.5f, -0.5f, 1.0f}, { 0.5f, -0.5f, -0.5f, 1.0f}, { 0.5f, -0.5f,  0.5f, 1.0f},
    { 0.5f, -0.5f,  0.5f, 1.0f}, {-0.5f, -0.5f,  0.5f, 1.0f}, {-0.5f, -0.5f, -0.5f, 1.0f},
    {-0.5f,  0.5f, -0.5f, 1.0f}, { 0.5f,  0.5f, -0.5f, 1.0f}, { 0.5f,  0.5f,  0.5f, 1.0f},
    { 0.5f,  0.5f,  0.5f, 1.0f}, {-0.5f,  0.5f,  0.5f, 1.0f}, {-0.5f,  0.5f, -0.5f, 1.0f},
};
vec3 norm_buf[] = {
    {-0.5f,  0.0f,  0.0f}, {-0.5f,  0.0f,  0.0f}, {-0.5f,  0.0f,  0.0f},
    {-0.5f,  0.0f,  0.0f}, {-0.5f,  0.0f,  0.0f}, {-0.5f,  0.0f,  0.0f},
    { 0.5f,  0.0f,  0.0f}, { 0.5f,  0.0f,  0.0f}, { 0.5f,  0.0f,  0.0f},
    { 0.5f,  0.0f,  0.0f}, { 0.5f,  0.0f,  0.0f}, { 0.5f,  0.0f,  0.0f},
    { 0.5f, -1.0f,  0.0f}, {-0.5f, -1.0f,  0.0f}, {-0.5f, -1.0f,  0.0f},
    {-0.5f, -1.0f,  0.0f}, { 0.5f, -1.0f,  0.0f}, { 0.5f, -1.0f,  0.0f},
    { 0.5f,  1.0f,  0.0f}, {-0.5f,  1.0f,  0.0f}, {-0.5f,  1.0f,  0.0f},
    {-0.5f,  1.0f,  0.0f}, { 0.5f,  1.0f,  0.0f}, { 0.5f,  1.0f,  0.0f},
    {-0.5f,  0.0f, -1.0f}, {-0.5f,  0.0f, -1.0f}, { 0.5f,  0.0f, -1.0f},
    { 0.5f,  0.0f, -1.0f}, { 0.5f,  0.0f, -1.0f}, {-0.5f,  0.0f, -1.0f},
    {-0.5f,  0.0f,  1.0f}, {-0.5f,  0.0f,  1.0f}, { 0.5f,  0.0f,  1.0f},
    { 0.5f,  0.0f,  1.0f}, { 0.5f,  0.0f,  1.0f}, {-0.5f,  0.0f,  1.0f},
};

vec2 uv_buf[] = {
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
    {0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, 
};

color_t color_buf[] = {
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
    {125, 0, 255}, {255, 60, 0}, {0, 255, 40},
};

int main(int argc, char** argv) {
    srand(time(0));

    GAEDisplay_t disp;
    GAEDisplay_init(&disp);
    GAEDisplay_clear(&disp, 0x1C1D1E);

    mesh_t mesh;
    mesh.vert = vert_buf;
    mesh.norm = norm_buf;
    mesh.uv = uv_buf;
    mesh.colors = color_buf;

    light_t light;
    light.pos = (vec3){1.2, 1.0, 2.0};
    light.ambient = (vec3){0.25, 0.25, 0.25};
    light.diffuse = (vec3){5, 0.5, 0.5};

    mat4 model = mat4_translate(0, 0, 50);

    vec3 up = { 0.0f, 1.0f, 0.0f };
    vec3 eye = { 0.0f, 0.0f, 10.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    mat4 view = mat4_lookAt(eye, center, up);

    float fov = M_PI / 4;
    float aspect = (double)disp.width / (double)disp.height;
    float znear = 0.1f;
    float zfar = 100.0f;
    mat4 projection = mat4_perspective(fov, aspect, znear, zfar);

    double angle = 0;
    clock_t start, end;
    while (1) {
        start = clock();
        GAEDisplay_clear(&disp, 0x1C1D1E);

        angle = (angle + 0.01) - (int)(angle / (2 * M_PI)) * 2 * M_PI;
        mat4 rotation = mat4_rotate(angle, angle, 0);
        mat4 model_rotated = mat4_mat4_mul_ret(&model, &rotation);

        for (int i = 0; i < 6 * 2; i++) {
            vec3 tri_norm[3] = {
                mesh.norm[i * 3],
                mesh.norm[i * 3 + 1],
                mesh.norm[i * 3 + 2],
            };
            vec2 tri_uv[3] = {
                mesh.uv[i * 3],
                mesh.uv[i * 3 + 1],
                mesh.uv[i * 3 + 2],
            };
            color_t tri_col[3] = {
                mesh.colors[i * 3],
                mesh.colors[i * 3 + 1],
                mesh.colors[i * 3 + 2],
            };
            vec4 tri_vert[3] = {
                mesh.vert[i * 3],
                mesh.vert[i * 3 + 1],
                mesh.vert[i * 3 + 2],
            };
            vec3 tri_vert_trans[3];
            transform_triangle(tri_vert, tri_vert_trans, &model_rotated, &view, &projection, disp.width, disp.height, znear, zfar);
            draw_triangle_filled2(&disp, tri_vert_trans, tri_uv, tri_col, tri_norm, &light);
        }

        GAEDisplay_update(&disp);
        end = clock();
        printf("FPS: %f\n", 1/((double)(end - start)/CLOCKS_PER_SEC));
    }

    GAEDisplay_destroy(&disp);

    free(mesh.prim);
    free(mesh.vert);
    free(mesh.uv);
    free(mesh.colors);

    return 0;
}
