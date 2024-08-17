#pragma once

/**
 * @brief This is an implementation of a Bilinear Resampling algorithm for upscaling images: https://en.wikipedia.org/wiki/Bilinear_interpolation
 */ 

#include <memory>
#include <cstddef>
#include <glm/glm.hpp>
#include "../../../core_utils/buffer.h"

class Bilinear {
private:
    glm::ivec2 dimIn, dimOut;
    Buffer<glm::vec3> output_buffer;
    static constexpr double pi_square = M_PI * M_PI;

public:
    /**
     * @brief Instance of a bilinear upscaler
     * @param in_width      The width of the input image
     * @param in_height     The height of the input image
     * @param out_width     The width of the output image
     * @param out_height    The height of the output image
     */
    Bilinear(std::size_t in_width, std::size_t in_height, std::size_t out_width, std::size_t out_height) {
        this->dimIn = {in_width, in_height};
        this->dimOut = {out_width, out_height};
        this->output_buffer = buffer_allocate<glm::vec3>(out_width * out_height);
    }

    ~Bilinear() {
        buffer_free(output_buffer);
    }

    Buffer<glm::vec3>* upscale(Buffer<glm::vec3>& in_buffer) {
        float x_ratio = static_cast<float>(dimIn.x - 1) / (dimOut.x - 1);
        float y_ratio = static_cast<float>(dimIn.y - 1) / (dimOut.y - 1);

        for (int y = 0; y < dimOut.y; ++y) {
            for (int x = 0; x < dimOut.x; ++x) {
                float src_x = x * x_ratio;
                float src_y = y * y_ratio;

                int x_int = static_cast<int>(src_x);
                int y_int = static_cast<int>(src_y);

                float x_diff = src_x - x_int;
                float y_diff = src_y - y_int;

                // Ensure the indices are within bounds
                int x_next = std::min(x_int + 1, dimIn.x - 1);
                int y_next = std::min(y_int + 1, dimIn.y - 1);

                // Get the four neighboring pixels
                glm::vec3 top_left = in_buffer.data[y_int * dimIn.x + x_int];
                glm::vec3 top_right = in_buffer.data[y_int * dimIn.x + x_next];
                glm::vec3 bottom_left = in_buffer.data[y_next * dimIn.x + x_int];
                glm::vec3 bottom_right = in_buffer.data[y_next * dimIn.x + x_next];

                // Perform bilinear interpolation
                output_buffer.data[y * dimOut.x + x] = _bilinear_interpolate(top_left, top_right, bottom_left, bottom_right, x_diff, y_diff);
            }
        }

        return &output_buffer;
    }

private:
    glm::vec3 _bilinear_interpolate(const glm::vec3& top_left, const glm::vec3& top_right, const glm::vec3& bottom_left, const glm::vec3& bottom_right, float x_ratio, float y_ratio) {
        glm::vec3 top = top_left * (1.0f - x_ratio) + top_right * x_ratio;
        glm::vec3 bottom = bottom_left * (1.0f - x_ratio) + bottom_right * x_ratio;
        return top * (1.0f - y_ratio) + bottom * y_ratio;
    }
};

