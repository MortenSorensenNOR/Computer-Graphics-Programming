#include "engine.h"

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height) {
    scene_init(&engine->scene, screen_width, screen_height);

    return 0;
}

int engine_run(Engine_t* engine, float dt) {
    scene_update(&engine->scene);
     
    return 0;
}

int engine_free(Engine_t* engine) {
    scene_free(&engine->scene);

    return 0;
}
