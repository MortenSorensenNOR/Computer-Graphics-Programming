#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "types.h"

namespace TR {

void vertex_post_processor(const std::vector<Vertex>& in_vertices, std::vector<Vertex>& out_verticies);

} // namespace T
