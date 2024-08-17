#pragma once

/**
 * @brief This is an implementation of the Lanczos Resampling algorithm for upscaling images: https://en.wikipedia.org/wiki/Lanczos_resampling
 */ 

#include <memory>
#include <cstddef>
#include <glm/glm.hpp>
#include "../../../core_utils/buffer.h"

class Lanczos {
private:
    std::size_t a;
    glm::ivec2 dimIn, dimOut;
    Buffer<glm::vec3> output_buffer;
    static constexpr double pi_square = M_PI * M_PI;

public:
    /**
     * @brief Instance of an Lanczos upscaler
     * @param in_width      The width of the input image
     * @param in_height     The height of the input image
     * @param out_width     The width of the output image
     * @param out_height    The height of the output image
     * @param a             The window size of the Lanczos algorithm
     */
    Lanczos(std::size_t in_width, std::size_t in_height, std::size_t out_width, std::size_t out_height, std::size_t a) {
        this->a = a;
        this->dimIn = {in_width, in_height};
        this->dimOut = {out_width, out_height};
        this->output_buffer = buffer_allocate<glm::vec3>(out_width * out_height);
    }

    ~Lanczos() {
        buffer_free(output_buffer);
    }

    Buffer<glm::vec3>* upscale(Buffer<glm::vec3>& in_buffer) {
        double x_ratio = static_cast<double>(dimIn.x) / dimOut.x;
        double y_ratio = static_cast<double>(dimIn.y) / dimOut.y;

        for (std::size_t y = 0; y < dimOut.y; y++) {
            for (std::size_t x = 0; x < dimOut.x; x++) {
                double src_x = x * x_ratio;
                double src_y = y * y_ratio;

                std::size_t x_int = static_cast<std::size_t>(src_x);
                std::size_t y_int = static_cast<std::size_t>(src_y);

                glm::vec3 value = glm::vec3(0.0, 0.0, 0.0);
                double sum = 0.0;

                for (std::size_t j = y_int - a + 1; j <= y_int + a; j++) {
                    for (std::size_t i = x_int - a + 1; i <= x_int + a; i++) {
                        if (i >= 0 && i < dimIn.x && j >= 0 && j < dimIn.y) {
                            double weight = _calc_lanczos(src_x - i, a) * _calc_lanczos(src_y - j, a);
                            value += in_buffer.data[i + j * dimIn.x] * static_cast<float>(weight);
                            sum += weight;
                        }
                    }
                }

                output_buffer.data[x + y * dimOut.x] = value / static_cast<float>(sum);
            }
        } 

        return &output_buffer;
    }

private:
    double _calc_lanczos(double x, std::size_t a) {
        if (x == 0.0) return 1.0;
        if (x > a || x < -a) return 0.0;
        return (a * std::sin(M_PI * x) * std::sin(M_PI * x / a)) / (pi_square * x * x);
    } 
};
