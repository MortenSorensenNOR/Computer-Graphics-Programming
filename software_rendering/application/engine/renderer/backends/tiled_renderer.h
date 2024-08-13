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

    Tile (glm::ivec2 tileDim, glm::ivec2 top_left, glm::ivec2 bottom_right, std::size_t id) {
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

static void vertex_shader(Buffer<glm::vec4>& vertex_in, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, Buffer<glm::vec4>& vertex_out);
static void fragment_shader(Fragment& frag, glm::vec<3, char, glm::lowp>& out);

class TiledRenderer : public RenderBackend {
private:
    glm::ivec2 tileDim = glm::ivec2(32, 32);
    std::size_t num_tiles;
    std::size_t num_horizontal_tiles;
    std::size_t num_vertical_tiles;
    std::vector<TileRender::Tile> tiles;

public:
    TiledRenderer(std::size_t render_width, std::size_t render_height) 
        : RenderBackend(render_width, render_height) {
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

    int Render(const glm::mat4& view, const glm::mat4& proj) override {
        Timer timer, total_timer;
        total_timer.start();

        // Vertex shader
        timer.start();
        std::vector<Buffer<glm::vec4>> view_space_vertex_data = {};
        view_space_vertex_data.reserve(render_queue.size());

        vertex_shader_stage(view, proj, view_space_vertex_data);
        timer.stop();
        printf("Vertex shader: %f us\n", timer.get());
             
        // Create triangles and do backface culling
        timer.start();
        std::vector<ProjectedTriangle> clipped_triangles; 
        clipped_triangles.reserve(render_queue_index_size / 3);
        clipping_stage(view_space_vertex_data, clipped_triangles);

        timer.stop();
        printf("Clipping stage: %f us\n", timer.get());

        // Clean up vertex shader
        for (std::size_t i = 0; i < view_space_vertex_data.size(); i++) {
            buffer_free(view_space_vertex_data.at(i));
        }

        // Transform to screen_space
        timer.start();
        std::vector<ScreenSpaceTriangle> screen_space_triangles;
        screen_space_triangles.reserve(clipped_triangles.size());
        screen_space_transform(clipped_triangles, screen_space_triangles);

        timer.stop();
        printf("Screen space transform stage: %f us\n", timer.get());
        
        // Assign triangles to tiles
        timer.start();
        assign_triangles_tiles(screen_space_triangles);

        timer.stop();
        printf("Assign tris to tiles stage: %f us\n", timer.get());

        // Rasterize tiles
        rasterization_stage();
        
        // Fragment shader
        fragment_shader_stage();

        total_timer.stop();
        printf("Total render time: %f us\n\n", total_timer.get());

        return 0;
    }

private:
    void vertex_shader_stage(const glm::mat4& view, const glm::mat4& proj, std::vector<Buffer<glm::vec4>>& output) {
        for (std::size_t i = 0; i < render_queue.size(); i++) {
            RenderObject* object = &render_queue.at(i);

            Buffer<glm::vec4> projected_vertex_buffer = buffer_allocate<glm::vec4>(object->mesh->vertexes.size());
            output.push_back(projected_vertex_buffer);

            vertex_shader(object->mesh->vertexes, object->model, view, proj, projected_vertex_buffer); 
        }
    }     

    void clipping_stage(std::vector<Buffer<glm::vec4>>& proj_vertexes, std::vector<TileRender::ProjectedTriangle>& clipped_tris) {
        for (std::size_t i = 0; i < render_queue.size(); i++) {
            Buffer<glm::vec4>* proj_vert_buff = &proj_vertexes.at(i);
            RenderObject* object = & render_queue.at(i);

            for (std::size_t idx = 0; idx < object->mesh->indices.size(); idx += 3) {
                ProjectedTriangle t;
                t.v0 = proj_vert_buff->data[idx];
                t.v1 = proj_vert_buff->data[idx+1];
                t.v2 = proj_vert_buff->data[idx+2];
                t.mesh_id = i;

                glm::vec3 edge1 = glm::vec3(t.v1 - t.v0);
                glm::vec3 edge2 = glm::vec3(t.v2 - t.v0);
                glm::vec3 face_normal = glm::cross(edge1, edge2);

                if (glm::dot(face_normal, glm::vec3(t.v0)) > 0.0f) {
                    clipped_tris.push_back(t);
                }
            }
        }
    }

    void screen_space_transform(std::vector<ProjectedTriangle>& clipped_tris, std::vector<ScreenSpaceTriangle>& screen_space_tris) {
        for (std::size_t i = 0; i < clipped_tris.size(); i++) {
            ProjectedTriangle* t = &clipped_tris.at(i);
            ScreenSpaceTriangle spt;
            spt.p0 = glm::vec2(t->v0.x / t->v0.w, t->v0.y / t->v0.w);
            spt.p1 = glm::vec2(t->v1.x / t->v1.w, t->v1.y / t->v1.w);
            spt.p2 = glm::vec2(t->v2.x / t->v2.w, t->v2.y / t->v2.w);

            spt.p0 = 0.5f * glm::vec2(view_width, view_height) * (spt.p0 + glm::vec2(1.0f));
            spt.p1 = 0.5f * glm::vec2(view_width, view_height) * (spt.p1 + glm::vec2(1.0f));
            spt.p2 = 0.5f * glm::vec2(view_width, view_height) * (spt.p2 + glm::vec2(1.0f));

            float minx = std::min(spt.p0.x, std::min(spt.p1.x, spt.p2.x));
            float miny = std::min(spt.p0.y, std::min(spt.p1.y, spt.p2.y));
            float maxx = std::max(spt.p0.x, std::max(spt.p1.x, spt.p2.x));
            float maxy = std::max(spt.p0.y, std::max(spt.p1.y, spt.p2.y));

            spt.top_left = glm::floor(glm::vec2(minx, miny));
            spt.bottom_right = glm::ceil(glm::vec2(maxx, maxy));

            spt.mesh_id = t->mesh_id;

            screen_space_tris.push_back(spt);
        }
    }

    void assign_triangles_tiles(std::vector<ScreenSpaceTriangle>& screen_space_triangles) {
        for (std::size_t tileID = 0; tileID < tiles.size(); tileID++) {
            Tile* tile = &tiles.at(tileID);
            tile->clear();
            tile->triangle_ids.reserve(512);

            for (std::size_t i = 0; i < screen_space_triangles.size(); i++) {
                ScreenSpaceTriangle* spt = &screen_space_triangles.at(i);

                if (spt->top_left.x >= tile->min_x && spt->top_left.y >= tile->min_y && spt->top_left.x <= tile->max_x && spt->top_left.y <= tile->max_y&& spt->top_left.x <= tile->max_x && spt->top_left.y <= tile->max_y) {
                    tile->triangle_ids.push_back(i);
                } else if (spt->bottom_right.x >= tile->min_x && spt->bottom_right.y >= tile->min_y && spt->top_left.x <= tile->max_x && spt->top_left.y <= tile->max_y&& spt->bottom_right.x <= tile->max_x && spt->bottom_right.y <= tile->max_y) {
                    tile->triangle_ids.push_back(i);
                }
            }
        }        
    }

    void rasterization_stage() {
        for (std::size_t tileID = 0; tileID < tiles.size(); tileID++) {
            Tile* tile = &tiles.at(tileID);

            for (std::size_t triID = 0; triID < tile->triangle_ids.size(); triID++) {
                 
            }
        }
    }

    void fragment_shader_stage() {
        for (std::size_t tileID = 0; tileID < tiles.size(); tileID++) {
            Tile* tile = &tiles.at(tileID);

        }
    }
};

}

static void TileRender::vertex_shader(Buffer<glm::vec4>& vertex_in, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, Buffer<glm::vec4>& vertex_out) {
    for (std::size_t i = 0; i < vertex_out.size(); i++) {
        vertex_out.data[i] = proj * view * model * vertex_in.data[i];
    }
};

static void TileRender::fragment_shader(Fragment& frag, glm::vec<3, char, glm::lowp>& out) {

}
