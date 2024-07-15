#include "engine.h"

int engine_init(Engine_t* engine) {
    scene_init(&engine->scene);

    return 0;
}

int engine_run(Engine_t* engine, float dt) {
     
    return 0;
}

int engine_free(Engine_t* engine) {
    scene_free(&engine->scene);

    return 0;
}
