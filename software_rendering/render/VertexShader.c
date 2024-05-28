#include "VertexShader.h"

int vertex_shader(vertex_shader_input_t* in, vertex_shader_output_t* out) {
    for (int i = 0; i < in->buf_size; ++i) {
        vec4 global_vert_pos = mat4_vec4_mul(in->model, &in->vertex_buf[i]);
        out->frag_buf[i] = vec4_to_vec3(&global_vert_pos);
        out->pos_buf[i] = mat4_vec4_mul(in->view_proj, &global_vert_pos);
        // TODO: calc normal: transpose(inverse(moedl)) * in->normal_buf[i]
    }

    return 0;
}
