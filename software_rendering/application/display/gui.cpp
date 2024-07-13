#include "gui.h"

int GUI_init(Display_t* display) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(display->window, display->sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(display->sdl_renderer);

    return 0;
}

int GUI_render_info(Info_t* info) {
    ImGui::Begin("Info");
    if (info->frame_time >= 0.0f) {
        ImGui::TextUnformatted(("Current frametime: " + std::to_string(info->frame_time * 1000.0) + " ms").c_str());
        ImGui::TextUnformatted(("Current FPS: " + std::to_string(1 / info->frame_time)).c_str());
    } else {
        ImGui::TextUnformatted("Current frametime: INVALID FRAMETIME");
        ImGui::TextUnformatted("Current FPS: INVALID FRAMETIME");
    }
    ImGui::End();

    return 0; 
}

int GUI_render_settings(Settings_t* settings) {

    return 0; 
}

int GUI_render(Info_t* info, Settings_t* settings) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    GUI_render_info(info);

    return 0; 
}

int GUI_free() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
