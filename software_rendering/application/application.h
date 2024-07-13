#pragma once 

#include <time.h>

#include "state/settings.h"
#include "state/info.h"
#include "core_utils/core_utils.h"
#include "display/display.h"
#include "display/gui.h"

typedef struct {
    Display_t display;

    Info_t app_info;
    Settings_t app_settings;
} Application_t;

int application_init(Application_t* app, size_t screen_width, size_t screen_height, std::string window_name, bool fullscreen = false);

int application_run(Application_t* app);

int application_free(Application_t* app);
