#include "gui.h"

int GUI_init(Window_t* display) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(display->sdl_window, display->sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(display->sdl_renderer);

    return 0;
}

int GUI_render_info(Info_t* info) {
    static int i = 0;
    static std::string ft;
    static std::string fps;
    i++;

    ImGui::Begin("Info");
    if (info->frame_time >= 0.0f) {
        // Make info text readable
        if (i % 100 == 0) {
            ft = std::to_string(info->frame_time * 1000.0f);
            fps = std::to_string(1 / info->frame_time);
        }
        ImGui::TextUnformatted(("Current frametime: " + ft + " ms").c_str());
        ImGui::TextUnformatted(("Current FPS: " + fps).c_str());
    } else {
        ImGui::TextUnformatted("Current frametime: INVALID FRAMETIME");
        ImGui::TextUnformatted("Current FPS: INVALID FRAMETIME");
    }
    ImGui::End();

    return 0; 
}

int GUI_render_settings(Settings_t* settings) {
    ImGui::Begin("Settings");
    ImGui::SliderFloat("LookAt.x:", &settings->lookAt_x, -10.0f, 10.0f);
    ImGui::SliderFloat("LookAt.y:", &settings->lookAt_y, -10.0f, 10.0f);
    ImGui::SliderFloat("LookAt.z:", &settings->lookAt_z, -10.0f, 10.0f);
    ImGui::End();

    return 0; 
}

int GUI_render(Info_t* info, Settings_t* settings) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    GUI_render_info(info);
    GUI_render_settings(settings);

    return 0; 
}

int GUI_free() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return 0;
}