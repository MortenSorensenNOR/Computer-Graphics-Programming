#pragma once
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../utilities/RenderUtil.h"

typedef struct {
    int s_width;
    int s_height;
    float zfar;
    float znear;

    u_char* frame_buffer;
    float* zbuffer;
} Render_t;

int renderer_init(Render_t* renderer, int s_width, int s_height, float zfar, float znear);

int renderer_reset_buffers(Render_t* renderer);

int renderer_render(Render_t* renderer, size_t current_texture, texture_t* textures);

int renderer_destroy(Render_t* renderer);
