#include <stdlib.h>
#include "RenderUtil.h"

int color_f(float r, float g, float b) {
    r = r < 0.0 ? 0.0 : r > 1.0 ? 1.0 : r;
    g = g < 0.0 ? 0.0 : g > 1.0 ? 1.0 : g;
    b = b < 0.0 ? 0.0 : b > 1.0 ? 1.0 : b;
    return (int)(r * 255) << 16 | (int)(g * 255) << 8 | (int)(b * 255);
}
