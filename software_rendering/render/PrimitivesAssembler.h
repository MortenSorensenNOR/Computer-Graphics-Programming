#pragma once
#include "../gml/GML.h"
#include "../rutil/RenderUtil.h"

typedef struct {
    int vert_buf_size;
    int index_buf_size;

    vec3* vert_buf;
    vec3* norm_buf;
    vec3* frag_buf;
    vec2* uv_buf;
    unsigned int* index_buf;
} primitives_assembler_input_t;

typedef struct {
    int tri_buf_size;
    triangle_t* tri_buf;
} primitives_assembler_output_t;

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out);
