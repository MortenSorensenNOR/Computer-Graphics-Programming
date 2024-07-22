#pragma once

#include <algorithm>
#include <glm/glm.hpp>

enum class TextureType {
    DIFFUSE,
    NORMAL,
    SPECULAR,
    AO,
    DISPLACEMENT
};

template <typename T>
struct Texture {
    std::size_t width, height;
    std::size_t size;
    glm::vec<3, T, glm::lowp>* data;
};
