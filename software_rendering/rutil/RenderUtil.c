#include "RenderUtil.h"

/*=============== COLOR ===============*/
int color_v(const vec3* c) {
    // Clip colors
    vec3 color_clamped = vec3_clamp(c, 0.0, 1.0);
    return (int)(color_clamped.x * 255) << 16 | (int)(color_clamped.y * 255) << 8 | (int)(color_clamped.z * 255);
}

int color_f(float r, float g, float b) {
    r = r < 0.0 ? 0.0 : r > 1.0 ? 1.0 : r;
    g = g < 0.0 ? 0.0 : g > 1.0 ? 1.0 : g;
    b = b < 0.0 ? 0.0 : b > 1.0 ? 1.0 : b;
    return (int)(r * 255) << 16 | (int)(g * 255) << 8 | (int)(b * 255);
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
