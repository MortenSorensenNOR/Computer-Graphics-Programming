#pragma once

#include <glm/glm.hpp>
#include "../../../core_utils/core_utils.h"
#include "../../engine_utils/texture.h"
#include "../../engine_utils/mesh.h"

namespace Renderer {

class RenderBackend {
public:
    RenderBackend();
    ~RenderBackend();

    /**
     * @brief Creates a texture and loads it into the memory of the render device, i.e. either CPU or GPU memory
     * @return The texture ID of the created texture
     */
    int CreateTexture(Texture<glm::vec3>& texture);

    int CreateMeshBuffer(Mesh& mesh);

    int BindTexture(int texture_id, TextureType type);

    int BindMesh(int mesh_id);
};

}

