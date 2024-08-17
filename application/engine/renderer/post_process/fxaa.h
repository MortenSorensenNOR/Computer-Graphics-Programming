#pragma once

/**
 * @brief This is an implementation of FXAA (fast approximate anti-aliasing): https://en.wikipedia.org/wiki/Fast_approximate_anti-aliasing
 */ 

#include <memory>
#include <float.h>
#include <cstddef>
#include <glm/glm.hpp>
#include "../../../core_utils/buffer.h"

class FXAA {
private: 
    glm::ivec2 dim;
    Buffer<glm::vec3> output_buffer;

public:
    FXAA(std::size_t width, std::size_t height) {
        this->dim = {width, height};
        this->output_buffer = buffer_allocate<glm::vec3>(width * height);
    }

    Buffer<glm::vec3>* fxaa(Buffer<glm::vec3>& input, float threshold) {
        double total_lumMin =  DBL_MAX;
        double total_lumMax = -DBL_MAX;

        for (std::size_t y = 1; y < dim.y - 1; y++) {
            for (std::size_t x = 1; x < dim.x - 1; x++) {
                double lumCenter = _luminance(input.data[y * dim.x + x]);
                double lumLeft = _luminance(input.data[y * dim.x + x - 1]);
                double lumRight = _luminance(input.data[y * dim.x + x + 1]);
                double lumUp = _luminance(input.data[(y - 1) * dim.x + x]);
                double lumDown = _luminance(input.data[(y + 1) * dim.x + x]);

                double lumMin = std::min({lumCenter, lumLeft, lumRight, lumUp, lumDown});
                double lumMax = std::max({lumCenter, lumLeft, lumRight, lumUp, lumDown});
                total_lumMin = std::min(total_lumMin, lumMin);
                total_lumMax = std::max(total_lumMax, lumMax);

                if (lumMax - lumMin > threshold) {
                    output_buffer.data[y * dim.x + x] = (input.data[y * dim.x + x] + input.data[y * dim.x + x - 1] + input.data[y * dim.x + x + 1] + input.data[(y - 1) * dim.x + x] + input.data[(y + 1) * dim.x + x]) / 5.0f;
                } else {
                    output_buffer.data[y * dim.x + x] = input.data[y * dim.x + x];
                }
            }
        }

        printf("Max lum: %f \t Min lum: %f\n", total_lumMax, total_lumMin);

        return &output_buffer;
    }

private:
    double _luminance(glm::vec3& p) {
        return 0.299 * p.x + 0.587 * p.y;
    }
};
 
