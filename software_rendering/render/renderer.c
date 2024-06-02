#include "renderer.h"

int renderer_init(int s_width, int s_height, float zfar, float znear, render_t* renderer) {
    renderer->s_width = s_width;
    renderer->s_height = s_height;

    renderer->frame_buffer = (int*)malloc(s_width * s_height * sizeof(int));
    renderer->zbuffer = (float*)malloc(s_width * s_height * sizeof(float));

    renderer->vs_out = (vertex_shader_output_t*)malloc(sizeof(vertex_shader_output_t));
    renderer->vs_out->buf_size = s_width * s_height;
    renderer->vs_out->frag_buf = (vec3*)malloc(s_width * s_height * sizeof(vec3));
    renderer->vs_out->normal_buf = (vec3*)malloc(s_width * s_height * sizeof(vec3));
    renderer->vs_out->pos_buf = (vec4*)malloc(s_width * s_height * sizeof(vec4));

    renderer->vpp_out = (vertex_post_processer_output_t*)malloc(sizeof(vertex_post_processer_output_t));
    renderer->vpp_out->buf_size = s_width * s_height;
    renderer->vpp_out->pos_buf = (vec3*)malloc(s_width * s_height * sizeof(vec3));
    renderer->zfar = zfar;
    renderer->znear = znear;

    return 0;
}

int renderer_reset_buffers(render_t* renderer) {
    memset(renderer->frame_buffer, 0, renderer->s_width * renderer->s_height * sizeof(int));
    memset(renderer->zbuffer, 0, renderer->s_width * renderer->s_height * sizeof(float));
    
    return 0;
}

int renderer_render(render_t* renderer, mat4* view_proj, mat4* model, render_object_t* object, light_t* lights) {
    renderer_reset_buffers(renderer);    

    // Vertex shader
    vertex_shader_input_t vs_in;

    vs_in.view_proj = view_proj;
    vs_in.model = model;
    
    // TODO: generalize to multiple meshes
    vs_in.buf_size = object->meshes[0].vbuff_size;
    vs_in.vertex_buf = object->meshes[0].vertex; 
    vs_in.normal_buf = object->meshes[0].normal; 
    
    printf("Running vertex shader on buffer with size %d\n", vs_in.buf_size);
    vertex_shader(&vs_in, renderer->vs_out);

    for (int i = 0; i < vs_in.buf_size; ++i) {
        printf("%f %f %f\n", renderer->vs_out->frag_buf[i].x, renderer->vs_out->frag_buf[i].y, renderer->vs_out->frag_buf[i].z);
    }

    // Vertex post processor
    vertex_post_processer_input_t vpp_in;
    vpp_in.buf_size = renderer->vs_out->buf_size;
    vpp_in.pos_buf = renderer->vs_out->pos_buf;
    vpp_in.s_width = renderer->s_width;
    vpp_in.s_height = renderer->s_height;

    // Rasterizer

    // Fragment shader

    return 0;
}

int renderer_destroy(render_t* renderer) {
    if (renderer->frame_buffer)
        free(renderer->frame_buffer);

    if (renderer->zbuffer)
        free(renderer->zbuffer);

    if (renderer->vs_out) {
        free(renderer->vs_out->frag_buf);
        free(renderer->vs_out->normal_buf);
        free(renderer->vs_out->pos_buf);
        free(renderer->vs_out);
    }

    if (renderer->vpp_out) {
        free(renderer->vpp_out->pos_buf);
        free(renderer->vpp_out);
    }

    return 0;
}
