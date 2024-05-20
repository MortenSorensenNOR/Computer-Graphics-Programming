#include "RenderUtil.h"
#include <math.h>

/*=============== COLOR ===============*/
int color_to_int(const color_t c) {
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

color_t color_interpolate(const color_t a, const color_t b, double t) {
    return (color_t){lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t)};
}

/*=============== MATH-STUFF ===============*/
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

void transform_triangle(const vec4 vert[3], vec3 transformed_vert[3], const mat4* model, const mat4* view, const mat4* projection, int s_width, int s_height, double z_near, double z_far) {
    for (int i = 0; i < 3; ++i) {
        transformed_vert[i] = transform_vertex(&vert[i], model, view, projection, s_width, s_height, z_near, z_far);
    }
}

/*=============== RENDER-STUFF ===============*/
void bresenham(GAEDisplay_t* disp, int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1; 
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;

    for (;;) {
        GAEDisplay_setPixel(disp, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_triangle_wireframe(GAEDisplay_t* disp, const vec2 vert[3], int color) {
    bresenham(disp, (int)vert[0].x, vert[0].y, (int)vert[1].x, (int)vert[1].y, color);
    bresenham(disp, (int)vert[1].x, vert[1].y, (int)vert[2].x, (int)vert[2].y, color);
    bresenham(disp, (int)vert[2].x, vert[2].y, (int)vert[0].x, (int)vert[0].y, color);
}

void draw_triangle_filled(GAEDisplay_t* disp, const vec3 vert[3], const vec2 uv[3], const color_t vert_color[3], const vec3 normals[3], const light_t* light) {
    vec3 p0 = {(int)vert[0].x, (int)vert[0].y, vert[0].z};
    vec3 p1 = {(int)vert[1].x, (int)vert[1].y, vert[1].z};
    vec3 p2 = {(int)vert[2].x, (int)vert[2].y, vert[2].z};

    if (p1.y < p0.y) swap_vec3(&p0, &p1);
    if (p2.y < p0.y) swap_vec3(&p2, &p0);
    if (p2.y < p1.y) swap_vec3(&p2, &p1);

    int total_height = (int)(p2.y - p0.y);
    for (int i = 0; i < total_height; i++) {
        if (p0.y + i < 0 || p0.y + i >= disp->height) continue;
        int second_half = i > p1.y - p0.y || p1.y == p0.y;
        int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height;

        vec2 A, B;
        A = (vec2){(int)(p0.x + (p2.x - p0.x) * alpha), (int)(p0.y + i)};
        
        if (second_half) {
            B = (vec2){(int)(p1.x + (p2.x - p1.x) * beta), (int)(p1.y + (i - (p1.y - p0.y)))};
        } else {
            B = (vec2){(int)(p0.x + (p1.x - p0.x) * beta), (int)( p0.y + i)};
        }

        if ((int)A.x > (int)B.x) {
            int t = A.x;
            A.x = B.x;
            B.x = t;
        };

        if ((int)A.x < 0) A.x = 0;
        if ((int)B.x > disp->width) B.x = disp->width;
        for (int x = (int)A.x; x < (int)B.x; x++) {
            if (x < 0 || x >=disp->width) continue;
            vec2 p = (vec2){x, (int)p0.y + i};
            
            vec2 xy_vert_pos[3] = {
                vec3_to_vec2(&vert[0]),
                vec3_to_vec2(&vert[1]),
                vec3_to_vec2(&vert[2]),
            };
            vec3 coords = get_barycentric_coordinate2d(xy_vert_pos, &p);

            double z_buffer_val = coords.x * vert[0].z + coords.y * vert[1].z + coords.z * vert[2].z;
            int not_covered = GAEDisplay_testAndSetZBuffer(disp, p.x, p.y, z_buffer_val);
            if (not_covered == 0) {
                continue;
            }

            color_t color = (color_t){
                coords.x * vert_color[0].r + coords.y * vert_color[1].r + coords.z * vert_color[2].r,
                coords.x * vert_color[0].g + coords.y * vert_color[1].g + coords.z * vert_color[2].g,
                coords.x * vert_color[0].b + coords.y * vert_color[1].b + coords.z * vert_color[2].b,
            };

            int color_int = color_to_int(color);
            GAEDisplay_setPixel(disp, p.x, p.y, color_int);
        }
    }
}

void fill_bottom_flat_triangle(GAEDisplay_t* disp, const vec3 vert[3], const vec2 uv[3], const color_t vert_color[3], const vec3 normals[3], const light_t* light) {
    double inverse_slope1 = (vert[1].x - vert[0].x) / (vert[1].y - vert[0].y);
    double inverse_slope2 = (vert[2].x - vert[0].x) / (vert[2].y - vert[0].y);

    double current_x1 = vert[0].x;
    double current_x2 = vert[0].x;

    for (int y = (int)vert[0].y; y <= (int)vert[1].y; y++) {
        for (int x = (int)current_x1; x <= (int)current_x2; x++) {
            if (x < 0 || x >=disp->width) continue;
            vec2 p = {x, y};
            
            vec2 xy_vert_pos[3] = {
                vec3_to_vec2(&vert[0]),
                vec3_to_vec2(&vert[1]),
                vec3_to_vec2(&vert[2]),
            };
            vec3 coords = get_barycentric_coordinate2d(xy_vert_pos, &p);

            double z_buffer_val = coords.x * vert[0].z + coords.y * vert[1].z + coords.z * vert[2].z;
            int not_covered = GAEDisplay_testAndSetZBuffer(disp, p.x, p.y, z_buffer_val);
            if (not_covered == 0) {
                continue;
            }

            color_t color = (color_t){
                coords.x * vert_color[0].r + coords.y * vert_color[1].r + coords.z * vert_color[2].r,
                coords.x * vert_color[0].g + coords.y * vert_color[1].g + coords.z * vert_color[2].g,
                coords.x * vert_color[0].b + coords.y * vert_color[1].b + coords.z * vert_color[2].b,
            };

            int color_int = color_to_int(color);
            GAEDisplay_setPixel(disp, p.x, p.y, color_int);
        }
        current_x1 += inverse_slope1;
        current_x2 += inverse_slope2;
    }
}

void fill_top_flat_triangle(GAEDisplay_t* disp, const vec3 vert[3], const vec2 uv[3], const color_t vert_color[3], const vec3 normals[3], const light_t* light) {
    double inverse_slope1 = (vert[2].x - vert[0].x) / (vert[2].y - vert[0].y);
    double inverse_slope2 = (vert[2].x - vert[1].x) / (vert[2].y - vert[1].y);

    double current_x1 = vert[2].x;
    double current_x2 = vert[2].x;

    for (int y = (int)vert[2].y; y > (int)vert[0].y; y--) {
        for (int x = (int)current_x1; x <= (int)current_x2; x++) {
            if (x < 0 || x >=disp->width) continue;
            vec2 p = {x, y};
            
            vec2 xy_vert_pos[3] = {
                vec3_to_vec2(&vert[0]),
                vec3_to_vec2(&vert[1]),
                vec3_to_vec2(&vert[2]),
            };
            vec3 coords = get_barycentric_coordinate2d(xy_vert_pos, &p);

            double z_buffer_val = coords.x * vert[0].z + coords.y * vert[1].z + coords.z * vert[2].z;
            int not_covered = GAEDisplay_testAndSetZBuffer(disp, p.x, p.y, z_buffer_val);
            if (not_covered == 0) {
                continue;
            }

            color_t color = (color_t){
                coords.x * vert_color[0].r + coords.y * vert_color[1].r + coords.z * vert_color[2].r,
                coords.x * vert_color[0].g + coords.y * vert_color[1].g + coords.z * vert_color[2].g,
                coords.x * vert_color[0].b + coords.y * vert_color[1].b + coords.z * vert_color[2].b,
            };

            int color_int = color_to_int(color);
            GAEDisplay_setPixel(disp, p.x, p.y, color_int);
        }
        current_x1 -= inverse_slope1;
        current_x2 -= inverse_slope2;
    }
}

void draw_triangle_filled2(GAEDisplay_t* disp, const vec3 vert[3], const vec2 uv[3], const color_t vert_color[3], const vec3 normals[3], const light_t* light) {
    vec3 p0 = {(int)vert[0].x, (int)vert[0].y, vert[0].z};
    vec3 p1 = {(int)vert[1].x, (int)vert[1].y, vert[0].z};
    vec3 p2 = {(int)vert[2].x, (int)vert[2].y, vert[0].z};

    if (p1.y < p0.y) swap_vec3(&p0, &p1);
    if (p2.y < p0.y) swap_vec3(&p2, &p0);
    if (p2.y < p1.y) swap_vec3(&p2, &p1);

    if (p1.y == p2.y) {
        // Bottom-flat triangle
        vec3 vert_bottom[3] = {p0, p1, p2};
        fill_bottom_flat_triangle(disp, vert_bottom, uv, vert_color, normals, light);
    } else if (p0.y == p1.y) {
        // Top-flag triangle
        vec3 vert_top[3] = {p0, p1, p2};
        fill_top_flat_triangle(disp, vert_top, uv, vert_color, normals, light);
    } else {
        vec3 p3 = {
            p0.x + ((p1.y - p0.y) / (p2.y - p0.y)) * (p2.x - p0.x),
            p1.y, 
            1.0
        };

        // Compute berycentric coordinates of input triangle
        vec2 xy_vert_pos[3] = {
            vec3_to_vec2(&vert[0]),
            vec3_to_vec2(&vert[1]),
            vec3_to_vec2(&vert[2]),
        };
        vec2 p3_vec2 = vec3_to_vec2(&p3);
        vec3 coords = get_barycentric_coordinate2d(xy_vert_pos, &p3_vec2);

        // Get attributes for p3
        double p3_z_value = vert[0].z * coords.x + vert[1].z * coords.y + vert[2].z * coords.z;
        p3.z = p3_z_value;

        vec2 p3_uv_coord = {
            uv[0].x * coords.x + uv[1].x * coords.y + uv[2].x * coords.z,
            uv[0].y * coords.x + uv[1].y * coords.y + uv[2].y * coords.z,
        };

        color_t p3_color = {
            (int)(vert_color[0].r * coords.x + vert_color[1].r * coords.y + vert_color[2].r * coords.z),
            (int)(vert_color[0].g * coords.x + vert_color[1].g * coords.y + vert_color[2].g * coords.z),
            (int)(vert_color[0].b * coords.x + vert_color[1].b * coords.y + vert_color[2].b * coords.z),
        };

        vec3 p3_normal = {1, 1, 1};

        // Bottom-flat triangle
        vec3 vert_bottom[3] = {
            p0, p1, p3
        };

        vec2 uv_bottom[3] = {
            uv[0], uv[1], p3_uv_coord
        };

        color_t color_bottom[3] = {
            vert_color[0], vert_color[1], p3_color
        };

        vec3 norm_bottom[3] = {
            normals[0], normals[1], p3_normal
        };

        fill_bottom_flat_triangle(disp, vert_bottom, uv_bottom, color_bottom, norm_bottom, light);
        
        // Top-flat triangle
        vec3 vert_top[3] = {
            p1, p3, p2
        };

        vec2 uv_top[3] = {
            uv[1], p3_uv_coord, uv[2]
        };

        color_t color_top[3] = {
            vert_color[1], p3_color, vert_color[2]
        };

        vec3 norm_top[3] = {
            normals[1], p3_normal, normals[2]
        };

        fill_top_flat_triangle(disp, vert_top, uv_top, color_top, norm_top, light);
    }
}


