#pragma once

#define RENDER_OBJECT_MAX_NUM_TEXTURES 5

#include <vector>
#include <glm/glm.hpp>

#include "../../core_utils/core_utils.h"
#include "mesh.h"
#include "texture.h"

struct RenderObject {
    Mesh mesh;
    // Texture<glm::vec3>* textures[RENDER_OBJECT_MAX_NUM_TEXTURES];

    RenderObject(Buffer<glm::vec4>& vertexes, Buffer<std::size_t>& indices) 
        : mesh(vertexes, indices) {}
};
