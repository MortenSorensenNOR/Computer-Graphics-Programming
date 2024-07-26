#pragma once

#define RENDER_OBJECT_MAX_NUM_TEXTURES 5

#include <vector>
#include <glm/glm.hpp>

#include "../../core_utils/core_utils.h"
#include "mesh.h"
#include "texture.h"

struct RenderObject {
    Mesh mesh;
    glm::mat4 model;

    RenderObject(Buffer<glm::vec4>& vertexes, Buffer<std::size_t>& indices) 
        : mesh(vertexes, indices) {}
};
