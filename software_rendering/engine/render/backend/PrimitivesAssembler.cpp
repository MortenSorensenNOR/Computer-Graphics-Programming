#include "PrimitivesAssembler.h"

static inline void pa_calculate_bounding_box(glm::vec3* vert, glm::vec2* bb_top_left, glm::vec2* bb_bottom_right) {
    float x_min = fmin(vert[0].x, vert[1].x);
    x_min = fmin(x_min, vert[2].x);

    float x_max = fmax(vert[0].x, vert[1].x);
    x_max = fmax(x_max, vert[2].x);

    float y_min = fmin(vert[0].y, vert[1].y);
    y_min = fmin(y_min, vert[2].y);

    float y_max = fmax(vert[0].y, vert[1].y);
    y_max = fmax(y_max, vert[2].y);

    bb_top_left->x = fmax(x_min, 0);
    bb_top_left->y = fmax(y_min, 0);
    bb_bottom_right->x = fmax(x_max, 0);
    bb_bottom_right->y = fmax(y_max, 0);
}

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out) {
    for (int i = 0; i < out->tri_buf_size; ++i) {
        int ibuf_off = 3*i;
        triangle_t* t = &out->tri_buf[i];

        // Culling
        glm::vec3 n = in->norm_buf[in->index_buf[ibuf_off]]; 
        glm::vec3 a = in->frag_buf[in->index_buf[ibuf_off]];
        glm::vec3 b = in->frag_buf[in->index_buf[ibuf_off]];
        glm::vec3 c = in->frag_buf[in->index_buf[ibuf_off]];

        glm::vec3 v0 = {(a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f, (a.z + b.z + c.z) / 3.0f};
        glm::vec3 cam_to_pos_vec = v0 - in->cam_pos;
        float dp = glm::dot(cam_to_pos_vec, n);

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

        t->color[0] = (glm::vec3){0.96862745098, 0.45490196078, 0.41568627451};
        t->color[1] = (glm::vec3){0.96862745098, 0.45490196078, 0.41568627451};
        t->color[2] = (glm::vec3){0.96862745098, 0.45490196078, 0.41568627451};

        pa_calculate_bounding_box(t->vert, &t->bb_top_left, &t->bb_bottom_right);
    }

    return 0;
}
