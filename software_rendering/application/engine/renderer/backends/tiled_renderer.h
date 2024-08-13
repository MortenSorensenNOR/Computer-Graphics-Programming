#pragma once

#include "../../../core_utils/core_utils.h"
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
        buffer_free(depth_buffer);
    }

    int Render(const glm::mat4& view, const glm::mat4& proj) override {
        std::vector<Buffer<glm::vec4>> projected_vertex_data = {};
        projected_vertex_data.reserve(render_queue.size());

        for (std::size_t i = 0; i < render_queue.size(); i++) {
            RenderObject* object = &render_queue.at(i);
            Buffer<glm::vec4> projected_vertex_buffer = buffer_allocate<glm::vec4>(object->mesh->vertexes.size());

            TileRender::vertex_shader(object->mesh->vertexes, object->model, view, proj, projected_vertex_buffer); 
            projected_vertex_data.push_back(projected_vertex_buffer);
        }

        for (std::size_t i = 0; i < projected_vertex_data.size(); i++) {
            for (std::size_t j = 0; j < projected_vertex_data.at(i).size(); j++) {

            }
        }

        for (std::size_t i = 0; i < projected_vertex_data.size(); i++) {
            buffer_free<glm::vec4>(projected_vertex_data.at(i));
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
    
    int BindTexture(std::size_t texture_id, TextureType type) final {
        return 0;
    }

    int BindMesh(std::size_t mesh_id) final {
        return 0;
    }
};

static void TileRender::vertex_shader(Buffer<glm::vec4>& vertex_in, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, Buffer<glm::vec4>& vertex_out) {
    for (std::size_t i = 0; i < vertex_out.size(); i++) {
        vertex_out.data[i] = proj * view * model * vertex_in.data[i];
    }
};
