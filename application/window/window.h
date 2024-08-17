#pragma once

#include <memory>
#include <string>
#include <unordered_map>

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

enum class WindowModifierKeys {
    LSHIFT,
    LCTRL,
    RSHIFT,
    RCTRL,
    SPACE,
    ESCAPE,
};

typedef struct {
    std::size_t width;
    std::size_t height;
    
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* fb_texture;

    const Uint8* keystate;
    int mouse_pos_x;
    int mouse_pos_y;
    int mouse_delta_x;
    int mouse_delta_y;
    bool mouse_left_btn_pressed;
    bool mouse_right_btn_pressed;
} Window_t;

int window_init(Window_t* window, size_t width, size_t height, std::string window_name, bool fullscreen = false);

int window_update(Window_t* window, Buffer<char>* fb); 

int window_update_key_state(Window_t* window);

int window_update_mouse_state(Window_t* window);

int window_get_key_state(Window_t* window, char key);

int window_get_modifier_key_state(Window_t* window, WindowModifierKeys modifier);

int window_get_mouse_state(Window_t* window, int& mouse_x, int& mouse_y, int& mouse_delta_x, int& mouse_delta_y, bool& mouse_left, bool& mouse_right);

int window_check_should_close(Window_t* window);

int window_capture_mouse(Window_t* window);

int window_release_mouse(Window_t* window);

int window_free(Window_t* window);

SDL_KeyCode _window_get_sdl_keycode(char key);
