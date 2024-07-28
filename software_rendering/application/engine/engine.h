#pragma once

#include <memory>
#include <queue>

#include "../state/settings.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "renderer/renderer.h"
#include "renderer/backends/simple_renderer.h"

typedef struct {
    Scene_t scene;
    Renderer renderer;
} Engine_t;

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height);

int engine_run(Engine_t* engine, float dt, Settings_t* settings);

Buffer<char>* engine_get_fb(Engine_t* engine);

int engine_free(Engine_t* engine);
