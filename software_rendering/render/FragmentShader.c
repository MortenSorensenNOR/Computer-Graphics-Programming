#include "FragmentShader.h"

int fragment_shader(fragment_shader_input_t* in, fragment_shader_output_t* out) {
    for (int y = 0; y < in->s_height; ++y) {
        for (int x = 0; x < in->s_width; ++x) {
            int i = x + y * in->s_width;
            if (in->zbuffer[i] >= 0.995) {
                continue;
            }

            int texture_index = (int)(in->uv[i].x * in->textures->diffuse_width) + (int)(in->uv[i].y * in->textures->diffuse_height) * in->textures->diffuse_width;
            vec3 diffuse_base = in->textures->diffuse[texture_index];

            vec3 norm = vec3_normalize(&in->norm[i]);
            vec3 lightDir;
            float diff;
            vec3 diffuse;
            
            vec3 viewDir;
            vec3 reflectDir;
            float spec;
            vec3 specular;
            
            out->framebuffer[i] = color_v(&diffuse_base);
        }
    }

    return 0;
}
