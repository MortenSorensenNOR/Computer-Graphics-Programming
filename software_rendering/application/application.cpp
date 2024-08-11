#include "application.h"

int application_init(Application_t* app, size_t screen_width, size_t screen_height, std::string window_name, std::string assets_path, bool fullscreen) {
    app->input_state.active_window = &app->window;
    window_init(&app->window, screen_width, screen_height, window_name, fullscreen);
    GUI_init(&app->window);
    engine_init(&app->engine, screen_width, screen_height);

    if (assets_path == "")
        return 1;
    scene_load_scene_from_file(&app->engine.scene, assets_path, "scenes/cube.scene");

    return 0;
}

int application_run(Application_t* app) {
    // Setup time measurement
    time_t _last_frame, _current_frame;
    _last_frame = clock();

    // Main application loop
    bool _quit = false;
    while (!_quit) {
        _current_frame = clock();
        app->app_info.frame_time = (float)(_current_frame - _last_frame)/(CLOCKS_PER_SEC);
        _last_frame = _current_frame;

        // User input - hooks into window key state and mouse state functions
        app->input_state.updateKeyState();
        app->input_state.updateMouseState();

        // Capture mouse to interact with camera through mouse movement
        if (app->input_state.mouse_left) {
            window_capture_mouse(&app->window);
            app->app_state.mouse_captured = true;
        }
        else if (app->input_state.getModifierKeyState(WindowModifierKeys::ESCAPE)) {
            window_release_mouse(&app->window);
            app->app_state.mouse_captured = false;
        }

        // Render
        engine_run(&app->engine, app->app_info.frame_time, &app->app_state, &app->app_settings, &app->input_state);
        
        // Display update
        Buffer<char>* render_fb = engine_get_fb(&app->engine);
        GUI_render(&app->app_info, &app->app_settings);
        window_update(&app->window, render_fb);

        if (window_check_should_close(&app->window))
            _quit = true;
    }

    return 0;
}

int application_free(Application_t* app) {
    GUI_free();
    window_free(&app->window);
    engine_free(&app->engine);

    return 0;
}
