#pragma once

#include <stdexcept>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "../../core_utils/core_utils.h"

struct Mesh {
    Buffer<glm::vec4> vertexes;
    Buffer<std::size_t> indices;

    Mesh(Buffer<glm::vec4>& v, Buffer<std::size_t>& i) {
        if (v.size == 0 || i.size == 0)
            throw std::runtime_error("Invalid length of mesh properties");


        vertexes = buffer_allocate<glm::vec4>(v.size);
        indices = buffer_allocate<std::size_t>(i.size);
        buffer_copy(v, vertexes);
        buffer_copy(i, indices);
    }
};
