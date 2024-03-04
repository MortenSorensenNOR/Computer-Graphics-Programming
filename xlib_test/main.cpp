#include <stdio.h>
#include <stdlib.h>

#include <random>

#include "DisplayClass.h"

int main(int argc, char** argv) {
    GAE::GAEDisplay display;  
    
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 0xFFFFFF); // define the range

    while (1) {
        std::pair<int, int> windowDimention = display.getWindowDimentions();
        int* framebuffer = display.getFrameBuffer();

        printf("Writing to frame buffer\n");
        for (int y = 0; y < windowDimention.second; y++) {
            for (int x = 0; x < windowDimention.first; x++) {
                int index = x + y * windowDimention.first;
                int color = distr(gen);
                framebuffer[index] = int(color);
            }
        }

        display.update();
    }

    return 0;
}
