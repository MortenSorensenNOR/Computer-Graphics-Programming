#pragma once

#include "render_backend.h"

namespace TileRender {
struct Tile {
    std::size_t min_x, min_y;
    std::size_t max_x, max_y;
    std::size_t id;

    Tile (glm::ivec2 top_left, glm::ivec2 bottom_right, std::size_t id) {
        this->min_x = top_left.x;
        this->min_y = top_left.y;
        this->max_x = bottom_right.x;
        this->max_y = bottom_right.y;
        this->id = id;
    }
};

static void vertex_shader(Buffer<glm::vec4>& vertex_in, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, Buffer<glm::vec4>& vertex_out);

}

class TiledRenderer : public RenderBackend {
private:
    static const std::size_t tile_size = 32;
    std::size_t num_tiles;
    std::size_t num_horizontal_tiles;
    std::size_t num_vertical_tiles;
    std::vector<TileRender::Tile> tiles;

public:
    TiledRenderer(std::size_t render_width, std::size_t render_height) 
        : RenderBackend(render_width, render_height) {
        num_horizontal_tiles = std::round((float)(render_width) / tile_size);
        num_vertical_tiles = std::round((float)(render_height) / tile_size);
        num_tiles = num_vertical_tiles * num_horizontal_tiles;
        tiles.reserve(num_tiles);

        for (std::size_t y = 0; y < num_vertical_tiles; y++) {
            for (std::size_t x = 0; x < num_horizontal_tiles; x++) {
                glm::ivec2 tile_min, tile_max;
                tile_min.x = x * tile_size;
                tile_min.y = y * tile_size;
                tile_max.x = std::min((x + 1) * tile_size, view_width);
                tile_max.y = std::min((y + 1) * tile_size, view_height);
                tiles.push_back(TileRender::Tile(tile_min, tile_max, x + y));
            }
        }
    }

    ~TiledRenderer() override {
        buffer_free(framebuffer);
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
                    framebuffer.data[index + 0] = colors[tile->id % colors.size()].x;
                    framebuffer.data[index + 1] = colors[tile->id % colors.size()].y;
                    framebuffer.data[index + 2] = colors[tile->id % colors.size()].z;
                }
            }
        }

        std::vector<RenderObject> empty;
        std::swap(render_queue, empty);

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

static void TileRender::vertex_shader(Buffer<glm::vec4>& vertex_in, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, Buffer<glm::vec4>& vertex_out) {
    for (std::size_t i = 0; i < vertex_out.size; i++) {
        vertex_out.data[i] = proj * view * model * vertex_in.data[i];
    }
};
