#include "VertexPostProcessor.h"

int vertex_post_processer(vertex_post_processer_input_t* in, vertex_post_processer_output_t* out) {
    for (int i = 0; i < in->buf_size; ++i) {
        vec3 ndc = (vec3){
            in->pos_buf[i].x / in->pos_buf[i].w, 
            in->pos_buf[i].y / in->pos_buf[i].w, 
            in->pos_buf[i].z / in->pos_buf[i].w, 
        }; 
        out->pos_buf[i] = (vec3){
            (ndc.x + 1.0) * in->s_width * 0.5,
            (1.0 - ndc.y) * in->s_height * 0.5,
            ndc.z
        };
        // Other z value transform ((in->zfar - in->znear) * ndc.z + (in->zfar + in->znear)) * 0.5
    }

    return 0;
}
