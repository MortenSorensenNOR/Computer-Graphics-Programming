#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

#define RASTERIZER_MULTI_THREAD
#ifdef RASTERIZER_MULTI_THREAD
typedef struct {
    int s_width;
    int s_height;

    vec3 cam_pos;
    light_t* light;
    texture_t* textures;


    int batch_start_y;
    int batch_end_y;
    vec2* uv;
    vec3* norm;
    vec3* frag;
    vec3* color;
    float* zbuffer;

    int* framebuffer;
} fragment_shader_thread_argument_t;

void* fragment_shader_thread_func(void* arg_ptr);

#endif 

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
