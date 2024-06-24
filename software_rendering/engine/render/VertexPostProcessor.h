#pragma once
#include "../gml/GML.h"
#include "../utilities/RenderUtil.h"

typedef struct {
    int s_width;
    int s_height;
    float znear;
    float zfar;

    int buf_size;
    vec4* pos_buf;
} vertex_post_processer_input_t;

typedef struct {
    int buf_size;
    vec3* pos_buf;
} vertex_post_processer_output_t;

int vertex_post_processer(vertex_post_processer_input_t* in, vertex_post_processer_output_t* out);
