#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"
#include "rutil/ModelLoader.h"

#include "render/renderer.h"

float fov = M_PI / 4;
float znear = 0.1f;
float zfar = 100.0f;


int main(int argc, char** argv) {
    srand(time(0));

    // Initialize display and user input
    Display_t disp;
    Input_t input;
    Display_init(&disp);
    Input_init(&input, &disp);
    Display_clear(&disp, 0x1C1D1E);

    // Load assets
    texture_t test;
    load_texture("../resources/chad.jpg", &test.diffuse, &test.diffuse_width, &test.diffuse_height);
    load_texture("../resources/birk.jpg", &test.specular, &test.specular_width, &test.specular_height);

    light_t light;
    render_object_t object;
    int object_load_success = parse_obj("../resources/cube.obj", &object);

    if (object_load_success) {
        return 1;
    }

    printf("Num meshes: %d\n", object.num_meshes);
    for (int i = 0; i < object.num_meshes; i++) {
        printf("Mesh %d: \t Num vertexes: %d\n", i, object.meshes[i].vbuff_size);
        printf("Indicies %d: \n", object.meshes[i].ibuff_size);

        for (int j = 0; j < object.meshes[i].ibuff_size; j++) {
            printf("%d\n", object.meshes[i].index[j]);
        }

        /* for (int i = 0; i < object.meshes[i].ibuff_size; i+=3) { */
        /*     printf("%d / %d / %d\n", object.meshes[i].index[i], object.meshes[i].index[i+1], object.meshes[i].index[i+2]); */
        /* } */
    }

    render_t renderer;
    renderer_init(disp.width, disp.height, &renderer);

    vec3 up = { 0.0f, 1.0f, 0.0f };
    vec3 eye = { 0.0f, 0.0f, 10.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };

    // Main render/input loop
    clock_t start, end;
    while (1) {
        start = clock();

        Input_update(&input);
        if (Input_getKeyState(&input, 'w')) 
            break;

        float aspect = (double)disp.width / (double)disp.height;

        mat4 view = mat4_lookAt(eye, center, up);
        mat4 projection = mat4_perspective(fov, aspect, znear, zfar);
        mat4 view_proj = mat4_mat4_mul_ret(&projection, &view);

        mat4 translation_matrix = mat4_translate(0, 0, 10);
        mat4 rotation_matrix = mat4_rotate(0, M_PI / 4, 0);
        mat4 model = mat4_mat4_mul_ret(&translation_matrix, &rotation_matrix);

        renderer_render(&renderer, &view_proj, &model, &object, &light);
        break;

        Display_clear(&disp, 0x1C1D1E);
        for (int x = 0; x < test.diffuse_width; ++x) {
            for (int y = 0; y < test.diffuse_height; ++y) {
                int color = color_v(&test.diffuse[x + y * test.diffuse_width]);
                Display_setPixel(&disp, x, y, color);
            }
        }
    
        Display_update(&disp);
        end = clock();
    }

    // Free assets
    free(test.diffuse);
    free_render_object(&object);
    renderer_destroy(&renderer);
    
    // Free display
    Display_destroy(&disp);

    return 0;
}
