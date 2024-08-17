#pragma once

#include "../../../core_utils/timer.h"
#include "render_backend.h"

namespace TileRender {
struct Vertex {
    glm::vec4 pos;
    glm::vec3 normal;
    glm::vec2 uv;

    std::size_t mesh_id;
};

struct ProjectedTriangle {
    std::size_t mesh_id;
    glm::vec4 v0, v1, v2;
};

struct ScreenSpaceTriangle {
    std::size_t mesh_id;
    glm::vec2 p0, p1, p2;
    glm::vec4 v0, v1, v2;

    glm::ivec2 top_left, bottom_right;
};

struct Fragment {
    glm::ivec2 pos;
};

struct Tile {
    std::size_t min_x, min_y;
    std::size_t max_x, max_y;
    std::size_t id;

    std::vector<std::size_t> triangle_ids;
    std::vector<Fragment> fragments;

    Tile(glm::ivec2 tileDim, glm::ivec2 top_left, glm::ivec2 bottom_right, std::size_t id) {
        this->min_x = top_left.x;
        this->min_y = top_left.y;
        this->max_x = bottom_right.x;
        this->max_y = bottom_right.y;
        this->id = id;

        fragments.reserve(tileDim.x * tileDim.y);
    }

    void clear() {
        std::vector<std::size_t> tmp = {};
        std::swap(triangle_ids, tmp);

        std::vector<Fragment> f_tmp = {};
        std::swap(fragments, f_tmp);
    }
};

static void vertex_shader(Buffer<glm::vec4> &vertex_in, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj, Buffer<glm::vec4> &vertex_out);
static inline glm::vec4 viewport_transform(const glm::vec4 &pos, std::size_t view_width, std::size_t view_height);
static inline bool EdgeFunction(const glm::vec3 &E, const glm::vec3 &sample);

class TiledRenderer : public RenderBackend {
    private:
        glm::ivec2 tileDim = glm::ivec2(32, 32);
        std::size_t num_tiles;
        std::size_t num_horizontal_tiles;
        std::size_t num_vertical_tiles;
        std::vector<TileRender::Tile> tiles;

        std::size_t max_num_tris = 0;

    public:
        TiledRenderer(std::size_t render_width, std::size_t render_height, Settings_t *app_settings)
            : RenderBackend(render_width, render_height, app_settings) {
                num_horizontal_tiles = std::round((float)(render_width) / tileDim.x);
                num_vertical_tiles = std::round((float)(render_height) / tileDim.y);
                num_tiles = num_vertical_tiles * num_horizontal_tiles;
                tiles.reserve(num_tiles);

                for (std::size_t y = 0; y < num_vertical_tiles; y++) {
                    for (std::size_t x = 0; x < num_horizontal_tiles; x++) {
                        glm::ivec2 tile_min, tile_max;
                        tile_min.x = x * tileDim.x;
                        tile_min.y = y * tileDim.y;
                        tile_max.x = std::min((x + 1) * tileDim.x, view_width);
                        tile_max.y = std::min((y + 1) * tileDim.y, view_height);
                        tiles.push_back(TileRender::Tile(tileDim, tile_min, tile_max, x + y));
                    }
                }
            }

        ~TiledRenderer() override {
            buffer_free(framebuffer);
            buffer_free(depth_buffer);
        }

        int Render(const glm::mat4 &view, const glm::mat4 &proj) override {
            max_num_tris = 0;

            // Vertex shader
            std::vector<Buffer<glm::vec4>> view_space_vertex_data = {};
            view_space_vertex_data.reserve(render_queue.size());

            vertex_shader_stage(view, proj, view_space_vertex_data);

            // Create triangles and do backface culling
            std::vector<ProjectedTriangle> clipped_triangles;
            clipped_triangles.reserve(render_queue_index_size / 3);
            clipping_stage(view_space_vertex_data, clipped_triangles);

            // Clean up vertex shader
            for (std::size_t i = 0; i < view_space_vertex_data.size(); i++) {
                buffer_free(view_space_vertex_data.at(i));
            }

            return 0;
        }

    private:
        void vertex_shader_stage(const glm::mat4 &view, const glm::mat4 &proj, std::vector<Buffer<glm::vec4>> &output) {
            for (std::size_t i = 0; i < render_queue.size(); i++) {
                RenderObject *object = &render_queue.at(i);

                Buffer<glm::vec4> projected_vertex_buffer = buffer_allocate<glm::vec4>(object->mesh->vertexes.size());
                output.push_back(projected_vertex_buffer);

                vertex_shader(object->mesh->vertexes, object->model, view, proj, projected_vertex_buffer);
            }
        }

        void clipping_stage(std::vector<Buffer<glm::vec4>> &proj_vertexes, std::vector<TileRender::ProjectedTriangle> &clipped_tris) {
            for (std::size_t i = 0; i < render_queue.size(); i++) {
                Buffer<glm::vec4> *proj_vert_buff = &proj_vertexes.at(i);
                RenderObject *object = &render_queue.at(i);

                for (std::size_t idx = 0; idx < object->mesh->indices.size(); idx += 3) {
                    ProjectedTriangle t;
                    t.v0 = proj_vert_buff->data[idx];
                    t.v1 = proj_vert_buff->data[idx + 1];
                    t.v2 = proj_vert_buff->data[idx + 2];
                    t.mesh_id = i;
                    clipped_tris.push_back(t);
                }
            }
        }
};
}

static void TileRender::vertex_shader(Buffer<glm::vec4> &vertex_in, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj, Buffer<glm::vec4> &vertex_out) {
    for (std::size_t i = 0; i < vertex_out.size(); i++) {
        vertex_out.data[i] = proj * view * model * vertex_in.data[i];
    }
};

static inline glm::vec4 TileRender::viewport_transform(const glm::vec4 &pos, std::size_t view_width, std::size_t view_height) {
    return glm::vec4((view_width * (pos.x + pos.w) / 2.0), view_height * (pos.w + pos.y) / 2, pos.z, pos.w);
}
