#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

typedef struct {
    mat4* view_proj;    
    mat4* model;

    int buf_size;
    vec4* vertex_buf;
    vec3* normal_buf;
} vertex_shader_input_t;

typedef struct {
    vec3* frag_buf;
    vec3* normal_buf;
    vec4* pos_buf;
} vertex_shader_output_t;

int vertex_shader(vertex_shader_input_t* in, vertex_shader_output_t* out);
