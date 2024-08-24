#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "types.h"

namespace TR {

void vertex_shader(const std::vector<Vertex>& in_vertices, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, std::vector<Vertex>& out_vertices);

} // namespace TR
