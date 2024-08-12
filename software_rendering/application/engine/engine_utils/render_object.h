#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../../core_utils/core_utils.h"
#include "mesh.h"
#include "texture.h"

struct RenderObject {
    Mesh* mesh;
    glm::mat4 model;

    std::size_t id;

    RenderObject(Mesh* m, std::size_t id = -1) {
        this->mesh = m;
        this->id = id;
    }
};
