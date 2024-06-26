#include "engine.h"

Engine_t* current_engine;

int engine_init(Engine_t* engine, int width, int height, std::string window_name, float fov) {
    current_engine = engine;

    // Initialize window
    int err = engine_window_init(engine, width, height, window_name);
    if (err != 0) {
        printf("Error: could not inizialize window. Error code %d\n", err);
        return 1;
    }

    engine_imgui_init(engine);

    // Initialize engine assets
    engine->assets.n_textures = 2;
    engine->assets.textures = (texture_t*)malloc(engine->assets.n_textures * sizeof(texture_t));

    texture_t* texture_1 = &engine->assets.textures[0];
    load_texture("../resources/imgs/chad.jpg", &texture_1->data, &texture_1->width, &texture_1->height);

    texture_t* texture_2 = &engine->assets.textures[1];
    load_texture("../resources/imgs/birk.jpg", &texture_2->data, &texture_2->width, &texture_2->height);

    // Initialize camera and renderer
    camera_init(&engine->camera, fov, width, height);
    renderer_init(&engine->renderer, &engine->camera);

    return 0;
}

int engine_window_init(Engine_t* engine, int width, int height, std::string window_name) {
    engine->screen_width = width;
    engine->screen_height = height;

    engine->window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    engine->sdl_renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_SOFTWARE);
    engine->fb_texture = SDL_CreateTexture(engine->sdl_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, width, height);

    return 0;
}

int engine_imgui_init(Engine_t* engine) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(engine->window, engine->sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(engine->sdl_renderer);

    return 0;
}

int engine_run(Engine_t* engine) {
    int current_texture = 0;
    bool running = true;

    int i = 0;
    double frameTimesLastFiveFrames = 0.0;
    double averagedFPS = 0.0;
    engine->lastFrameTime = SDL_GetPerformanceCounter();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        Uint32 currentFrameTime = SDL_GetPerformanceCounter();
        engine->deltaTime = (double)((currentFrameTime - engine->lastFrameTime)) / (double)(SDL_GetPerformanceFrequency());
        engine->lastFrameTime = currentFrameTime;

        i++;
        frameTimesLastFiveFrames += engine->deltaTime;

        if (i >= 5 && frameTimesLastFiveFrames >= 0.000001) {
            averagedFPS = 1.0 / (frameTimesLastFiveFrames / 5.0);
            frameTimesLastFiveFrames = 0.0;
            i = 0;
        }

        // ImGui frame setup
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::TextUnformatted(("Current frametime: " + std::to_string(averagedFPS)).c_str());
        ImGui::SliderInt("Image num", &current_texture, 0, 1);
        ImGui::End();

        // Software rendering
        renderer_render(&engine->renderer, current_texture, engine->assets.textures);

        // SDL render and present renderer framebuffer 
        ImGui::Render();
        int err = SDL_UpdateTexture(engine->fb_texture, NULL, engine->renderer.frame_buffer, engine->screen_width * 3);
        SDL_RenderClear(engine->sdl_renderer);
        SDL_RenderCopy(engine->sdl_renderer, engine->fb_texture, NULL, NULL);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), engine->sdl_renderer);
        SDL_RenderPresent(engine->sdl_renderer);
    }

    return 0;
}

int engine_free(Engine_t* engine) {
    // ImGui cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // SDL cleanup
    SDL_DestroyTexture(engine->fb_texture);
    SDL_DestroyRenderer(engine->sdl_renderer);
    SDL_DestroyWindow(engine->window);
    SDL_Quit();

    // Renderer cleanup
    renderer_destroy(&engine->renderer);

    return 0;
}
