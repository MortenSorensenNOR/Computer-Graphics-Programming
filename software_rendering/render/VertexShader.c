#include "VertexShader.h"

int vertex_shader(vertex_shader_input_t* in, vertex_shader_output_t* out) {
    mat4 inv_model;
    mat4 trans_inv_model;
    mat3 trans_inv_model_3;
    mat4_inverse(in->model, &inv_model);
    mat4_transpose(&inv_model, &trans_inv_model);
    mat4_to_mat3(&trans_inv_model, &trans_inv_model_3);

    for (int i = 0; i < in->buf_size; ++i) {
        vec4 global_vert_pos = mat4_vec4_mul(in->model, &in->vertex_buf[i]);
        out->frag_buf[i] = vec4_to_vec3(&global_vert_pos);
        out->pos_buf[i] = mat4_vec4_mul(in->view_proj, &global_vert_pos);

        vec3 trasformed_norm = mat3_vec3_mul(&trans_inv_model_3, &in->normal_buf[i]); 
        out->normal_buf[i] = vec3_normalize(&trasformed_norm);
    }

    return 0;
}
