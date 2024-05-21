#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/GAEDisplay.h"
#include "rutil/RenderUtil.h"

int main(int argc, char** argv) {
    srand(time(0));

    GAEDisplay_t disp;
    user_input input_buffer;
    GAEDisplay_init(&disp);
    GAEDisplay_clear(&disp, 0x1C1D1E);

    while (1) {
        GAEDisplay_getInput(&disp, &input_buffer);
        GAEDisplay_clear(&disp, 0x1C1D1E);
        GAEDisplay_update(&disp);
    }

    GAEDisplay_destroy(&disp);

    return 0;
}
