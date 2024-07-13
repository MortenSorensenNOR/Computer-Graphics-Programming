#pragma once

#include <string>

#include "display.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

#include "../state/info.h"
#include "../state/settings.h"

int GUI_init(Display_t* display);

int GUI_render_info(Info_t* info);

int GUI_render_settings(Settings_t* settings);

int GUI_render(Info_t* info, Settings_t* settings);

int GUI_free();
