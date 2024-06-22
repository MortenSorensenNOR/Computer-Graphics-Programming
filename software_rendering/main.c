#include <stdlib.h>
#include <time.h>

#include "engine/engine.h"

int main(int argc, char** argv) {
    srand(time(0));

    Engine_t engine;
    engine_init(&engine, 1920, 1080, M_PI / 4);

    engine_run(&engine);    
    
    engine_free(&engine);

    return 0;
}
