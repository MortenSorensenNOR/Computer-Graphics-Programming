#include "window.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_scancode.h>

int window_init(Window_t* window, size_t width, size_t height, std::string window_name, bool fullscreen) {
    window->width = width;
    window->height = height;

    uint32_t window_flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }

    window->sdl_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    window->sdl_renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);
    window->fb_texture = SDL_CreateTexture(window->sdl_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);

    return 0;
}

int window_update(Window_t* window, Buffer<char>* fb) {
    if (fb->size() != window->width * window->height * 3) { 
        return 1;
    }

    if (!fb->data) {
        return 2;
    }

    // Copy over the framebuffer to an SDL texture and render that to the screen
    int err = SDL_UpdateTexture(window->fb_texture, NULL, fb->data, window->width * 3);
    if (err) {
        printf("Could not update texture (%s)\n", SDL_GetError());
        return 2;
    }

    // SDL_RenderClear(window->sdl_renderer);
    SDL_RenderCopy(window->sdl_renderer, window->fb_texture, NULL, NULL);

    // If using imgui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), window->sdl_renderer);

    // Present the render
    SDL_RenderPresent(window->sdl_renderer);

    return 0;
} 

int window_update_key_state(Window_t* window) {
    window->keystate = SDL_GetKeyboardState(NULL);
    return 0;
}

int window_update_mouse_state(Window_t* window) {
    Uint32 mouseState = SDL_GetMouseState(&window->mouse_pos_x, &window->mouse_pos_y); 
    window->mouse_left_btn_pressed = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
    window->mouse_right_btn_pressed = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
    SDL_GetRelativeMouseState(&window->mouse_delta_x, &window->mouse_delta_y);

    return 0;
}

int window_get_key_state(Window_t* window, char key) {
    SDL_KeyCode keycode = _window_get_sdl_keycode(key);
    if (keycode == SDLK_UNKNOWN) {
        return -1;
    }

    SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
    if (window->keystate[scancode])
        return 1;

    return 0;
}

int window_get_modifier_key_state(Window_t* window, WindowModifierKeys modifier) {
    static std::unordered_map<WindowModifierKeys, SDL_KeyCode> keycode_map {
        {WindowModifierKeys::LSHIFT, SDLK_LSHIFT},
        {WindowModifierKeys::RSHIFT, SDLK_RSHIFT},
        {WindowModifierKeys::LCTRL, SDLK_LCTRL},
        {WindowModifierKeys::RCTRL, SDLK_RCTRL},
        {WindowModifierKeys::SPACE, SDLK_SPACE},
        {WindowModifierKeys::ESCAPE, SDLK_ESCAPE},
    };

    if (keycode_map.find(modifier) != keycode_map.end()) {
        SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode_map[modifier]);
        return window->keystate[scancode];
    }
    return -1;
}

int window_get_mouse_state(Window_t* window, int& mouse_x, int& mouse_y, int& mouse_delta_x, int& mouse_delta_y, bool& mouse_left, bool& mouse_right) {
    mouse_x = window->mouse_pos_x;
    mouse_y = window->mouse_pos_y;
    mouse_delta_x = window->mouse_delta_x;
    mouse_delta_y = window->mouse_delta_y;
    mouse_left = window->mouse_left_btn_pressed;
    mouse_right = window->mouse_right_btn_pressed;
    return 0;
}

int window_check_should_close(Window_t* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            return 1;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_q) {
                return 1;
            }
        }
    }
    return 0;
}

int window_capture_mouse(Window_t* window) {
    SDL_SetWindowGrab(window->sdl_window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // SDL_SetWindowKeyboardGrab(window->sdl_window, SDL_TRUE); // Makes it annoying to fullscreen in i3
    return 0;
}

int window_release_mouse(Window_t* window) {
    SDL_SetWindowGrab(window->sdl_window, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    // SDL_SetWindowKeyboardGrab(window->sdl_window, SDL_FALSE);
    return 0;
}

int window_free(Window_t* window) {
    SDL_DestroyTexture(window->fb_texture);
    SDL_DestroyRenderer(window->sdl_renderer);
    SDL_DestroyWindow(window->sdl_window);
    SDL_Quit();

    return 0;
}

SDL_KeyCode _window_get_sdl_keycode(char key) {
    static std::unordered_map<char, SDL_KeyCode> key_to_keycode_map = {
        {'a', SDLK_a}, {'b', SDLK_b}, {'c', SDLK_c}, {'d', SDLK_d},
        {'e', SDLK_e}, {'f', SDLK_f}, {'g', SDLK_g}, {'h', SDLK_h},
        {'i', SDLK_i}, {'j', SDLK_j}, {'k', SDLK_k}, {'l', SDLK_l},
        {'m', SDLK_m}, {'n', SDLK_n}, {'o', SDLK_o}, {'p', SDLK_p},
        {'q', SDLK_q}, {'r', SDLK_r}, {'s', SDLK_s}, {'t', SDLK_t},
        {'u', SDLK_u}, {'v', SDLK_v}, {'w', SDLK_w}, {'x', SDLK_x},
        {'y', SDLK_y}, {'z', SDLK_z}, {'0', SDLK_0}, {'1', SDLK_1},
        {'2', SDLK_2}, {'3', SDLK_3}, {'4', SDLK_4}, {'5', SDLK_5},
        {'6', SDLK_6}, {'7', SDLK_7}, {'8', SDLK_8}, {'9', SDLK_9},
    };

    if (key_to_keycode_map.find(key) != key_to_keycode_map.end()) {
        return key_to_keycode_map[key];
    }
    return SDLK_UNKNOWN;
}
