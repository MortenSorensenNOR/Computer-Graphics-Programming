#include "renderer.h"

int renderer_init(Render_t* renderer, int s_width, int s_height, float zfar, float znear) {
    renderer->s_width = s_width;
    renderer->s_height = s_height;

    renderer->frame_buffer = (u_char*)malloc(s_width * s_height * 3);
    renderer->zbuffer = (float*)malloc(s_width * s_height * sizeof(float));

    renderer->zfar = zfar;
    renderer->znear = znear;

    return 0;
}

int renderer_reset_buffers(Render_t* renderer) {
    memset(renderer->frame_buffer, 0, renderer->s_width * renderer->s_height * sizeof(int));
    for (int i = 0; i < renderer->s_width * renderer->s_height; ++i) {
        renderer->zbuffer[i] = 1.0f;
    }
    
    return 0;
}

int renderer_render(Render_t* renderer, size_t current_texture, texture_t* textures) {
    texture_t* test = &textures[current_texture];
    for (int y = 0; y < renderer->s_height; y++) {
        for (int x = 0; x < renderer->s_width; x++) {
            int y1 = y % test->height;
            int x1 = x % test->width;
            
            int i = x1 + test->width * y1;
            vec3 c = test->data[i];

            u_char r = (u_char)(c.x * 255);
            u_char g = (u_char)(c.y * 255);
            u_char b = (u_char)(c.z * 255);

            int fb_i = x + y * renderer->s_width;
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
