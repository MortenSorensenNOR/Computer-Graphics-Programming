#pragma once

#include <glm/glm.hpp>
#include "../../core_utils/core_utils.h"

enum class LightType {
    PointLight,
    SpotLight,

};

typedef struct {
    glm::vec3 pos;
    glm::vec<3, float, glm::lowp> color;

    LightType type;
} Light;
