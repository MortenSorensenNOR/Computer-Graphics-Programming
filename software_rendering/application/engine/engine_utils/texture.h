#pragma once

#include <glm/glm.hpp>

template <typename T>
struct Texture {
    std::size_t width, height;
    std::size_t size;
    glm::vec<3, T, glm::lowp> data;
};
