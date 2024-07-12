#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "../core_utils/core_utils.h"

typedef struct {
    size_t width;
    size_t height;
    
    SDL_Window* render_target;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* fb_texture;
} Display_t;
