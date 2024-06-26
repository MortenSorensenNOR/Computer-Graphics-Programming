#include "renderer.h"

int renderer_init(Render_t* renderer, Camera_t* camera) {
    renderer->camera = camera;

    renderer->frame_buffer = (u_char*)malloc(camera->width * camera->height * 3);
    renderer->zbuffer = (float*)malloc(camera->width * camera->height * sizeof(float));

    return 0;
}

int renderer_reset_buffers(Render_t* renderer) {
    glm::vec2 resolution = camera_get_resolution(renderer->camera);
    memset(renderer->frame_buffer, 0, resolution.x * resolution.y * sizeof(int));
    // for (int i = 0; i < renderer->s_width * renderer->s_height; ++i) {
    //     renderer->zbuffer[i] = 1.0f;
    // }
    
    return 0;
}

int renderer_render(Render_t* renderer, size_t current_texture, texture_t* textures) {
    glm::vec2 resolution = camera_get_resolution(renderer->camera);

    texture_t* test = &textures[current_texture];
    for (int y = 0; y < resolution.y; y++) {
        for (int x = 0; x < resolution.x; x++) {
            int y1 = y % test->height;
            int x1 = x % test->width;
            
            int i = x1 + test->width * y1;
            glm::vec3 c = test->data[i];

            u_char r = (u_char)(c.x * 255);
            u_char g = (u_char)(c.y * 255);
            u_char b = (u_char)(c.z * 255);

            int fb_i = x + y * resolution.x;
            renderer->frame_buffer[3 * fb_i + 0] = r;
            renderer->frame_buffer[3 * fb_i + 1] = g;
            renderer->frame_buffer[3 * fb_i + 2] = b;
        }
    } 

    return 0;
}   

int renderer_destroy(Render_t* renderer) {
    if (renderer->frame_buffer)
        free(renderer->frame_buffer);

    if (renderer->zbuffer)
        free(renderer->zbuffer);

    return 0;
}
