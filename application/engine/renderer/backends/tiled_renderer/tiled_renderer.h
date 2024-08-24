#pragma once

#include "../../../../core_utils/timer.h"
#include "../render_backend.h"

#include "types.h"
#include "vertex_shader.h"
#include "vertex_post_processor.h"
#include "primitive_assembler.h"
#include "primitive_binner.h"
#include "rasterizer.h"
#include "fragment_shader.h"

namespace TR {
class TiledRenderer : public RenderBackend {
    private:
        glm::ivec2 tileDim = glm::ivec2(32, 32);

        std::size_t num_tiles;
        std::size_t num_horizontal_tiles;
        std::size_t num_vertical_tiles;
        std::size_t num_triangles_per_tile;
        std::vector<TR::Tile> tiles;

        std::size_t max_num_tris = 0;

    public:
        TiledRenderer(std::size_t render_width, std::size_t render_height, Settings_t *app_settings)
            : RenderBackend(render_width, render_height, app_settings) {
                num_horizontal_tiles = std::round((float)(render_width) / tileDim.x);
                num_vertical_tiles = std::round((float)(render_height) / tileDim.y);
                num_tiles = num_vertical_tiles * num_horizontal_tiles;
                num_triangles_per_tile = 1024;
                tiles.reserve(num_tiles);

                for (std::size_t y = 0; y < num_vertical_tiles; y++) {
                    for (std::size_t x = 0; x < num_horizontal_tiles; x++) {
                        TR::Tile tile;
                        tile.min = glm::ivec2(x * tileDim.x, y * tileDim.y);
                        tile.max = glm::ivec2(std::min((x + 1) * tileDim.x, view_width), 
                                              std::min((y + 1) * tileDim.y, view_height));
                        tiles.push_back(tile);
                        tiles.back().triangles.reserve(num_triangles_per_tile);
                    }
                }
            }

        ~TiledRenderer() override {
            buffer_free(framebuffer);
            buffer_free(depth_buffer);
        }

        int Render(const glm::mat4 &view, const glm::mat4 &proj) override {
            Timer timer;

            // Clear buffers
            ClearBuffers();

            // Vertex shader
            
            // Vertex post-processor
            
            // Primitive assembler
            
            // Primitive binner

            // Rasterizer

            // Fragment shader

            return 0;
        }
};
}

