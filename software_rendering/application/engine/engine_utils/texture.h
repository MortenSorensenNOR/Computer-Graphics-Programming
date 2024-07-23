#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include "../../core_utils/core_utils.h"

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
    Buffer<glm::vec<3, T, glm::lowp>> data;

    Texture(std::size_t texture_width, std::size_t texture_height, Buffer<glm::vec<3, T, glm::lowp>>& texture_data) {
        this->width = texture_width;
        this->height = texture_height;
        buffer_copy(texture_data, this->data);
    }
};
