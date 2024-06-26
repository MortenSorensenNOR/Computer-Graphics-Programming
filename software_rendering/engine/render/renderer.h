#pragma once
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include "../utilities/Types.h"
#include "camera.h"

typedef struct {
    Camera_t* camera;

    u_char* frame_buffer;
    float* zbuffer;
} Render_t;

int renderer_init(Render_t* renderer, Camera_t* camera);

int renderer_reset_buffers(Render_t* renderer);

int renderer_render(Render_t* renderer, size_t current_texture, texture_t* textures);

int renderer_destroy(Render_t* renderer);
