#include <stdlib.h>
#include <time.h>

#include "engine/engine.h"

int main(int argc, char** argv) {
    srand(time(0));

    Engine_t engine;
    int err = engine_init(&engine, 1920, 1080, "Computer Graphics Programming", 90.0f);
    if (err != 0) {
        return 0;
    }

    engine_run(&engine);    
    
    engine_free(&engine);

    return 0;
}
