#pragma once

#include <GLFW/glfw3.h>

#include "gml/GML.h"
#include "render/renderer.h"

typedef struct {
    Render_t renderer;

    int screen_width;
    int screen_height;

    float camera_fov;
} Engine_t;

int engine_init(Engine_t* engine, int width, int height, float camera_fov);

int engine_run(Engine_t* engine);

int engine_free(Engine_t* engine);
