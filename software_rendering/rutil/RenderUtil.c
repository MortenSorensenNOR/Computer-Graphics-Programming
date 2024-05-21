#include "RenderUtil.h"
#include <math.h>

/*=============== COLOR ===============*/
int color_to_int(const color_t c) {
    // Clip colors
    int r = c.r;
    int g = c.g;
    int b = c.b;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    return r << 16 | g << 8 | b;
}

vec3 color_to_vec3(const color_t c) {
    // Clip colors
    int r = c.r;
    int g = c.g;
    int b = c.b;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    return (vec3){r, g, b};
}

vec3 get_barycentric_coordinate2d(const vec2 vert[3], const vec2* p) {
    vec2 v0 = vec2_sub(&vert[1], &vert[0]);
    vec2 v1 = vec2_sub(&vert[2], &vert[0]);
    vec2 v2 = vec2_sub(p, &vert[0]);

    double d00 = vec2_dot(&v0, &v0);
    double d01 = vec2_dot(&v0, &v1);
    double d11 = vec2_dot(&v1, &v1);
    double d20 = vec2_dot(&v2, &v0);
    double d21 = vec2_dot(&v2, &v1);

    double det = (d00 * d11 - d01 * d01);
    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0f - v - w;
    return (vec3){u, v, w};
}

vec3 get_barycentric_coordinate3d(const vec3 vert[3], const vec3* p) {
    vec3 v0 = vec3_sub(&vert[1], &vert[0]);
    vec3 v1 = vec3_sub(&vert[2], &vert[0]);
    vec3 v2 = vec3_sub(p, &vert[0]);

    double d00 = vec3_dot(&v0, &v0);
    double d01 = vec3_dot(&v0, &v1);
    double d11 = vec3_dot(&v1, &v1);
    double d20 = vec3_dot(&v2, &v0);
    double d21 = vec3_dot(&v2, &v1);

    double det = (d00 * d11 - d01 * d01);
    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0f - v - w;
    return (vec3){u, v, w};
}
