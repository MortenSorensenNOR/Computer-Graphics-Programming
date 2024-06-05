#include "renderer.h"

int renderer_init(render_t* renderer, int s_width, int s_height, float zfar, float znear) {
    renderer->s_width = s_width;
    renderer->s_height = s_height;

    renderer->frame_buffer = (int*)malloc(s_width * s_height * sizeof(int));
    renderer->zbuffer = (float*)malloc(s_width * s_height * sizeof(float));

    /* renderer->vs_out = (vertex_shader_output_t*)malloc(sizeof(vertex_shader_output_t)); */
    /* renderer->vs_out->buf_size = s_width * s_height; */
    /* renderer->vs_out->frag_buf = (vec3*)malloc(s_width * s_height * sizeof(vec3)); */
    /* renderer->vs_out->normal_buf = (vec3*)malloc(s_width * s_height * sizeof(vec3)); */
    /* renderer->vs_out->pos_buf = (vec4*)malloc(s_width * s_height * sizeof(vec4)); */
    /*  */
    /* renderer->vpp_out = (vertex_post_processer_output_t*)malloc(sizeof(vertex_post_processer_output_t)); */
    /* renderer->vpp_out->buf_size = s_width * s_height; */
    /* renderer->vpp_out->pos_buf = (vec3*)malloc(s_width * s_height * sizeof(vec3)); */
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
    vertex_shader_output_t vs_out;

    vs_in.view_proj = view_proj;
    vs_in.model = model;
    vs_in.buf_size = object->meshes[0].vbuff_size;
    vs_in.vertex_buf = object->meshes[0].vertex; 
    vs_in.normal_buf = object->meshes[0].normal; 
    
    vs_out.buf_size = object->meshes[0].vbuff_size;
    vs_out.pos_buf = (vec4*)malloc(vs_out.buf_size * sizeof(vec4));
    vs_out.normal_buf = (vec3*)malloc(vs_out.buf_size * sizeof(vec3));
    vs_out.frag_buf = (vec3*)malloc(vs_out.buf_size * sizeof(vec3));
    
    printf("Running vertex shader on buffer with size %d\n", vs_in.buf_size);
    vertex_shader(&vs_in, &vs_out);

    // Vertex post processor
    vertex_post_processer_input_t vpp_in;
    vertex_post_processer_output_t vpp_out;

    vpp_in.buf_size = vs_out.buf_size;
    vpp_in.pos_buf = vs_out.pos_buf;
    vpp_in.s_width = renderer->s_width;
    vpp_in.s_height = renderer->s_height;

    vpp_out.buf_size = vs_out.buf_size;
    vpp_out.pos_buf = (vec3*)malloc(vpp_out.buf_size * sizeof(vec3));

    vertex_post_processer(&vpp_in, &vpp_out);

    // Primitives assembler
    primitives_assembler_input_t pa_in;
    primitives_assembler_output_t pa_out;

    pa_in.vert_buf_size = vpp_out.buf_size;
    pa_in.vert_buf = vpp_out.pos_buf;
    pa_in.norm_buf = vs_out.normal_buf;
    pa_in.frag_buf = vs_out.frag_buf;
    pa_in.uv_buf = object[0].meshes->uv;
    pa_in.index_buf_size = object[0].meshes->ibuff_size;
    pa_in.index_buf = object[0].meshes->index;

    // TODO: Find better way to alloc memory for triangles
    pa_out.tri_buf_size = pa_in.index_buf_size / 3;
    pa_out.tri_buf = (triangle_t*)malloc(pa_out.tri_buf_size * sizeof(triangle_t));

    primitives_assembler(&pa_in, &pa_out);
    
    // Rasterizer
    rasterizer_input_t ra_in;
    rasterizer_output_t ra_out;

    // Fragment shader
    fragement_shader_input_t fs_in;
    fragement_shader_output_t fs_out;

    // Free allocated resources
    free(vs_out.pos_buf);

    return 0;
}

int renderer_destroy(render_t* renderer) {
    if (renderer->frame_buffer)
        free(renderer->frame_buffer);

    if (renderer->zbuffer)
        free(renderer->zbuffer);

    /* if (renderer->vs_out) { */
    /*     free(renderer->vs_out->frag_buf); */
    /*     free(renderer->vs_out->normal_buf); */
    /*     free(renderer->vs_out->pos_buf); */
    /*     free(renderer->vs_out); */
    /* } */
    /*  */
    /* if (renderer->vpp_out) { */
    /*     free(renderer->vpp_out->pos_buf); */
    /*     free(renderer->vpp_out); */
    /* } */

    return 0;
}
