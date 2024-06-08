#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

typedef struct {
    int s_width;
    int s_height;

    vec3 cam_pos;
    light_t* light;
    texture_t* textures;

    vec2* uv;
    vec3* norm;
    vec3* frag;
    vec3* color;
    float* zbuffer;
} fragment_shader_input_t;

typedef struct {
    int buf_size;
    int* framebuffer;
} fragment_shader_output_t;

int fragment_shader(fragment_shader_input_t* in, fragment_shader_output_t* out);
