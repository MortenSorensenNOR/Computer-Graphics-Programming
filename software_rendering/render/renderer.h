#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

#include "VertexShader.h"
#include "VertexPostProcessor.h"
#include "Rasterizer.h"
#include "FragmentShader.h"

extern int render_frame_buffer_width;
extern int render_frame_buffer_height;
extern int* render_frame_buffer;

int renderer_initialize();

int renderer_render(int s_width, int s_height, mat4* viewmatrix, scene_t* scenes);

int renderer_destroy();
