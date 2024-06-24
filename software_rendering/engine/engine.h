#pragma once

#include <GLFW/glfw3.h>

#include "gml/GML.h"
#include "utilities/RenderUtil.h"
#include "utilities/ModelLoader.h"
#include "render/renderer.h"

#define MAX_NUM_TEXTURES 256

typedef struct {
    size_t n_textures;
    texture_t* textures;
} Engine_assets_t;

typedef struct {
    int screen_width;
    int screen_height;

    GLFWwindow* window;

    Engine_assets_t assets;

    Render_t renderer;
    float camera_fov;
} Engine_t;

// For external reference in glfw callbacks/hooks
extern Engine_t* current_engine;

int engine_init(Engine_t* engine, int width, int height, char* window_name, float camera_fov);
int engine_window_init(Engine_t* engine, int width, int height, char* window_name);

static void engine_get_key_input(GLFWwindow* window, int key, int scancode, int action, int mods);
int engine_run(Engine_t* engine);

int engine_free(Engine_t* engine);
