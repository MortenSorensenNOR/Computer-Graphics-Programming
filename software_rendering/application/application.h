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

typedef struct {
    Window_t window;

    State_t app_state;
    Info_t app_info;
    Settings_t app_settings;
    InputState input_state;

    Engine_t engine;
    std::string assets_path;
} Application_t;

int application_init(Application_t* app, size_t screen_width, size_t screen_height, std::string window_name, std::string assets_path, bool fullscreen = false);

int application_run(Application_t* app);

int application_free(Application_t* app);
