#pragma once

#include <memory>
#include <queue>

#include "../core_utils/timer.h"
#include "../state/state.h"
#include "../state/settings.h"
#include "../state/input.h"
#include "game_logic/CameraController.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "renderer/renderer.h"
#include "renderer/backends/simple_renderer.h"
#include "renderer/backends/tiled_renderer/tiled_renderer.h"
#include "renderer/post_process/lanczos_upscale.h"
#include "renderer/post_process/bilinear_upscale.h"
#include "renderer/post_process/fxaa.h"

class Engine {
private:
    Scene scene;
    Renderer renderer;

    std::unique_ptr<CameraController> camera_controller;

    Buffer<char> output_frame_buffer;

public:
    Engine(std::size_t screen_width, std::size_t screen_height, Settings_t* app_settings);

    ~Engine();

    /**
     * @brief Load in a scene from a file
     * @return Error code
     */
    int load_scene_from_file(std::string assets_path, std::string file_name);

    /**
     * @brief Run the engine ~~~ update the scene, run game logic and render to framebuffer
     * @param dt            The time since last ran
     * @param app_state     A pointer to the current app state
     * @param settings      A pointer to the current app settings
     * @param input         A pointer to the current app input state
     * @return Error code
     */
    int run(float dt, State_t* app_state, Settings_t* settings, InputState* input);

    /**
     * @brief Retrieves a pointer to the current renderers framebuffer
     * @return The pointer to the framebuffer, i.e. a char buffer of size 3 * screen_width * screen_height
     */
    Buffer<char>* get_fb();
};
