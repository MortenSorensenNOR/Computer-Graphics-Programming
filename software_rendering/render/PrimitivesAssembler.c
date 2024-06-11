#include "PrimitivesAssembler.h"

static inline void pa_calculate_bounding_box(vec3* vert, vec2* bb_top_left, vec2* bb_bottom_right) {
    float x_min = min(vert[0].x, vert[1].x);
    x_min = min(x_min, vert[2].x);

    float x_max = max(vert[0].x, vert[1].x);
    x_max = max(x_max, vert[2].x);

    float y_min = min(vert[0].y, vert[1].y);
    y_min = min(y_min, vert[2].y);

    float y_max = max(vert[0].y, vert[1].y);
    y_max = max(y_max, vert[2].y);

    bb_top_left->x = max(x_min, 0);
    bb_top_left->y = max(y_min, 0);
    bb_bottom_right->x = max(x_max, 0);
    bb_bottom_right->y = max(y_max, 0);
}

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out) {
    for (int i = 0; i < out->tri_buf_size; ++i) {
        int ibuf_off = 3*i;
        triangle_t* t = &out->tri_buf[i];

        // Culling
        vec3 n = in->norm_buf[in->index_buf[ibuf_off]]; 
        vec3 a = in->frag_buf[in->index_buf[ibuf_off]];
        vec3 b = in->frag_buf[in->index_buf[ibuf_off]];
        vec3 c = in->frag_buf[in->index_buf[ibuf_off]];

        vec3 v0 = {(a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f, (a.z + b.z + c.z) / 3.0f};
        vec3 cam_to_pos_vec = vec3_sub(&in->cam_pos, &v0);
        float dp = vec3_dot(&cam_to_pos_vec, &n);

        if (dp <= 0) {
            t->valid = 0;
            continue;
        }
        t->valid = 1;

        // Set attribute data
        t->norm = n;

        t->vert[0] = in->vert_buf[in->index_buf[ibuf_off]];
        t->vert[1] = in->vert_buf[in->index_buf[ibuf_off+1]];
        t->vert[2] = in->vert_buf[in->index_buf[ibuf_off+2]];

        t->frag[0] = in->frag_buf[in->index_buf[ibuf_off]];
        t->frag[1] = in->frag_buf[in->index_buf[ibuf_off+1]];
        t->frag[2] = in->frag_buf[in->index_buf[ibuf_off+2]];

        t->uv[0] = in->uv_buf[in->index_buf[ibuf_off]];
        t->uv[1] = in->uv_buf[in->index_buf[ibuf_off+1]];
        t->uv[2] = in->uv_buf[in->index_buf[ibuf_off+2]];

        t->color[0] = (vec3){0.96862745098, 0.45490196078, 0.41568627451};
        t->color[1] = (vec3){0.96862745098, 0.45490196078, 0.41568627451};
        t->color[2] = (vec3){0.96862745098, 0.45490196078, 0.41568627451};

        pa_calculate_bounding_box(t->vert, &t->bb_top_left, &t->bb_bottom_right);
    }

    return 0;
}
