#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/implot.h"

int GUI_init(Window_t* display) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImPlot::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(display->sdl_window, display->sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(display->sdl_renderer);

    return 0;
}

int GUI_render_info(Info_t* info) {
    static float time_elapsed = 0.0;
    static std::string ft;
    static std::string fps;

    constexpr int HISTORY_SIZE = 128;
    static int frame_index = 0;
    static float frame_time_data[HISTORY_SIZE];
    frame_time_data[frame_index] = info->frame_time * 1000.0; 

    static float frame_time_max = 0.0f;
    static int frame_time_max_index = 127;
    if (info->frame_time * 1000.0f > frame_time_max || frame_time_max_index > frame_index || frame_index == 0) {
        frame_time_max = info->frame_time * 1000.0f;
        frame_time_max_index = frame_index;
    }

    static float frame_time_min = 100000.0f;
    static int frame_time_min_index = 127;
    if (info->frame_time * 1000.0f < frame_time_min || frame_time_min_index > frame_index || frame_index == 0) {
        frame_time_min = info->frame_time * 1000.0f;
        frame_time_min_index = frame_index;
    }
    frame_index = (frame_index + 1) % HISTORY_SIZE;

    time_elapsed += info->frame_time;
    if (time_elapsed > 0.2) {
        ft = std::to_string(info->frame_time * 1000.0);
        if (info->frame_time <= 0.0000001f)
            fps = "INVALID FRAMERATE";
        else
            fps = std::to_string(1 / info->frame_time);
        time_elapsed -= 0.2;
    }

    ImGui::BeginChild("Info", ImVec2(367, 140 + 4 * ImGui::GetTextLineHeight()), true);
    ImGui::TextUnformatted(("Current frametime: " + ft + " ms").c_str());
    ImGui::TextUnformatted(("Current FPS: " + fps).c_str());

    if (ImPlot::BeginPlot("Frametime/frame", ImVec2(350, 140))) {
        ImPlot::StyleColorsDark();
        ImPlot::SetupAxisLimits(ImAxis_Y1, frame_time_min * 0.65, frame_time_max * 1.35, ImGuiCond_Always);
        ImPlot::PlotLine("", frame_time_data, HISTORY_SIZE, 1, 0, 0, frame_index);
        ImPlot::EndPlot();
    }

    ImGui::EndChild();

    return 0; 
}

int GUI_render_settings(Settings_t* settings) {
    ImGui::BeginChild("Settings", ImVec2(0, 3*ImGui::GetTextLineHeight()), true);
    ImGui::Checkbox("Depth view", &settings->depthView);
    ImGui::EndChild();

    return 0; 
}

int GUI_render(Info_t* info, Settings_t* settings) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings + Info");

    ImGui::BeginChild("Instructions", ImVec2(0, 5 * ImGui::GetTextLineHeight()), true);
    ImGui::Text("Instructions:");
    ImGui::BulletText("Click on the render view to capture the mouse.");
    ImGui::BulletText("Press ESC to release the mouse.");
    ImGui::EndChild();

    GUI_render_info(info);
    GUI_render_settings(settings);

    ImGui::End();

    return 0; 
}

bool GUI_is_any_window_hovered() {
    // Check if any ImGui window is hovered, including any child windows or popups
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_RootAndChildWindows) || ImGui::IsAnyItemHovered();
}

int GUI_free() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    return 0;
}
