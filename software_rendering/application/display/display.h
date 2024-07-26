#pragma once

#include <memory>
#include <string>
#include <omp.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

#include "../core_utils/core_utils.h"
#include "../core_utils/buffer.h"

typedef struct {
    std::size_t width;
    std::size_t height;
    
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* fb_texture;
} Display_t;

int display_init(Display_t* display, size_t width, size_t height, std::string window_name, bool fullscreen = false);

int display_update(Display_t* display, Buffer<char>* fb); 

int display_check_should_close(Display_t* display);

int display_free(Display_t* display);
