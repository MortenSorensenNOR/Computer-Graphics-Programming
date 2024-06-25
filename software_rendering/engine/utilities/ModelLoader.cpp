#define STB_IMAGE_IMPLEMENTATION
#include "ModelLoader.h"
#include "stb_image.h"

glm::vec3 assimp_vec3_to_vec3(const aiVector3D* p) {
    return (glm::vec3){p->x, p->y, p->z};
}

int parse_obj(const char* fpath, render_object_t* object) {
    const struct aiScene* scene = aiImportFile(fpath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords );
    if (!scene) {
        printf("Error importing .obj file: %s\n", aiGetErrorString());
        return 1;
    }

    object->meshes = (mesh_t*)malloc(scene->mNumMeshes * sizeof(mesh_t));
    object->num_meshes = scene->mNumMeshes;
    printf("Loading model with %d objects\n", scene->mNumMeshes);

    unsigned long long total_vert_count = 0;
    for (unsigned long long i = 0; i < scene->mNumMeshes; ++i) {
        struct aiMesh* mesh = scene->mMeshes[i];
        total_vert_count += mesh->mNumVertices;

        // Allocate space for each of the mesh buffers
        object->meshes[i].vertex = (glm::vec4*)malloc(mesh->mNumVertices * sizeof(glm::vec4));
        object->meshes[i].normal = (glm::vec3*)malloc(mesh->mNumVertices * sizeof(glm::vec3));
        object->meshes[i].uv = (glm::vec2*)malloc(mesh->mNumVertices * sizeof(glm::vec2));

        if (!object->meshes[i].vertex || !object->meshes[i].normal || !object->meshes[i].uv) {
            printf("Error: Could not allocate buffers.\n");
            free_render_object(object);
            return 1;
        }

        object->meshes[i].vbuff_size = mesh->mNumVertices;
        object->meshes[i].nbuff_size = mesh->mNumVertices;
        object->meshes[i].uvbuff_size = mesh->mNumVertices;

        for (int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];
    
            glm::vec3 position_vec3 = assimp_vec3_to_vec3(&position);
            glm::vec4 position_vec4 = (glm::vec4){position_vec3.x, position_vec3.y, position_vec3.z, 1.0f};
            glm::vec3 normal_vec3 = assimp_vec3_to_vec3(&normal);

            object->meshes[i].vertex[j] = position_vec4;
            object->meshes[i].normal[j] = normal_vec3;

            if (mesh->mTextureCoords[0]) {
                aiVector3D uv = mesh->mTextureCoords[0][j];
                glm::vec3 uv_vec3 = assimp_vec3_to_vec3(&uv);
                glm::vec2 uv_vec2 = (glm::vec2){uv_vec3.x, uv_vec3.y};
                object->meshes[i].uv[j] = uv_vec2;
            }
        }

        unsigned long long num_indicies = 0;
        for (unsigned long long j = 0; j < mesh->mNumFaces; ++j) {
            num_indicies += mesh->mFaces[j].mNumIndices; 
        }

        if (num_indicies == 0) {
            printf("Error: No faces for loaded model. Exiting\n");
            free_render_object(object);
            return 1;
        } 

        object->meshes[i].index = (unsigned long long*)malloc(num_indicies * sizeof(unsigned long long));
        object->meshes[i].ibuff_size = num_indicies;

        if (!object->meshes[i].index) {
            printf("Error: Could not allocate index buffer. Exiting\n");
            free_render_object(object);
            return 1;
        }

        unsigned long long curr_index = 0;
        for (unsigned long long j = 0; j < mesh->mNumFaces; ++j) {
            struct aiFace face = mesh->mFaces[j];

            for (unsigned long long k = 0; k < face.mNumIndices; ++k) {
                object->meshes[i].index[curr_index+k] = face.mIndices[k];
            }
            curr_index += face.mNumIndices;
        }
    } 
    long long int mesh_allocated_memory =  total_vert_count * (4 * sizeof(float) + 3 * sizeof(float) + 2 * sizeof(float) + sizeof(int));
    printf("Total allocated memory: %lld bytes\n", mesh_allocated_memory);

    aiReleaseImport(scene);

    return 0;
}

int load_texture(const char* fpath, glm::vec3** buffer, int* texture_width, int* texture_height) {
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
    *buffer = (glm::vec3*)malloc(width * height * sizeof(glm::vec3));
    for (int i = 0; i < width * height; i++) {
        (*buffer)[i] = (glm::vec3){img[i * channels] / 255.0, img[i * channels + 1] / 255.0, img[i * channels + 2] / 255.0};
    }
    
    stbi_image_free(img);

    return 0;
}

int free_render_object(render_object_t* object) {
    if (object) {
        printf("Freeing render object: %p\n", object);
        if (object->meshes) {
            for (int i = 0; i < object->num_meshes; ++i) {
                if (object->meshes[i].vertex != 0)
                    free(object->meshes[i].vertex);
                if (object->meshes[i].normal != 0)
                    free(object->meshes[i].normal);
                if (object->meshes[i].uv != 0)
                    free(object->meshes[i].uv);
                if (object->meshes[i].index != 0)
                    free(object->meshes[i].index);
            }
            free(object->meshes);
        }
    }

    return 0;
}
