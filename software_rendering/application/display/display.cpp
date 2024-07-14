#include "display.h"

int display_init(Display_t* display, size_t width, size_t height, std::string window_name, bool fullscreen) {
    display->width = width;
    display->height = height;

    uint32_t window_flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }

    display->window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    display->sdl_renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_SOFTWARE);
    display->fb_texture = SDL_CreateTexture(display->sdl_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);

    return 0;
}

int display_update(Display_t* display, Buffer<u_char>& fb) {
    if (fb.size != display->width * display->height * 3) { 
        return 1;
    }

    if (!fb.data) {
        return 2;
    }

    // Copy over the framebuffer to an SDL texture and render that to the screen
    int err = SDL_UpdateTexture(display->fb_texture, NULL, fb.data, display->width * 3);
    if (err) {
        printf("Could not update texture (%s)\n", SDL_GetError());
        return 2;
    }

    SDL_RenderClear(display->sdl_renderer);
    SDL_RenderCopy(display->sdl_renderer, display->fb_texture, NULL, NULL);

    // If using imgui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), display->sdl_renderer);

    // Present the render
    SDL_RenderPresent(display->sdl_renderer);

    return 0;
} 

int display_check_should_close(Display_t* display) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            return 1;
        }
    }
    return 0;
}

int display_free(Display_t* display) {
    SDL_DestroyTexture(display->fb_texture);
    SDL_DestroyRenderer(display->sdl_renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();

    return 0;
}

