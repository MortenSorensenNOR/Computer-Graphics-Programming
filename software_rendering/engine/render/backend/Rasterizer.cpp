#include "Rasterizer.h"

static inline void ra_barycentric(glm::vec2* v0, glm::vec2* v1, glm::vec2* v2, glm::vec2* p, glm::vec3* bc) {
    glm::vec2 v10 = {v1->x - v0->x, v1->y - v0->y};
    glm::vec2 v21 = {v2->x - v0->x, v2->y - v0->y};
    glm::vec2 vp0 = {p->x - v0->x, p->y - v0->y};

    double d00 = v10.x * v10.x + v10.y * v10.y;
    double d01 = v10.x * v21.x + v10.y * v21.y;
    double d11 = v21.x * v21.x + v21.y * v21.y;
    double d20 = vp0.x * v10.x + vp0.y * v10.y;
    double d21 = vp0.x * v21.x + vp0.y * v21.y;

    double det = (d00 * d11 - d01 * d01);
    bc->y = (d11 * d20 - d01 * d21) / det;
    bc->z = (d00 * d21 - d01 * d20) / det;
    bc->x = 1.0f - bc->y - bc->z;
}


#ifdef RASTERIZER_MULTI_THREAD
void* rasterizer_thread_func(void* arg_ptr) {
    triangle_t* t;
    glm::vec2 v0, v1, v2, p;
    glm::vec3 bc;
    int index;

    int x_min, x_max, y_min, y_max;
    rasterizer_thread_argument_t* thread_arg = (rasterizer_thread_argument_t*)arg_ptr;
    for (t = thread_arg->in_tri_buf; t < &thread_arg->in_tri_buf[thread_arg->in_tri_buf_size]; ++t) {
        if (!t->valid)
            continue;

        x_min = fmax(t->bb_top_left.x, 0);
        x_max = fmin(t->bb_bottom_right.x, thread_arg->s_width);
        y_min = fmax(t->bb_top_left.y, thread_arg->out_batch_start_y);
        y_max = fmin(t->bb_bottom_right.y, thread_arg->out_batch_end_y);

        for (int y = y_min; y <= y_max; ++y) {
            for (int x = x_min; x <= x_max; ++x) {
                p = (glm::vec2){x, y};
                v0 = (glm::vec2){t->vert[0].x, t->vert[0].y};
                v1 = (glm::vec2){t->vert[1].x, t->vert[1].y};
                v2 = (glm::vec2){t->vert[2].x, t->vert[2].y};
                ra_barycentric(&v0, &v1, &v2, &p, &bc);
                
                if (bc.x < 0.0f || bc.y < 0.0f || bc.z < 0.0f)
                   continue; 

                // Calculate z value and test z-buffer
                float z = t->vert[0].z * bc.x + t->vert[1].z * bc.y + t->vert[2].z * bc.z;    
                index = x + y * thread_arg->s_width;

                if (!(z < thread_arg->out_zbuffer[index]))
                    continue;

                thread_arg->out_zbuffer[index] = z;
                thread_arg->out_uv[index] = (glm::vec2){
                    t->uv[0].x * bc.x + t->uv[1].x * bc.y + t->uv[2].x * bc.z,
                    t->uv[0].y * bc.x + t->uv[1].y * bc.y + t->uv[2].y * bc.z
                };
                thread_arg->out_frag[index] = (glm::vec3){
                    t->frag[0].x * bc.x + t->frag[1].x * bc.y + t->frag[2].x * bc.z,
                    t->frag[0].y * bc.x + t->frag[1].y * bc.y + t->frag[2].y * bc.z,
                    t->frag[0].z * bc.x + t->frag[1].z * bc.y + t->frag[2].z * bc.z,
                };
                thread_arg->out_color[index] = (glm::vec3){
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

    int num_threads = 2;
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
    glm::vec2 v0, v1, v2, p;
    glm::vec3 bc;             
    int index;

    for (t = in->tri_buf; t < &in->tri_buf[in->tri_buf_size]; t++) {
        if (!t->valid)
            continue;

        for (int y = fmax(t->bb_top_left.y, 0); y < fmin(t->bb_bottom_right.y, in->s_height); y++) {
            for (int x = fmax(t->bb_top_left.x, 0); x < fmin(t->bb_bottom_right.x, in->s_width); x++) {
                p = (glm::vec2){x, y};
                v0 = (glm::vec2){t->vert[0].x, t->vert[0].y};
                v1 = (glm::vec2){t->vert[1].x, t->vert[1].y};
                v2 = (glm::vec2){t->vert[2].x, t->vert[2].y};
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
                out->uv[index] = (glm::vec2){
                    t->uv[0].x * bc.x + t->uv[1].x * bc.y + t->uv[2].x * bc.z,
                        t->uv[0].y * bc.x + t->uv[1].y * bc.y + t->uv[2].y * bc.z
                };
                out->frag[index] = (glm::vec3){
                    t->frag[0].x * bc.x + t->frag[1].x * bc.y + t->frag[2].x * bc.z,
                        t->frag[0].y * bc.x + t->frag[1].y * bc.y + t->frag[2].y * bc.z,
                        t->frag[0].z * bc.x + t->frag[1].z * bc.y + t->frag[2].z * bc.z,
                };
                out->color[index] = (glm::vec3){
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
