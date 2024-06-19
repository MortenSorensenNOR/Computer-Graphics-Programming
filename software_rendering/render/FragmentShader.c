#include "FragmentShader.h"

#ifdef RASTERIZER_MULTI_THREAD
void* fragment_shader_thread_func(void* arg_ptr) {
    fragment_shader_thread_argument_t* thread_arg = (fragment_shader_thread_argument_t*)arg_ptr; 
    for (int y = thread_arg->batch_start_y; y < thread_arg->batch_end_y; ++y) {
        for (int x = 0; x < thread_arg->s_width; x++) {
            int i = x + y * thread_arg->s_width;
            if (thread_arg->zbuffer[i] >= 0.999) {
                continue;
            }
            
            unsigned long long diffuse_texture_index = (int)( thread_arg->uv[i].x * thread_arg->textures->diffuse_width) + 
                (int)(thread_arg->uv[i].y * thread_arg->textures->diffuse_height) * thread_arg->textures->diffuse_width;
            diffuse_texture_index = min(diffuse_texture_index, thread_arg->textures->diffuse_width * thread_arg->textures->diffuse_height);

            unsigned long long specular_texture_index = (int)( thread_arg->uv[i].x * thread_arg->textures->specular_width) + 
                (int)(thread_arg->uv[i].y * thread_arg->textures->specular_height) * thread_arg->textures->specular_width;
            specular_texture_index = min(specular_texture_index, thread_arg->textures->specular_width * thread_arg->textures->specular_height);

            // vec3 diffuse_base = in->textures->diffuse[diffuse_texture_index];
            vec3 diffuse_base = thread_arg->textures->diffuse[diffuse_texture_index];
            vec3 metalic_roughness = thread_arg->textures->specular[specular_texture_index];

            vec3 ambient = (vec3){
                diffuse_base.x * thread_arg->light->ambient.x,
                diffuse_base.y * thread_arg->light->ambient.y,
                diffuse_base.z * thread_arg->light->ambient.z,
            };

            printf("(%f, %f, %f)\n", thread_arg->frag[i].x, thread_arg->frag[i].y, thread_arg->frag[i].z);

            vec3 norm = vec3_normalize(&thread_arg->norm[i]);
            vec3 lightDir = vec3_sub(&thread_arg->light->pos, &thread_arg->frag[i]);
            lightDir = vec3_normalize(&lightDir);

            float diff =vec3_dot(&norm, &lightDir);
            diff = max(diff, 0.0);

            vec3 diffuse = (vec3){
                thread_arg->light->diffuse.x * diff * diffuse_base.x,
                thread_arg->light->diffuse.y * diff * diffuse_base.y,
                thread_arg->light->diffuse.z * diff * diffuse_base.z,
            };
            
            vec3 result = vec3_add(&ambient, &diffuse);
            thread_arg->framebuffer[i] = color_v(&result);
        }
    }

    return NULL;
}
#endif

int fragment_shader(fragment_shader_input_t* in, fragment_shader_output_t* out) {
    #ifdef RASTERIZER_MULTI_THREAD
    int num_threads = 4;
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    fragment_shader_thread_argument_t* thread_args = (fragment_shader_thread_argument_t*)malloc(num_threads * sizeof(fragment_shader_thread_argument_t));
    
    int batch_size_y = in->s_height / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].s_width = in->s_width;
        thread_args[i].s_height = in->s_height;

        thread_args[i].cam_pos = in->cam_pos;
        thread_args[i].light = in->light;
        thread_args[i].textures = in->textures;

        thread_args[i].batch_start_y = i * batch_size_y;
        thread_args[i].batch_end_y = i * batch_size_y + batch_size_y;
        thread_args[i].uv = in->uv;
        thread_args[i].norm = in->norm;
        thread_args[i].frag = in->frag;
        thread_args[i].color = in->color;
        thread_args[i].zbuffer = in->zbuffer;

        thread_args[i].framebuffer = out->framebuffer;

        pthread_create(&threads[i], NULL, fragment_shader_thread_func, (void*)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_args);

    #else
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
    #endif


    return 0;
}
