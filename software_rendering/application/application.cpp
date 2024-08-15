#include "application.h"

Application::Application(size_t screen_width, size_t screen_height, std::string window_name, std::string assets_path, bool fullscreen) 
    : engine(screen_width, screen_height, &app_settings) {
    
    input_state.active_window = &window;
    window_init(&window, screen_width, screen_height, window_name, fullscreen);
    GUI_init(&window);

    std::string file_name = "scenes/cube.scene";
    int err = engine.load_scene_from_file(assets_path, file_name);
    if (err) {
        throw std::runtime_error("Could not load scene from file (" + file_name + ")");
    }
}

Application::~Application() {
    GUI_free();
    window_free(&window);
}

int Application::run() {
    // Setup time measurement
    time_t _last_frame, _current_frame;
    _last_frame = clock();

    // Main application loop
    bool _quit = false;
    while (!_quit) {
        _current_frame = clock();
        app_info.frame_time = (float)(_current_frame - _last_frame)/(CLOCKS_PER_SEC);
        _last_frame = _current_frame;

        // User input - hooks into window key state and mouse state functions
        input_state.updateKeyState();
        input_state.updateMouseState();

        // Capture mouse to interact with camera through mouse movement
        if (input_state.mouse_left && !GUI_is_any_window_hovered()) {
            window_capture_mouse(&window);
            app_state.mouse_captured = true;
        }
        else if (input_state.getModifierKeyState(WindowModifierKeys::ESCAPE)) {
            window_release_mouse(&window);
            app_state.mouse_captured = false;
        }

        // Render
        engine.run(app_info.frame_time, &app_state, &app_settings, &input_state);
        
        // Display update
        Buffer<char>* render_fb = engine.get_fb();
        GUI_render(&app_info, &app_settings);
        window_update(&window, render_fb);

        if (window_check_should_close(&window))
            _quit = true;
    }

    return 0;
}
