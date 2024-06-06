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

    bb_top_left->x = x_min;
    bb_top_left->y = y_min;
    bb_bottom_right->x = x_max;
    bb_bottom_right->y = y_max;
}

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out) {
    float k;
    for (int i = 0; i < out->tri_buf_size; ++i) {
        int ibuf_off = 3*i;
        triangle_t* t = &out->tri_buf[i];
        t->vert[0] = in->vert_buf[in->index_buf[ibuf_off]];
        t->vert[1] = in->vert_buf[in->index_buf[ibuf_off+1]];
        t->vert[2] = in->vert_buf[in->index_buf[ibuf_off+2]];

        t->frag[0] = in->frag_buf[in->index_buf[ibuf_off]];
        t->frag[1] = in->frag_buf[in->index_buf[ibuf_off+1]];
        t->frag[2] = in->frag_buf[in->index_buf[ibuf_off+2]];

        t->uv[0] = in->uv_buf[in->index_buf[ibuf_off]];
        t->uv[1] = in->uv_buf[in->index_buf[ibuf_off+1]];
        t->uv[2] = in->uv_buf[in->index_buf[ibuf_off+2]];

        // printf("%f %f, %f %f, %f %f\n", t->uv[0].x, t->uv[0].y, t->uv[1].x, t->uv[1].y, t->uv[2].x, t->uv[2].y);

        t->color[0] = (vec3){1.0f, 0.0f, 0.0f};
        t->color[1] = (vec3){0.0f, 1.0f, 0.0f};
        t->color[2] = (vec3){0.0f, 0.0f, 1.0f};

        t->norm = in->norm_buf[in->index_buf[ibuf_off]];
        pa_calculate_bounding_box(t->vert, &t->bb_top_left, &t->bb_bottom_right);
        t->valid = 1;
    }
    return 0;
}
