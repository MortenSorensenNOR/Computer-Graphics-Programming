#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"
#include "rutil/ModelLoader.h"

#include "render/VertexShader.h"

int main(int argc, char** argv) {
    srand(time(0));

    Display_t disp;
    Input_t input;
    Display_init(&disp);
    Input_init(&input, &disp);
    Display_clear(&disp, 0x1C1D1E);

    texture_t test;
    load_texture("../resources/chad.jpg", &test.diffuse, &test.diffuse_width, &test.diffuse_height);

    /* scene_t scene; */
    /* parse_obj("../resources/airboat.obj", &scene); */

    while (1) {
        Input_update(&input);
        if (Input_getKeyState(&input, 'w')) 
            break;

        Display_clear(&disp, 0x1C1D1E);
    
        for (int x = 0; x < test.diffuse_width; ++x) {
            for (int y = 0; y < test.diffuse_height; ++y) {
                int color = color_to_int(&test.diffuse[x + y * test.diffuse_width]);
                Display_setPixel(&disp, x, y, color);
            }
        }
    
        Display_update(&disp);
    }

    free(test.diffuse);
    /* free_scene(&scene); */

    Display_destroy(&disp);

    return 0;
}
