#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"
#include "rutil/ModelLoader.h"

#include "render/renderer.h"

int main(int argc, char** argv) {
    srand(time(0));

    // Initialize display and user input
    Display_t disp;
    Input_t input;
    Display_init(&disp);
    Input_init(&input, &disp);
    Display_clear(&disp, 0x1C1D1E);

    // Load assets
    texture_t test;
    load_texture("../resources/chad.jpg", &test.diffuse, &test.diffuse_width, &test.diffuse_height);
    load_texture("../resources/birk.jpg", &test.specular, &test.specular_width, &test.specular_height);

    scene_t scene;
    parse_obj("../resources/cube.obj", &scene);

    triangle_t test_tri;
    test_tri.vert[0] = scene.objects[0].mesh->vertex[scene.objects[0].mesh->index[0]];
    test_tri.vert[1] = scene.objects[0].mesh->vertex[scene.objects[0].mesh->index[1]];
    test_tri.vert[2] = scene.objects[0].mesh->vertex[scene.objects[0].mesh->index[2]];

    printf("%f %f %f\n", test_tri.vert[0].x, test_tri.vert[0].y, test_tri.vert[0].z);
    printf("%f %f %f\n", test_tri.vert[1].x, test_tri.vert[1].y, test_tri.vert[1].z);
    printf("%f %f %f\n", test_tri.vert[2].x, test_tri.vert[2].y, test_tri.vert[2].z);

    // Main render/input loop
    clock_t start, end;
    while (1) {
        start = clock();

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

        end = clock();
    }

    // Free assets
    free(test.diffuse);
    free_scene(&scene);
    
    // Free display
    Display_destroy(&disp);

    return 0;
}
