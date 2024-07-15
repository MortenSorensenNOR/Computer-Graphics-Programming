#include "application.h"

int application_init(Application_t* app, size_t screen_width, size_t screen_height, std::string window_name, std::string assets_path, bool fullscreen) {
    // Display and GUI init
    int err = display_init(&app->display, screen_width, screen_height, window_name, fullscreen);
    if (err) {
        printf("Could not initialize display\n");
        return 1;
    }
    GUI_init(&app->display);

    // Engine and scene init
    engine_init(&app->engine);
    scene_init(&app->engine.scene);

    if (assets_path == "")
        return 1;

    scene_load_scene_from_file(&app->engine.scene, assets_path, "scenes/cube.scene");

    return 0;
}

int application_run(Application_t* app) {
    std::size_t buff_size = app->display.width * app->display.height * 3;
    Buffer<u_char> test_fb = buffer_allocate<u_char>(buff_size);
    for (int i = 0; i < buff_size; i++) {
        test_fb.data[i] = 128;
    }

    time_t _last_frame, _current_frame;
    _last_frame = clock();

    while (1) {
        _current_frame = clock();
        app->app_info.frame_time = (float)(_current_frame - _last_frame)/(CLOCKS_PER_SEC);
        _last_frame = _current_frame;

        GUI_render(&app->app_info, &app->app_settings);
        display_update(&app->display, test_fb);

        if (display_check_should_close(&app->display))
            break;
    }

    buffer_free(test_fb);

    return 0;
}

int application_free(Application_t* app) {
    display_free(&app->display);
    GUI_free();

    return 0;
}
