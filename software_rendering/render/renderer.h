#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

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

    /* vertex_shader_output_t* vs_out; */
    /* vertex_post_processer_output_t* vpp_out; */
    /* primitives_assembler_output_t* pa_out; */
    /* rasterizer_output_t* rast_out; */
    /* fragement_shader_output_t* fs_out; */
} render_t;

int renderer_init(render_t* renderer, int s_width, int s_height, float zfar, float znear);

int renderer_reset_buffers(render_t* renderer);

int renderer_render(render_t* renderer, mat4* view_proj, mat4* model, render_object_t* object, light_t* lights);

int renderer_destroy(render_t* renderer);
