#include "Rasterizer.h"

static inline void ra_barycentric(vec2* v0, vec2* v1, vec2* v2, vec2* p, vec3* bc) {
    vec2 v10 = vec2_sub(v1, v0);
    vec2 v21 = vec2_sub(v2, v0);
    vec2 vp0 = vec2_sub(p, v0);

    double d00 = vec2_dot(&v10, &v10);
    double d01 = vec2_dot(&v10, &v21);
    double d11 = vec2_dot(&v21, &v21);
    double d20 = vec2_dot(&vp0, &v10);
    double d21 = vec2_dot(&vp0, &v21);

    double det = (d00 * d11 - d01 * d01);
    bc->y = (d11 * d20 - d01 * d21) / det;
    bc->z = (d00 * d21 - d01 * d20) / det;
    bc->x = 1.0f - bc->y - bc->z;
}


#ifdef RASTERIZER_MULTI_THREAD
void* rasterizer_thread_func(void* arg_ptr) {
    triangle_t* t;
    vec2 v0, v1, v2, p;
    vec3 bc;
    int index;

    int x_min, x_max, y_min, y_max;
    rasterizer_thread_argument_t* thread_arg = (rasterizer_thread_argument_t*)arg_ptr;
    for (t = thread_arg->in_tri_buf; t < &thread_arg->in_tri_buf[thread_arg->in_tri_buf_size]; ++t) {
        if (!t->valid)
            continue;

        x_min = max(t->bb_top_left.x, 0);
        x_max = min(t->bb_bottom_right.x, thread_arg->s_width);
        y_min = max(t->bb_top_left.y, thread_arg->out_batch_start_y);
        y_max = min(t->bb_bottom_right.y, thread_arg->out_batch_end_y);

        for (int y = y_min; y <= y_max; ++y) {
            for (int x = x_min; x <= x_max; ++x) {
                p = (vec2){x, y};
                v0 = (vec2){t->vert[0].x, t->vert[0].y};
                v1 = (vec2){t->vert[1].x, t->vert[1].y};
                v2 = (vec2){t->vert[2].x, t->vert[2].y};
                ra_barycentric(&v0, &v1, &v2, &p, &bc);
                
                if (bc.x < 0.0f || bc.y < 0.0f || bc.z < 0.0f)
                   continue; 

                // Calculate z value and test z-buffer
                float z = t->vert[0].z * bc.x + t->vert[1].z * bc.y + t->vert[2].z * bc.z;    
                index = x + y * thread_arg->s_width;

                if (!(z < thread_arg->out_zbuffer[index]))
                    continue;

                thread_arg->out_zbuffer[index] = z;
                thread_arg->out_uv[index] = (vec2){
                    t->uv[0].x * bc.x + t->uv[1].x * bc.y + t->uv[2].x * bc.z,
                    t->uv[0].y * bc.x + t->uv[1].y * bc.y + t->uv[2].y * bc.z
                };
                thread_arg->out_frag[index] = (vec3){
                    t->frag[0].x * bc.x + t->frag[1].x * bc.y + t->frag[2].x * bc.z,
                    t->frag[0].y * bc.x + t->frag[1].y * bc.y + t->frag[2].y * bc.z,
                    t->frag[0].z * bc.x + t->frag[1].z * bc.y + t->frag[2].z * bc.z,
                };
                thread_arg->out_color[index] = (vec3){
                    t->color[0].x * bc.x + t->color[1].x * bc.y + t->color[2].x * bc.z,
                    t->color[0].y * bc.x + t->color[1].y * bc.y + t->color[2].y * bc.z,
                    t->color[0].z * bc.x + t->color[1].z * bc.y + t->color[2].z * bc.z,
                };
                thread_arg->out_norm[index] = t->norm;

            }
        }
    }

    return NULL;
}
#endif

int rasterizer(rasterizer_input_t* in, rasterizer_output_t* out) {
    #ifdef RASTERIZER_MULTI_THREAD

    int num_threads = 4;
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    rasterizer_thread_argument_t* thread_args = (rasterizer_thread_argument_t*)malloc(num_threads * sizeof(rasterizer_thread_argument_t));
    
    int batch_size_y = in->s_height / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].s_width = in->s_width;
        thread_args[i].s_height = in->s_height;

        thread_args[i].in_tri_buf = in->tri_buf;
        thread_args[i].in_tri_buf_size = in->tri_buf_size;

        thread_args[i].out_batch_start_y = i * batch_size_y;
        thread_args[i].out_batch_end_y = i * batch_size_y + batch_size_y;

        thread_args[i].out_uv = out->uv;
        thread_args[i].out_norm = out->norm;
        thread_args[i].out_frag = out->frag;
        thread_args[i].out_color = out->color;
        thread_args[i].out_zbuffer = out->zbuffer;

        pthread_create(&threads[i], NULL, rasterizer_thread_func, (void*)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
 
    #else 
    triangle_t* t;
    vec2 v0, v1, v2, p;
    vec3 bc;             
    int index;

    for (t = in->tri_buf; t < &in->tri_buf[in->tri_buf_size]; t++) {
        if (!t->valid)
            continue;

        for (int y = max(t->bb_top_left.y, 0); y < min(t->bb_bottom_right.y, in->s_height); y++) {
            for (int x = max(t->bb_top_left.x, 0); x < min(t->bb_bottom_right.x, in->s_width); x++) {
                p = (vec2){x, y};
                v0 = (vec2){t->vert[0].x, t->vert[0].y};
                v1 = (vec2){t->vert[1].x, t->vert[1].y};
                v2 = (vec2){t->vert[2].x, t->vert[2].y};
                ra_barycentric(&v0, &v1, &v2, &p, &bc);

                if (bc.x < 0.0f || bc.y < 0.0f || bc.z < 0.0f)
                    continue; 

                // Calculate z value and test z-buffer
                float z = t->vert[0].z * bc.x + t->vert[1].z * bc.y + t->vert[2].z * bc.z;    
                index = x + y * in->s_width;

                if (!(z < out->zbuffer[index]))
                    continue;

                // Set output buffer attributes
                out->zbuffer[index] = z;
                out->uv[index] = (vec2){
                    t->uv[0].x * bc.x + t->uv[1].x * bc.y + t->uv[2].x * bc.z,
                        t->uv[0].y * bc.x + t->uv[1].y * bc.y + t->uv[2].y * bc.z
                };
                out->frag[index] = (vec3){
                    t->frag[0].x * bc.x + t->frag[1].x * bc.y + t->frag[2].x * bc.z,
                        t->frag[0].y * bc.x + t->frag[1].y * bc.y + t->frag[2].y * bc.z,
                        t->frag[0].z * bc.x + t->frag[1].z * bc.y + t->frag[2].z * bc.z,
                };
                out->color[index] = (vec3){
                    t->color[0].x * bc.x + t->color[1].x * bc.y + t->color[2].x * bc.z,
                        t->color[0].y * bc.x + t->color[1].y * bc.y + t->color[2].y * bc.z,
                        t->color[0].z * bc.x + t->color[1].z * bc.y + t->color[2].z * bc.z,
                };
                out->norm[index] = t->norm;
            }
        } 
    }
    #endif
    return 0;
}
