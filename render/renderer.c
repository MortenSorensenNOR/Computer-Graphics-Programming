#include <time.h>
#include "renderer.h"

int renderer_init(render_t* renderer, int s_width, int s_height, float zfar, float znear) {
    renderer->s_width = s_width;
    renderer->s_height = s_height;

    renderer->frame_buffer = (int*)malloc(s_width * s_height * sizeof(int));
    renderer->zbuffer = (float*)malloc(s_width * s_height * sizeof(float));

    renderer->zfar = zfar;
    renderer->znear = znear;

    return 0;
}

int renderer_reset_buffers(render_t* renderer) {
    memset(renderer->frame_buffer, 0, renderer->s_width * renderer->s_height * sizeof(int));
    for (int i = 0; i < renderer->s_width * renderer->s_height; ++i) {
        renderer->zbuffer[i] = 1.0f;
    }
    
    return 0;
}

int renderer_render(render_t* renderer, vec3 cam_pos, mat4* view_proj, mat4* model, render_object_t* object, light_t* lights) {
    double total_time = 0, vs_time = 0, vpp_time = 0, pa_time = 0, ra_time = 0, fs_time = 0;
    clock_t start, end;
    clock_t total_start, total_end;

    total_start = clock();

    rasterizer_output_t ra_out;
    ra_out.buf_size = renderer->s_width * renderer->s_height;
    ra_out.uv = (vec2*)malloc(ra_out.buf_size * sizeof(vec2));
    ra_out.norm = (vec3*)malloc(ra_out.buf_size * sizeof(vec3));
    ra_out.frag = (vec3*)malloc(ra_out.buf_size * sizeof(vec3));
    ra_out.color = (vec3*)malloc(ra_out.buf_size * sizeof(vec3));
    ra_out.zbuffer = renderer->zbuffer;

    for (int i = 0; i < object->num_meshes; ++i) {
        // Vertex shader
        vertex_shader_input_t vs_in;
        vertex_shader_output_t vs_out;

        start = clock();
        vs_in.view_proj = view_proj;
        vs_in.model = model;
        vs_in.buf_size = object->meshes[i].vbuff_size;
        vs_in.vertex_buf = object->meshes[i].vertex; 
        vs_in.normal_buf = object->meshes[i].normal; 

        vs_out.buf_size = object->meshes[i].vbuff_size;
        vs_out.pos_buf = (vec4*)malloc(vs_out.buf_size * sizeof(vec4));
        vs_out.normal_buf = (vec3*)malloc(vs_out.buf_size * sizeof(vec3));
        vs_out.frag_buf = (vec3*)malloc(vs_out.buf_size * sizeof(vec3));

        vertex_shader(&vs_in, &vs_out);
        end = clock();
        vs_time += (1000.0f * (end - start) / CLOCKS_PER_SEC);

        // Vertex post processor
        vertex_post_processer_input_t vpp_in;
        vertex_post_processer_output_t vpp_out;

        start = clock();
        vpp_in.znear = renderer->znear;
        vpp_in.zfar = renderer->zfar;
        vpp_in.buf_size = vs_out.buf_size;
        vpp_in.pos_buf = vs_out.pos_buf;
        vpp_in.s_width = renderer->s_width;
        vpp_in.s_height = renderer->s_height;

        vpp_out.buf_size = vs_out.buf_size;
        vpp_out.pos_buf = (vec3*)malloc(vpp_out.buf_size * sizeof(vec3));

        vertex_post_processer(&vpp_in, &vpp_out);

        if (vs_out.pos_buf)
            free(vs_out.pos_buf);

        end = clock();
        vpp_time += (1000.0f * (end - start) / CLOCKS_PER_SEC);

        // Primitives assembler
        primitives_assembler_input_t pa_in;
        primitives_assembler_output_t pa_out;

        start = clock();
        pa_in.cam_pos = cam_pos;
        pa_in.vert_buf_size = vpp_out.buf_size;
        pa_in.vert_buf = vpp_out.pos_buf;
        pa_in.norm_buf = vs_out.normal_buf;
        pa_in.frag_buf = vs_out.frag_buf;
        pa_in.uv_buf = object->meshes[i].uv;
        pa_in.index_buf_size = object->meshes[i].ibuff_size;
        pa_in.index_buf = object->meshes[i].index;

        // TODO: Find better way to alloc memory for triangles
        pa_out.tri_buf_size = pa_in.index_buf_size / 3;
        pa_out.tri_buf = (triangle_t*)malloc(pa_out.tri_buf_size * sizeof(triangle_t));

        primitives_assembler(&pa_in, &pa_out);

        if (vpp_out.pos_buf)
            free(vpp_out.pos_buf);
        if (vs_out.normal_buf)
            free(vs_out.normal_buf);
        if (vs_out.frag_buf)
            free(vs_out.frag_buf);

        end = clock();
        pa_time += (1000.0f * (end - start) / CLOCKS_PER_SEC);

        // Rasterizer
        rasterizer_input_t ra_in;

        start = clock();
        ra_in.s_width = renderer->s_width;
        ra_in.s_height = renderer->s_height;
        ra_in.tri_buf_size = pa_out.tri_buf_size;
        ra_in.tri_buf = pa_out.tri_buf;

        rasterizer(&ra_in, &ra_out);

        end = clock();
        ra_time += (1000.0f * (end - start) / CLOCKS_PER_SEC);

        if (pa_out.tri_buf)
            free(pa_out.tri_buf);
    }

    // Fragment shader
    fragment_shader_input_t fs_in;
    fragment_shader_output_t fs_out;

    start = clock();
    fs_in.s_width = renderer->s_width;
    fs_in.s_height = renderer->s_height;
    fs_in.zbuffer = renderer->zbuffer;
    fs_in.textures = object->textures;
    fs_in.cam_pos = cam_pos;
    fs_in.light = lights;

    fs_in.norm = ra_out.norm;
    fs_in.uv = ra_out.uv;
    fs_in.frag = ra_out.frag;
    fs_in.color = ra_out.color;

    fs_out.buf_size = renderer->s_width * renderer->s_height;
    fs_out.framebuffer = renderer->frame_buffer;

    fragment_shader(&fs_in, &fs_out);

    end = clock();
    fs_time = (1000.0f * (end - start) / CLOCKS_PER_SEC);

    if (ra_out.uv)
        free(ra_out.uv);
    if (ra_out.norm)
        free(ra_out.norm);
    if (ra_out.frag)
        free(ra_out.frag);
    if (ra_out.color)
        free(ra_out.color);

    total_end = clock();
    total_time = (1000.0f * (total_end - total_start) / CLOCKS_PER_SEC);
    printf("Total time: %f ms    vs_time: %f ms    vpp_time: %f ms    pa_time: %f ms    ra_time: %f ms    fs_time: %f ms\n", total_time, vs_time, vpp_time, pa_time, ra_time, fs_time);

    return 0;
}   

int renderer_destroy(render_t* renderer) {
    if (renderer->frame_buffer)
        free(renderer->frame_buffer);

    if (renderer->zbuffer)
        free(renderer->zbuffer);

    return 0;
}
