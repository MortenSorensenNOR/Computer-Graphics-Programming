#pragma once
#include "../../utilities/Types.h"
#include <pthread.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#define RASTERIZER_MULTI_THREAD
#ifdef RASTERIZER_MULTI_THREAD
typedef struct {
    int s_width;
    int s_height;

    glm::vec3 cam_pos;
    light_t* light;
    texture_t* textures;


    int batch_start_y;
    int batch_end_y;
    glm::vec2* uv;
    glm::vec3* norm;
    glm::vec3* frag;
    glm::vec3* color;
    float* zbuffer;

    int* framebuffer;
} fragment_shader_thread_argument_t;

void* fragment_shader_thread_func(void* arg_ptr);

#endif 

typedef struct {
    int s_width;
    int s_height;

    glm::vec3 cam_pos;
    light_t* light;
    texture_t* textures;

    glm::vec2* uv;
    glm::vec3* norm;
    glm::vec3* frag;
    glm::vec3* color;
    float* zbuffer;
} fragment_shader_input_t;

typedef struct {
    int buf_size;
    int* framebuffer;
} fragment_shader_output_t;

int fragment_shader(fragment_shader_input_t* in, fragment_shader_output_t* out);
