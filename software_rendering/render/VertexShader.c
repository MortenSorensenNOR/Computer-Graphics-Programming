#include "VertexShader.h"

#ifdef VERTEX_SHADER_MULTI_THREAD
void* vertex_shader_thread_func(void* arg_ptr) {
    vertex_shader_thread_argument_t* thread_arg = (vertex_shader_thread_argument_t*)arg_ptr;
    for (int i = thread_arg->batch_start; i < thread_arg->batch_end; i++) {
        vec4 global_vert_pos = mat4_vec4_mul(thread_arg->model, &thread_arg->in_vertex_buf[i]);
        thread_arg->out_frag_buf[i] = vec4_to_vec3(&global_vert_pos);
        thread_arg->out_pos_buf[i] = mat4_vec4_mul(thread_arg->proj, &global_vert_pos);

        vec3 trasformed_norm = mat3_vec3_mul(thread_arg->inv_moodel_trans, &thread_arg->in_normal_buf[i]); 
        thread_arg->out_normal_buf[i] = vec3_normalize(&trasformed_norm);
    } 
    return NULL;
}
#endif


int vertex_shader(vertex_shader_input_t* in, vertex_shader_output_t* out) {
    mat4 inv_model;
    mat4 trans_inv_model;
    mat3 trans_inv_model_3;
    mat4_inverse(in->model, &inv_model);
    mat4_transpose(&inv_model, &trans_inv_model);
    mat4_to_mat3(&trans_inv_model, &trans_inv_model_3);

    #ifdef VERTEX_SHADER_MULTI_THREAD
    int num_threads = 2;
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    vertex_shader_thread_argument_t* thread_args = (vertex_shader_thread_argument_t*)malloc(num_threads * sizeof(vertex_shader_thread_argument_t));

    int batch_size = in->buf_size / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].model = in->model;
        thread_args[i].proj = in->view_proj;
        thread_args[i].inv_moodel_trans = &trans_inv_model_3;

        thread_args[i].batch_start = i * batch_size;
        thread_args[i].batch_end = i * batch_size + batch_size;
        thread_args[i].in_vertex_buf = in->vertex_buf;
        thread_args[i].in_normal_buf = in->normal_buf;

        thread_args[i].out_frag_buf = out->frag_buf;
        thread_args[i].out_normal_buf = out->normal_buf;
        thread_args[i].out_pos_buf = out->pos_buf;

        pthread_create(&threads[i], NULL, vertex_shader_thread_func, (void*)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Make sure all verticies are transformed
    for (int i = batch_size * num_threads; i < in->buf_size; ++i) {
        vec4 global_vert_pos = mat4_vec4_mul(in->model, &in->vertex_buf[i]);
        out->frag_buf[i] = vec4_to_vec3(&global_vert_pos);
        out->pos_buf[i] = mat4_vec4_mul(in->view_proj, &global_vert_pos);

        vec3 trasformed_norm = mat3_vec3_mul(&trans_inv_model_3, &in->normal_buf[i]); 
        out->normal_buf[i] = vec3_normalize(&trasformed_norm);
    }

    #else
    for (int i = 0; i < in->buf_size; ++i) {
        vec4 global_vert_pos = mat4_vec4_mul(in->model, &in->vertex_buf[i]);
        out->frag_buf[i] = vec4_to_vec3(&global_vert_pos);
        out->pos_buf[i] = mat4_vec4_mul(in->view_proj, &global_vert_pos);

        vec3 trasformed_norm = mat3_vec3_mul(&trans_inv_model_3, &in->normal_buf[i]); 
        out->normal_buf[i] = vec3_normalize(&trasformed_norm);
    }
    #endif

    return 0;
}
