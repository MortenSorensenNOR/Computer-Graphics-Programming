#pragma once
#include "../gml/GML.h"

#include "VertexShader.h"
#include "VertexPostProcessor.h"
#include "PrimitivesAssembler.h"
#include "Rasterizer.h"
#include "FragmentShader.h"

typedef struct {
    int s_width;
    int s_height;
    float zfar;
    float znear;

    int* frame_buffer;
    float* zbuffer;
} Render_t;

int renderer_init(Render_t* renderer, int s_width, int s_height, float zfar, float znear);

int renderer_reset_buffers(Render_t* renderer);

int renderer_render(Render_t* renderer, vec3 cam_pos, mat4* view_proj, mat4* model, render_object_t* object, light_t* lights);

int renderer_destroy(Render_t* renderer);
