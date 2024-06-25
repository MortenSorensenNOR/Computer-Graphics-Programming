#pragma once
#include "../utilities/Types.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <pthread.h>

#define RASTERIZER_MULTI_THREAD
#ifdef RASTERIZER_MULTI_THREAD

// Assumes threads split work horizontally
typedef struct {
    int s_width;
    int s_height;

    int in_tri_buf_size;
    triangle_t* in_tri_buf;

    int out_batch_start_y;
    int out_batch_end_y;
    glm::vec2* out_uv;
    glm::vec3* out_norm;
    glm::vec3* out_frag;
    glm::vec3* out_color;
    float* out_zbuffer;
} rasterizer_thread_argument_t;

void* rasterizer_thread_func(void* arg_ptr);
#endif

typedef struct {
    int s_width; 
    int s_height;

    int tri_buf_size;
    triangle_t* tri_buf;
} rasterizer_input_t;

typedef struct {
    int buf_size;
    glm::vec2* uv;
    glm::vec3* norm;
    glm::vec3* frag;
    glm::vec3* color;
    float* zbuffer;
} rasterizer_output_t;

static inline void ra_barycentric(glm::vec2* v0, glm::vec2* v1, glm::vec2* v2, glm::vec2* p, glm::vec3* bc);

int rasterizer(rasterizer_input_t* in, rasterizer_output_t* out);
