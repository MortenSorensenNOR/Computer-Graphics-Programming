#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"
#include "rutil/ModelLoader.h"

#define RASTERIZER_MULTI_THREAD
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
    texture_t texture;
    load_texture("../resources/backpack/diffuse_new.jpg", &texture.diffuse, &texture.diffuse_width, &texture.diffuse_height);
    load_texture("../resources/backpack/metallic_roughness.png", &texture.specular, &texture.specular_width, &texture.specular_height);
    
    light_t light;
    light.pos = (vec3){1.2f, 1.0f, 2.0f};
    light.ambient = (vec3){0.1f, 0.1f, 0.1f};
    light.diffuse = (vec3){1.25f, 1.25f, 1.25f};
    light.specular = (vec3){1.0f, 1.0f, 1.0f};
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;

    render_object_t object;
    int object_load_success = parse_obj("../resources/backpack/backpack.obj", &object);
    if (object_load_success) {
        return 1;
    }
    object.textures = &texture;
    
    render_t renderer;
    renderer_init(&renderer, disp.width, disp.height, zfar, znear);
    
    // Matricies and stuff
    vec3 camera_pos   = {0.0f, 0.0f, -3.0f};
    vec3 camera_front = {0.0f, 0.0f, 1.0f};
    vec3 camera_up    = {0.0f, 1.0f, 0.0f};
    float aspect = (double)disp.width / (double)disp.height;

    mat4 view, projection, view_proj;
    mat4 rotation_matrix, model;
    
    float scale = 0.0135f;
    mat4 translation_matrix = mat4_translate(0, 0, 15.0f);
    mat4 scale_matrix = mat4_scale(scale, scale, scale);

    // Main render/input loop
    float angle = M_PI;

    float camera_angle_pitch = 0.0f;
    float camera_angle_roll = 0.0f;

    int iter = 0;
    float dt = 0;
    clock_t start = clock(), end = clock();
    while (1) {
        start = clock();
        dt = (float)(start - end) / CLOCKS_PER_SEC;
    
        Input_update(&input);
        if (Input_getKeyState(&input, 'x')) 
            break;

        if (Input_getKeyState(&input, 'h'))
            camera_angle_roll -= M_PI * 0.1;
        if (Input_getKeyState(&input, 'l'))
            camera_angle_roll += M_PI * 0.1;
        if (Input_getKeyState(&input, 'j'))
            camera_angle_pitch -= M_PI * 0.1;
        if (Input_getKeyState(&input, 'k'))
            camera_angle_pitch += M_PI * 0.1;

        angle += 0.01f * M_PI;

        // Camera
        camera_front.x = sin(camera_angle_roll);
        camera_front.y = sin(camera_angle_pitch);

        view = mat4_lookAt(camera_front, camera_pos, camera_up);
        projection = mat4_perspective(fov, aspect, znear, zfar);
        view_proj = mat4_mat4_mul_ret(&projection, &view);
    
        // Model
        rotation_matrix = mat4_rotate(0, angle, 0);
        model = mat4_mat4_mul_ret(&rotation_matrix, &scale_matrix);
        model = mat4_mat4_mul_ret(&translation_matrix, &model);
    
        renderer_reset_buffers(&renderer);    
        renderer_render(&renderer, camera_pos, &view_proj, &model, &object, &light);
    
        memcpy(disp.framebuffer, renderer.frame_buffer, renderer.s_width * renderer.s_height * sizeof(int));
        Display_update(&disp);

        end = clock();
        printf("FPS: %f\n", 1.0f / ((double)(end - start)/CLOCKS_PER_SEC));
    }
    
    // Free assets
    free(texture.diffuse);
    free_render_object(&object);
    renderer_destroy(&renderer);
    
    // Free display
    Display_destroy(&disp);

    return 0;
}
