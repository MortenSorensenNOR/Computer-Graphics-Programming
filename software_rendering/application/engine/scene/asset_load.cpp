#include "asset_load.h"

std::vector<std::size_t> model_load_from_path(std::vector<Mesh>& meshes, std::string path) {
    std::vector<std::size_t> res = {}; 

    printf("Loading mesh from path: %s\n", path.c_str());

    // Load logic
    const struct aiScene* scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
    if (!scene) {
        throw std::runtime_error("Could opening file at location: " + path + " (" + aiGetErrorString() + ")");
        return {};
    }

    std::size_t num_meshes = scene->mNumMeshes;
    for (std::size_t i = 0; i < num_meshes; i++) {
        struct aiMesh* mesh = scene->mMeshes[i];

        Buffer<glm::vec4> vertexes;
        Buffer<std::size_t> indices;

        std::size_t num_vertexes = mesh->mNumVertices;
        vertexes = buffer_allocate<glm::vec4>(num_vertexes);
        
        for (std::size_t v = 0; v < num_vertexes; v++) {
            aiVector3D pos = mesh->mVertices[v];
            glm::vec4 pos_glm = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
            vertexes.data[v] = pos_glm;
        }

        std::vector<std::size_t> index_buffer_data = {};
        for (std::size_t f = 0; f < mesh->mNumFaces; f++) {
            struct aiFace face = mesh->mFaces[f];

            for (std::size_t k = 0; k < face.mNumIndices; k++) {
                index_buffer_data.push_back(face.mIndices[k]);
            }
        }

        indices = buffer_allocate<std::size_t>(index_buffer_data.size());
        for (std::size_t j = 0; j < index_buffer_data.size(); j++) {
            indices.data[j] = index_buffer_data.at(j);;
        }

        // Create mesh from buffers and push to meshes vector
        Mesh loaded_mesh(vertexes, indices);
        meshes.push_back(loaded_mesh);
        res.push_back(meshes.size() - 1);

        buffer_free(vertexes);
        buffer_free(indices);
    }

    aiReleaseImport(scene);

    return res;
}

std::size_t texture_load_from_path(std::vector<Texture<float>>& textures, std::string path, TextureType type) {
    std::size_t width, height;
    Buffer<glm::vec<3, float, glm::lowp>> texture_data;

    Texture<float> texture(width, height, texture_data);

    textures.push_back(texture);
    return textures.size() - 1;
}

