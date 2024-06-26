#pragma once
#include <pthread.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include "../utilities/Types.h"

#define VERTEX_SHADER_MULTI_THREAD
#ifdef VERTEX_SHADER_MULTI_THREAD
typedef struct {
    glm::mat4* model;
    glm::mat4* proj;
    glm::mat3* inv_moodel_trans;

    int batch_start;
    int batch_end;
    glm::vec4* in_vertex_buf;
    glm::vec3* in_normal_buf;

    glm::vec3* out_frag_buf;
    glm::vec3* out_normal_buf;
    glm::vec4* out_pos_buf;
} vertex_shader_thread_argument_t;

void* vertex_shader_thread_func(void* arg_ptr);
#endif

typedef struct {
    glm::mat4* view_proj;    
    glm::mat4* model;

    int buf_size;
    glm::vec4* vertex_buf;
    glm::vec3* normal_buf;
} vertex_shader_input_t;

typedef struct {
    int buf_size;
    glm::vec3* frag_buf;
    glm::vec3* normal_buf;
    glm::vec4* pos_buf;
} vertex_shader_output_t;

int vertex_shader(vertex_shader_input_t* in, vertex_shader_output_t* out);
