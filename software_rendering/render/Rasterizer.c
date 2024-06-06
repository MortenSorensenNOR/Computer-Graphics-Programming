#include "Rasterizer.h"

static inline void ra_barycentric(vec2* v0, vec2* v1, vec2* v2, vec2* p, vec3* bc) {
    vec2 v10 = vec2_sub(v1, v0);
    vec2 v21 = vec2_sub(v2, v0);
    vec2 vp0 = vec2_sub(p, v0);

    double d00 = vec2_dot(&v10, &v10);
    double d01 = vec2_dot(&v10, &v21);
    double d11 = vec2_dot(&v21, &v21);
    double d20 = vec2_dot(&vp0, &v10);
    double d21 = vec2_dot(&vp0, &v21);

    double det = (d00 * d11 - d01 * d01);
    bc->y = (d11 * d20 - d01 * d21) / det;
    bc->z = (d00 * d21 - d01 * d20) / det;
    bc->x = 1.0f - bc->y - bc->z;
}

int rasterizer(rasterizer_input_t* in, rasterizer_output_t* out) {
    triangle_t* t;
    vec2 v0, v1, v2, p;
    vec3 bc;             
    int index;

    for (t = in->tri_buf; t < &in->tri_buf[in->tri_buf_size]; t++) {
        if (!t->valid)
            continue;

        for (int y = t->bb_top_left.y; y < t->bb_bottom_right.y; y++) {
            for (int x = t->bb_top_left.x; x < t->bb_bottom_right.x; x++) {
                p = (vec2){x, y};
                v0 = (vec2){t->vert[0].x, t->vert[0].y};
                v1 = (vec2){t->vert[1].x, t->vert[1].y};
                v2 = (vec2){t->vert[2].x, t->vert[2].y};
                ra_barycentric(&v0, &v1, &v2, &p, &bc);
                
                if (bc.x < 0.0f || bc.y < 0.0f || bc.z < 0.0f)
                   continue; 

                // Calculate z value and test z-buffer
                float z = t->vert[0].z * bc.x + t->vert[1].z * bc.y + t->vert[2].z * bc.z;    
                index = x + y * in->s_width;

                if (!(z < out->zbuffer[index]))
                    continue;
                
                // Set output buffer attributes
                out->zbuffer[index] = z;
                out->uv[index] = (vec2){
                    t->uv[0].x * bc.x + t->uv[1].x * bc.y + t->uv[2].x * bc.z,
                    t->uv[0].y * bc.x + t->uv[1].y * bc.y + t->uv[2].y * bc.z
                };
                out->frag[index] = (vec3){
                    t->frag[0].x * bc.x + t->frag[1].x * bc.y + t->frag[2].x * bc.z,
                    t->frag[0].y * bc.x + t->frag[1].y * bc.y + t->frag[2].y * bc.z,
                    t->frag[0].z * bc.x + t->frag[1].z * bc.y + t->frag[2].z * bc.z,
                };
                out->color[index] = (vec3){
                    t->color[0].x * bc.x + t->color[1].x * bc.y + t->color[2].x * bc.z,
                    t->color[0].y * bc.x + t->color[1].y * bc.y + t->color[2].y * bc.z,
                    t->color[0].z * bc.x + t->color[1].z * bc.y + t->color[2].z * bc.z,
                };
                out->norm[index] = t->norm;
            }
        } 
    }
    return 0;
}
