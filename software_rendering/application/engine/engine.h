#pragma once

#include "scene/scene.h"

typedef struct {
    Scene_t scene;
} Engine_t;

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height);

int engine_run(Engine_t* engine, float dt);

int engine_free(Engine_t* engine);
