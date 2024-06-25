#pragma once

#include <SDL2/SDL_render.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

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

    Engine_assets_t assets;

    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* fb_texture;

    Render_t renderer;
    float camera_fov;
} Engine_t;

// For external reference in glfw callbacks/hooks
extern Engine_t* current_engine;

int engine_init(Engine_t* engine, int width, int height, std::string window_name, float camera_fov);
int engine_window_init(Engine_t* engine, int width, int height, std::string window_name);
int engine_imgui_init(Engine_t* engine);

int engine_run(Engine_t* engine);

int engine_free(Engine_t* engine);
