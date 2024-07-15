#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

struct Mesh {
    std::size_t vertex_buffer_size;
    std::size_t normal_buffer_size;
    std::size_t uv_buffer_size;
    std::size_t index_buffer_size;

    glm::vec4* vertexes;
    glm::vec3* normals;
    glm::vec2* uvs; 
    std::size_t* indices;
};
