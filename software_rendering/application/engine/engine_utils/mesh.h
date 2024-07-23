#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "../../core_utils/core_utils.h"

struct Mesh {
    Buffer<glm::vec4> vertexes;
    Buffer<glm::vec3> normals;
    Buffer<glm::vec2> uvs;
    Buffer<std::size_t> indices;

    Mesh(Buffer<glm::vec4>& vertexes, Buffer<glm::vec3>& normals, Buffer<glm::vec2>& uvs, Buffer<std::size_t>& indices) {
        buffer_copy(vertexes, this->vertexes);
        buffer_copy(normals, this->normals);
        buffer_copy(uvs, this->uvs);
        buffer_copy(indices, this->indices);
    }
};
