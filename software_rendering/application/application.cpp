#include "application.h"

int application_init(Application_t* app, size_t screen_width, size_t screen_height, std::string window_name, std::string assets_path, bool fullscreen) {
    display_init(&app->display, screen_width, screen_height, window_name, fullscreen);
    GUI_init(&app->display);
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

        engine_run(&app->engine, app->app_info.frame_time, &app->app_settings);
        
        Buffer<char>* render_fb = engine_get_fb(&app->engine);
        GUI_render(&app->app_info, &app->app_settings);
        display_update(&app->display, render_fb);

        if (display_check_should_close(&app->display))
            _quit = true;
    }

    return 0;
}

int application_free(Application_t* app) {
    display_free(&app->display);
    GUI_free();
    engine_free(&app->engine);

    return 0;
}
