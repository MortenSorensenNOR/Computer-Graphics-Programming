#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GAEDisplay.h"

int main(int argc, char** argv) {
    srand(time(0));

    struct GAEDisplay disp;
    GAEDisplay_init(&disp);

    while (1) {
        for (int y = 0; y < disp.height; y++) {
            for (int x = 0; x < disp.width; x++) {
                int index = x + y * disp.width;
                int color = (rand() % 0xFFFFFF);
                disp.framebuffer[index] = color;
            }
        }

        GAEDisplay_update(&disp);
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
