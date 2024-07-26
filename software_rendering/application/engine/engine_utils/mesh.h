#pragma once

#include <stdexcept>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "../../core_utils/core_utils.h"

struct Mesh {
    Buffer<glm::vec4> vertexes;
    Buffer<std::size_t> indices;

    Mesh(Buffer<glm::vec4>& vertexes, Buffer<std::size_t>& indices) {
        if (vertexes.size == 0 || indices.size == 0)
            throw std::runtime_error("Invalid length of mesh properties");

        this->vertexes = buffer_allocate<glm::vec4>(vertexes.size);
        this->indices = buffer_allocate<std::size_t>(indices.size);
        buffer_copy(vertexes, this->vertexes);
        buffer_copy(indices, this->indices);
    }
};
