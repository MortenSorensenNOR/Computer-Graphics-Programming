#include "FragmentShader.h"

int fragment_shader(fragment_shader_input_t* in, fragment_shader_output_t* out) {
    for (int y = 0; y < in->s_height; ++y) {
        for (int x = 0; x < in->s_width; ++x) {
            int i = x + y * in->s_width;
            if (in->zbuffer[i] >= 0.995) {
                continue;
            }

            unsigned long long diffuse_texture_index = (int)( in->uv[i].x * in->textures->diffuse_width) + (int)(in->uv[i].y * in->textures->diffuse_height) * in->textures->diffuse_width;
            diffuse_texture_index = min(diffuse_texture_index, in->textures->diffuse_width * in->textures->diffuse_height);

            unsigned long long specular_texture_index = (int)( in->uv[i].x * in->textures->specular_width) + (int)(in->uv[i].y * in->textures->specular_height) * in->textures->specular_width;
            specular_texture_index = min(specular_texture_index, in->textures->specular_width * in->textures->specular_height);

            // vec3 diffuse_base = in->textures->diffuse[diffuse_texture_index];
            vec3 diffuse_base = in->textures->diffuse[diffuse_texture_index];
            vec3 metalic_roughness = in->textures->specular[specular_texture_index];

            vec3 ambient = (vec3){
                diffuse_base.x * in->light->ambient.x,
                diffuse_base.y * in->light->ambient.y,
                diffuse_base.z * in->light->ambient.z,
            };

            vec3 norm = vec3_normalize(&in->norm[i]);
            vec3 lightDir = vec3_sub(&in->light->pos, &in->frag[i]);
            lightDir = vec3_normalize(&lightDir);

            float diff = max(vec3_dot(&norm, &lightDir), 0.0);
            vec3 diffuse = (vec3){
                in->light->diffuse.x * diff * diffuse_base.x,
                in->light->diffuse.y * diff * diffuse_base.y,
                in->light->diffuse.z * diff * diffuse_base.z,
            };
            
            // TODO: Fix specular
            // vec3 viewDir = vec3_sub(&in->cam_pos, &in->frag[i]);
            // viewDir = vec3_normalize(&viewDir);
            //
            // // All other uses of lightDir are negated
            // lightDir = vec3_mul(&lightDir, -1.0f);
            // double norm_light_proj = vec3_dot(&norm, &lightDir);
            // vec3 reflectDir = vec3_mul(&norm, 2.0f * norm_light_proj);
            // reflectDir = vec3_sub(&lightDir, &reflectDir);
            //
            // float spec = pow(max(vec3_dot(&viewDir, &reflectDir), 0.0f), 32.0f);
            // vec3 specular = (vec3){
            //     in->light->specular.x * spec * metalic_roughness.y,
            //     in->light->specular.y * spec * metalic_roughness.y,
            //     in->light->specular.z * spec * metalic_roughness.y
            // };

            vec3 result = vec3_add(&ambient, &diffuse);
            // result = vec3_add(&result, &specular);

            out->framebuffer[i] = color_v(&result);
        }
    }

    return 0;
}
