#include "PrimitivesAssembler.h"

int primitives_assembler(primitives_assembler_input_t* in, primitives_assembler_output_t* out) {
    for (int i = 0; i < out->tri_buf_size; ++i) {
        int ibuf_off = 3*i;
        triangle_t* t = &out->tri_buf[i];
        t->vert[0] = in->vert_buf[in->index_buf[ibuf_off]];
        t->vert[1] = in->vert_buf[in->index_buf[ibuf_off+1]];
        t->vert[2] = in->vert_buf[in->index_buf[ibuf_off+2]];
        t->norm[0] = in->norm_buf[in->index_buf[ibuf_off]];
        t->norm[1] = in->norm_buf[in->index_buf[ibuf_off+1]];
        t->norm[2] = in->norm_buf[in->index_buf[ibuf_off+2]];
        t->frag[0] = in->frag_buf[in->index_buf[ibuf_off]];
        t->frag[1] = in->frag_buf[in->index_buf[ibuf_off+1]];
        t->frag[2] = in->frag_buf[in->index_buf[ibuf_off+2]];
        t->uv[0] = in->uv_buf[in->index_buf[ibuf_off]];
        t->uv[1] = in->uv_buf[in->index_buf[ibuf_off+1]];
        t->uv[2] = in->uv_buf[in->index_buf[ibuf_off+2]];
    }
    return 0;
}
