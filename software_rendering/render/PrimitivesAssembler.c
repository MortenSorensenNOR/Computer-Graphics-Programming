#include "PrimitivesAssembler.h"

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out) {
    for (int i = 0; i < in->index_buf_size; i += 3) {
        triangle_t* t;
        t->vert[0] = in->vert_buf[in->index_buf[i]];
        t->vert[1] = in->vert_buf[in->index_buf[i+1]];
        t->vert[2] = in->vert_buf[in->index_buf[i+2]];
        t->norm[0] = in->norm_buf[in->index_buf[i]];
        t->norm[1] = in->norm_buf[in->index_buf[i+1]];
        t->norm[2] = in->norm_buf[in->index_buf[i+2]];
        t->frag[0] = in->frag_buf[in->index_buf[i]];
        t->frag[1] = in->frag_buf[in->index_buf[i+1]];
        t->frag[2] = in->frag_buf[in->index_buf[i+2]];
        t->uv[0] = in->uv_buf[in->index_buf[i]];
        t->uv[1] = in->uv_buf[in->index_buf[i+1]];
        t->uv[2] = in->uv_buf[in->index_buf[i+2]];
    }
    return 0;
}
