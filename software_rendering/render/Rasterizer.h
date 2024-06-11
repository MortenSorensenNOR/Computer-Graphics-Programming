#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

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
    vec2* out_uv;
    vec3* out_norm;
    vec3* out_frag;
    vec3* out_color;
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
    vec2* uv;
    vec3* norm;
    vec3* frag;
    vec3* color;
    float* zbuffer;
} rasterizer_output_t;

static inline void ra_barycentric(vec2* v0, vec2* v1, vec2* v2, vec2* p, vec3* bc);

int rasterizer(rasterizer_input_t* in, rasterizer_output_t* out);
