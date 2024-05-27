#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"

int main(int argc, char** argv) {
    srand(time(0));

    GAEDisplay_t disp;
    GAEInput_t input;
    GAEDisplay_init(&disp);
    GAEInput_init(&input, &disp);

    GAEDisplay_clear(&disp, 0x1C1D1E);

    while (1) {
        GAEInput_update(&disp, &input);
        /* GAEDisplay_clear(&disp, 0x1C1D1E); */

        GAEDisplay_setPixel(&disp, input.mouse_x, input.mouse_y, 0xFF0000);

        GAEDisplay_update(&disp);

        if (GAEInput_getKeyState(&input, 'w')) {
            printf("W is pressed!\n");
        } 
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
