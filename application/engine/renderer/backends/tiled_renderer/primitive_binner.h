#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "types.h"

namespace TR {

void primitive_binner(const std::vector<Triangle>& in_triangles, std::vector<Tile>& tiles);

} // namespace TR
