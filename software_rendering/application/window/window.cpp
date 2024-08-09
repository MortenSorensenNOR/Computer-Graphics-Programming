#include "window.h"

int display_init(Window_t* window, size_t width, size_t height, std::string window_name, bool fullscreen) {
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

int display_update(Window_t* window, Buffer<char>* fb) {
    if (fb->size != window->width * window->height * 3) { 
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

    SDL_RenderClear(window->sdl_renderer);
    SDL_RenderCopy(window->sdl_renderer, window->fb_texture, NULL, NULL);

    // If using imgui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), window->sdl_renderer);

    // Present the render
    SDL_RenderPresent(window->sdl_renderer);

    return 0;
} 

int display_check_should_close(Window_t* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            return 1;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) {
                return 1;
            }
        }
    }
    return 0;
}

int display_free(Window_t* window) {
    SDL_DestroyTexture(window->fb_texture);
    SDL_DestroyRenderer(window->sdl_renderer);
    SDL_DestroyWindow(window->sdl_window);
    SDL_Quit();

    return 0;
}

