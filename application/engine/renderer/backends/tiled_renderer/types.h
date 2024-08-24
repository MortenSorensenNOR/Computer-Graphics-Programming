#pragma once

#include <glm/glm.hpp>

namespace TR {

struct Vertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Triangle {
    std::size_t mesh_id; // Will be used to look up correct textures
    Vertex vertices[3];
};

struct Tile {
    glm::vec2 min;
    glm::vec2 max;
    std::vector<Triangle> triangles;
};

} // namespace TR
