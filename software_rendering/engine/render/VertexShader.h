#pragma once
#include "../gml/GML.h"
#include "../utilities/RenderUtil.h"
#include <pthread.h>

#define VERTEX_SHADER_MULTI_THREAD
#ifdef VERTEX_SHADER_MULTI_THREAD
typedef struct {
    mat4* model;
    mat4* proj;
    mat3* inv_moodel_trans;

    int batch_start;
    int batch_end;
    vec4* in_vertex_buf;
    vec3* in_normal_buf;

    vec3* out_frag_buf;
    vec3* out_normal_buf;
    vec4* out_pos_buf;
} vertex_shader_thread_argument_t;

void* vertex_shader_thread_func(void* arg_ptr);
#endif

typedef struct {
    mat4* view_proj;    
    mat4* model;

    int buf_size;
    vec4* vertex_buf;
    vec3* normal_buf;
} vertex_shader_input_t;

typedef struct {
    int buf_size;
    vec3* frag_buf;
    vec3* normal_buf;
    vec4* pos_buf;
} vertex_shader_output_t;

int vertex_shader(vertex_shader_input_t* in, vertex_shader_output_t* out);
