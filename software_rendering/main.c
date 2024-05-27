#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"

int main(int argc, char** argv) {
    srand(time(0));

    Display_t disp;
    Input_t input;
    Display_init(&disp);
    Input_init(&input, &disp);
    Display_clear(&disp, 0x1C1D1E);

    while (1) {
        Input_update(&input);
        /* Display_clear(&disp, 0x1C1D1E); */

        Display_setPixel(&disp, input.mouse_x, input.mouse_y, 0xFF0000);

        Display_update(&disp);

        if (Input_getKeyState(&input, 'w')) {
            printf("W is pressed!\n");
        } 
    }

    Display_destroy(&disp);

    return 0;
}
