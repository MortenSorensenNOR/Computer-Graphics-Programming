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
    load_texture("../resources/backpack/diffuse_new.jpg", &test.diffuse, &test.diffuse_width, &test.diffuse_height);
    
    light_t light;
    render_object_t object;
    int object_load_success = parse_obj("../resources/backpack/backpack.obj", &object);
    if (object_load_success) {
        return 1;
    }
    object.textures = &test;
    
    render_t renderer;
    renderer_init(&renderer, disp.width, disp.height, zfar, znear);
    
    // Matricies and stuff
    vec3 up = { 0.0f, 1.0f, 0.0f };
    vec3 eye = { 0.0f, 0.0f, 10.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    float aspect = (double)disp.width / (double)disp.height;

    mat4 view, projection, view_proj;
    mat4 rotation_matrix, model;
    
    float scale = 0.01f;
    mat4 scale_matrix = mat4_scale(scale, scale, scale);
    mat4 translation_matrix = mat4_translate(0, 0, 15);

    // Main render/input loop
    float angle = 0;
    float view_anlge = 0;
    clock_t start, end;
    while (1) {
        start = clock();
        angle += M_PI * 0.005;
    
        Input_update(&input);
        if (Input_getKeyState(&input, 'x')) 
            break;
        if (Input_getKeyState(&input, 'w'))
            center.z += 0.05;
        if (Input_getKeyState(&input, 's'))
            center.z -= 0.05;
        if (Input_getKeyState(&input, 'u'))
            view_anlge += M_PI * 0.005;
        if (Input_getKeyState(&input, 'n'))
            view_anlge -= M_PI * 0.005;

        // Camera
        view = mat4_lookAt(eye, center, up);
        projection = mat4_perspective(fov, aspect, znear, zfar);
        view_proj = mat4_mat4_mul_ret(&projection, &view);
    
        // Model
        rotation_matrix = mat4_rotate(0, angle, 0);
        model = mat4_mat4_mul_ret(&rotation_matrix, &scale_matrix);
        model = mat4_mat4_mul_ret(&translation_matrix, &model);
    
        renderer_reset_buffers(&renderer);    
        renderer_render(&renderer, &view_proj, &model, &object, &light);
    
        memcpy(disp.framebuffer, renderer.frame_buffer, renderer.s_width * renderer.s_height * sizeof(int));
        Display_update(&disp);

        end = clock();
        printf("FPS: %f\n", 1.0f / ((double)(end - start)/CLOCKS_PER_SEC));
    }
    
    // Free assets
    free(test.diffuse);
    free_render_object(&object);
    renderer_destroy(&renderer);
    
    // Free display
    Display_destroy(&disp);

    return 0;
}
