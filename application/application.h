#pragma once 

#include <time.h>

#include "state/state.h"
#include "state/settings.h"
#include "state/info.h"
#include "state/input.h"
#include "core_utils/core_utils.h"
#include "window/window.h"
#include "window/gui.h"
#include "engine/engine.h"

class Application {
private:
    Window_t window;

    State_t app_state;
    Info_t app_info;
    Settings_t app_settings;
    InputState input_state;

    Engine engine;
    std::string assets_path;

public:
    Application(size_t screen_width, size_t screen_height, std::string window_name, std::string assets_path, bool fullscreen = false);
    ~Application();

    /**
     * @brief Main function of the project ~~~ Will run until window is closed
     * @return Error code
     */
    int run();
};

