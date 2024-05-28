#include "ModelLoader.h"

vec3 assimp_vec3_to_vec3(const struct aiVector3D* p) {
    return (vec3){p->x, p->y, p->z};
}

int parse_obj(const char* fpath, scene_t* res_scene) {
    const struct aiScene* scene = aiImportFile(fpath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        printf("Error importing .obj file: %s\n", aiGetErrorString());
        return 1;
    }

    res_scene->objects = (render_object_t*)malloc(scene->mNumMeshes * sizeof(render_object_t));
    res_scene->num_objects = scene->mNumMeshes;
    res_scene->lights = (light_t*)malloc(scene->mNumLights * sizeof(light_t));
    res_scene->num_lights = scene->mNumLights;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        struct aiMesh* mesh = scene->mMeshes[i];
        res_scene->objects[i].mesh = (mesh_t*)malloc(sizeof(mesh_t));

        // Allocate space for each of the mesh buffers
        res_scene->objects[i].mesh->vertex = (vec4*)malloc(mesh->mNumVertices * sizeof(vec4));
        res_scene->objects[i].mesh->normal = (vec3*)malloc(mesh->mNumVertices * sizeof(vec3));
        res_scene->objects[i].mesh->uv = (vec2*)malloc(mesh->mNumVertices * sizeof(vec2));

        res_scene->objects[i].mesh->vbuff_size = mesh->mNumVertices;
        res_scene->objects[i].mesh->nbuff_size = mesh->mNumVertices;
        res_scene->objects[i].mesh->uvbuff_size = mesh->mNumVertices;

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            struct aiVector3D position = mesh->mVertices[j];
            struct aiVector3D normal = mesh->mVertices[j];
            struct aiVector3D uv = mesh->mVertices[j];
    
            vec3 position_vec3 = assimp_vec3_to_vec3(&position);
            vec4 position_vec4 = vec3_to_vec4(&position_vec3);
            vec3 normal_vec3 = assimp_vec3_to_vec3(&normal);
            vec3 uv_vec3 = assimp_vec3_to_vec3(&uv);
            vec2 uv_vec2 = vec3_to_vec2(&uv_vec3);
    
            res_scene->objects[i].mesh->vertex[j] = position_vec4;
            res_scene->objects[i].mesh->normal[j] = normal_vec3;
            res_scene->objects[i].mesh->uv[j] = uv_vec2;
        }

        int num_indicies = 0;
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            num_indicies += mesh->mFaces[i].mNumIndices; 
        }

        res_scene->objects[i].mesh->index = (int*)malloc(num_indicies * sizeof(int));
        res_scene->objects[i].mesh->ibuff_size = num_indicies;

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            struct aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                unsigned int index = face.mIndices[k];
                res_scene->objects[i].mesh->index[k] = index;
            }
        }
    } 

    // TODO: Implement texture loading
    /* for (unsigned int i = 0; i < scene->mNumMaterials; ++i) { */
    /*     struct aiMaterial* material = scene->mMaterials[i]; */
    /*      */
    /*     if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) */
    /* } */
    aiReleaseImport(scene);

    return 0;
}

int load_texture(const char* path, vec3* buffer, int* buffer_size) {
    return 0;
}

int free_scene(scene_t* scene) {
    for (int i = 0; i < scene->num_objects; ++i) {
        if (scene->objects[i].mesh) {
            if (scene->objects[i].mesh->vertex)
                free(scene->objects[i].mesh->vertex);
            if (scene->objects[i].mesh->normal)
                free(scene->objects[i].mesh->normal);
            if (scene->objects[i].mesh->uv)
                free(scene->objects[i].mesh->uv);
            if (scene->objects[i].mesh->index)
                free(scene->objects[i].mesh->index);

            free(scene->objects[i].mesh);
        }
    }
    free(scene->objects);
    free(scene->lights);

    return 0;
}
