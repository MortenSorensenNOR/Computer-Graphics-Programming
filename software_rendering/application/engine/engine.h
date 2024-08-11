#pragma once

#include <memory>
#include <queue>

#include "../state/state.h"
#include "../state/settings.h"
#include "../state/input.h"
#include "game_logic/CameraController.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "renderer/renderer.h"
#include "renderer/backends/simple_renderer.h"
#include "renderer/backends/tiled_renderer.h"

typedef struct {
    Scene_t scene;
    Renderer renderer;

    std::unique_ptr<CameraController> camera_controller;
} Engine_t;

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height);

int engine_run(Engine_t* engine, float dt, State_t* app_state, Settings_t* settings, InputState* input);

Buffer<char>* engine_get_fb(Engine_t* engine);

int engine_free(Engine_t* engine);
