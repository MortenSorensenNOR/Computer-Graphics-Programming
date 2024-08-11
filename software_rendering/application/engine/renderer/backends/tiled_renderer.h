#pragma once

#include "render_backend.h"

namespace TileRender {
struct Tile {
    std::size_t min_x, min_y;
    std::size_t max_x, max_y;

    glm::vec<3, char, glm::lowp> color;

    Tile (glm::ivec2 top_left, glm::ivec2 bottom_right, glm::vec<3, char, glm::lowp> c) {
        min_x = top_left.x;
        min_y = top_left.y;
        max_x = bottom_right.x;
        max_y = bottom_right.y;
        color = c;
    }
};
}

class TiledRenderer : public RenderBackend {
private:
    static const std::size_t tile_size = 32;
    std::size_t num_tiles;
    std::size_t num_horizontal_tiles;
    std::size_t num_vertical_tiles;
    std::vector<TileRender::Tile> tiles;

    std::vector<glm::vec<3, char, glm::lowp>> colors;

public:
    TiledRenderer(std::size_t render_width, std::size_t render_height) 
        : RenderBackend(render_width, render_height) {
        num_horizontal_tiles = std::round((float)(render_width) / tile_size);
        num_vertical_tiles = std::round((float)(render_height) / tile_size);
        num_tiles = num_vertical_tiles * num_horizontal_tiles;
        tiles.reserve(num_tiles);

        colors.push_back(glm::vec<3, char, glm::lowp>(0xdc, 0x14, 0x3c));
        colors.push_back(glm::vec<3, char, glm::lowp>(0x41, 0x69, 0xe1));
        colors.push_back(glm::vec<3, char, glm::lowp>(0x32, 0xcd, 0x32));
        colors.push_back(glm::vec<3, char, glm::lowp>(0xda, 0xa5, 0x20));
        colors.push_back(glm::vec<3, char, glm::lowp>(0xba, 0x55, 0xd3));
        colors.push_back(glm::vec<3, char, glm::lowp>(0x2f, 0x4f, 0x4f));

        for (std::size_t y = 0; y < num_vertical_tiles; y++) {
            for (std::size_t x = 0; x < num_horizontal_tiles; x++) {
                tiles.push_back(TileRender::Tile(glm::ivec2(x * tile_size, y * tile_size), glm::ivec2((x + 1) * tile_size, (y + 1) * tile_size), colors[(x + y) % colors.size()]));
            }
        }
    }

    ~TiledRenderer() override {
        buffer_free(framebuffer);
    }

    int RenderQueueAdd(struct RenderObject& object) override {
        render_queue.push_back(object);
        return 0;
    }

    int Render(const glm::mat4& view, const glm::mat4& projection) override {
        for (std::size_t i = 0; i < tiles.size(); i++) {
            TileRender::Tile* tile = &tiles[i];

            for (std::size_t y = tile->min_y; y < tile->max_y; y++) {
                for (std::size_t x = tile->min_x; x < tile->max_x; x++) {
                    if (x > view_width || y > view_height) {
                        continue;
                    }
                    std::size_t index = 3 * (x + y * view_width);
                    framebuffer.data[index + 0] = tile->color.x;
                    framebuffer.data[index + 1] = tile->color.y;
                    framebuffer.data[index + 2] = tile->color.z;
                }
            }
        }

        return 0;
    }

    int CreateTexture(Texture<glm::vec3>& texture) final {
        return 0;
    }

    int CreateMeshBuffer(Mesh& mesh) final {
        return 0;
    }

    int BindTexture(int texture_id, TextureType type) final {
        return 0;
    }

    int BindMesh(int mesh_id) final {
        return 0;
    }
};
