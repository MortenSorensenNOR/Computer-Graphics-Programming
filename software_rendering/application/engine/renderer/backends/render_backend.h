#pragma once

#include <glm/glm.hpp>
#include "../../../core_utils/core_utils.h"
#include "../../engine_utils/texture.h"
#include "../../engine_utils/mesh.h"

class RenderBackend {
public:
    RenderBackend();
    ~RenderBackend();

    /**
     * @brief Creates a texture and loads it into the memory of the render device, i.e. either CPU or GPU memory
     * @return The texture ID of the created texture
     */
    int virtual CreateTexture(Texture<glm::vec3>& texture) = 0;

    int virtual CreateMeshBuffer(Mesh& mesh) = 0;

    int virtual BindTexture(int texture_id, TextureType type) = 0;

    int virtual BindMesh(int mesh_id) = 0;
};
