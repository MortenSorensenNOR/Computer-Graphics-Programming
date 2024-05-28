#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display/Display.h"
#include "rutil/RenderUtil.h"
#include "rutil/ModelLoader.h"

#include "render/renderer.h"

void bresenham(Display_t* disp, int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1; 
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;

    for (;;) {
        Display_setPixel(disp, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_triangle_wireframe(Display_t* disp, const vec2 vert[3], int color) {
    bresenham(disp, (int)vert[0].x, vert[0].y, (int)vert[1].x, (int)vert[1].y, color);
    bresenham(disp, (int)vert[1].x, vert[1].y, (int)vert[2].x, (int)vert[2].y, color);
    bresenham(disp, (int)vert[2].x, vert[2].y, (int)vert[0].x, (int)vert[0].y, color);
}

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

    // Free assets
    free(test.diffuse);
    free_scene(&scene);
    
    // Free display
    Display_destroy(&disp);

    return 0;
}
