#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "types.h"

namespace TR {

void primitive_assembler(const std::vector<Vertex>& in_vertices, const std::vector<uint32_t>& in_indices, std::vector<Triangle>& out_triangles);

} // namespace TR
