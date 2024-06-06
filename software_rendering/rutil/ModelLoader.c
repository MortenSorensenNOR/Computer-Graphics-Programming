#define STB_IMAGE_IMPLEMENTATION
#include "ModelLoader.h"
#include "stb_image.h"

vec3 assimp_vec3_to_vec3(const struct aiVector3D* p) {
    return (vec3){p->x, p->y, p->z};
}

int parse_obj(const char* fpath, render_object_t* object) {
    const struct aiScene* scene = aiImportFile(fpath, aiProcess_Triangulate | aiProcess_FlipUVs );
    if (!scene) {
        printf("Error importing .obj file: %s\n", aiGetErrorString());
        return 1;
    }

    object->meshes = (mesh_t*)malloc(scene->mNumMeshes * sizeof(mesh_t));
    object->num_meshes = scene->mNumMeshes;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        struct aiMesh* mesh = scene->mMeshes[i];

        // Allocate space for each of the mesh buffers
        object->meshes[i].vertex = (vec4*)malloc(mesh->mNumVertices * sizeof(vec4));
        object->meshes[i].normal = (vec3*)malloc(mesh->mNumVertices * sizeof(vec3));
        object->meshes[i].uv = (vec2*)malloc(mesh->mNumVertices * sizeof(vec2));

        object->meshes[i].vbuff_size = mesh->mNumVertices;
        object->meshes[i].nbuff_size = mesh->mNumVertices;
        object->meshes[i].uvbuff_size = mesh->mNumVertices;

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            struct aiVector3D position = mesh->mVertices[j];
            struct aiVector3D normal = mesh->mNormals[j];
    
            vec3 position_vec3 = assimp_vec3_to_vec3(&position);
            vec4 position_vec4 = vec3_to_vec4(&position_vec3);
            vec3 normal_vec3 = assimp_vec3_to_vec3(&normal);

            struct aiVector3D uv = mesh->mTextureCoords[0][j];
            vec3 uv_vec3 = assimp_vec3_to_vec3(&uv);
            vec2 uv_vec2 = vec3_to_vec2(&uv_vec3);
    
            object->meshes[i].vertex[j] = position_vec4;
            object->meshes[i].normal[j] = normal_vec3;
            object->meshes[i].uv[j] = uv_vec2;
        }

        int num_indicies = 0;
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            num_indicies += mesh->mFaces[i].mNumIndices; 
        }

        object->meshes[i].index = (unsigned int*)malloc(num_indicies * sizeof(int));
        object->meshes[i].ibuff_size = num_indicies;

        int curr_index = 0;
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            struct aiFace face = mesh->mFaces[j];

            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                object->meshes[i].index[curr_index+k] = face.mIndices[k];
            }
            curr_index += face.mNumIndices;
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

int load_texture(const char* fpath, vec3** buffer, int* texture_width, int* texture_height) {
    int width, height, channels;
    unsigned char* img = stbi_load(fpath, &width, &height, &channels, 0);
    if (img == NULL) {
        printf("Error in loading the image\n");
        return 1;
    }

    if (channels < 3) {
        printf("Error: non-rgb images have not yet been implemented\n");
        stbi_image_free(img);
        return 1;
    }

    if (width <= 0 || height <= 0 || width * height <= 0) {
        printf("Error: image must have positive dimensions\n");
        stbi_image_free(img);
        return 1;
    }

    *texture_width = width;
    *texture_height = height;
    *buffer = (vec3*)malloc(width * height * sizeof(vec3));
    for (int i = 0; i < width * height; i++) {
        (*buffer)[i] = (vec3){img[i * channels] / 255.0, img[i * channels + 1] / 255.0, img[i * channels + 2] / 255.0};
    }
    
    stbi_image_free(img);

    return 0;
}

int free_render_object(render_object_t* object) {
    if (object->meshes) {
        for (int i = 0; i < object->num_meshes; ++i) {
            if (object->meshes[i].vertex)
                free(object->meshes[i].vertex);
            if (object->meshes[i].normal)
                free(object->meshes[i].normal);
            if (object->meshes[i].uv)
                free(object->meshes[i].uv);
            if (object->meshes[i].index)
                free(object->meshes[i].index);
        }
        free(object->meshes);
    }

    return 0;
}
