#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

struct Mesh {
    std::size_t vertex_buffer_size;
    std::size_t normal_buffer_size;
    std::size_t uv_buffer_size;
    std::size_t index_buffer_size;

    glm::vec4* vertex;
    glm::vec3* normal;
    glm::vec2* uv; 
    std::size_t* index;
};
