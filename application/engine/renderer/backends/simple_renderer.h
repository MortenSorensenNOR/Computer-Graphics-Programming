#pragma once

#include "render_backend.h"
#include "../../engine_utils/render_object.h"
#include <cmath>

namespace SimpleRender {
static inline glm::vec4 vertex_shader(const glm::vec4& pos, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
static inline glm::vec4 viewport_transform(const glm::vec4& pos, std::size_t view_width, std::size_t view_height);
static inline bool EdgeFunction(const glm::vec3& E, const glm::vec3& sample);
}

class SimpleRenderer : public RenderBackend {
private:

public:
    SimpleRenderer(std::size_t render_resolution_width, std::size_t render_resolution_height, Settings_t* app_settings) 
        : RenderBackend(render_resolution_width, render_resolution_height, app_settings) {}

    ~SimpleRenderer() override {
        buffer_free(framebuffer);
        buffer_free(depth_buffer);
    }

    int Render(const glm::mat4& view, const glm::mat4& projection) override {
        for (std::size_t i = 0; i < render_queue.size(); i++) {
            RenderObject object = render_queue.at(i); 

            std::size_t num_tris = object.mesh->indices.size() / 3;
            Buffer<glm::vec4>* vertex_buffer = &object.mesh->vertexes;
            Buffer<std::size_t>* index_buffer = &object.mesh->indices;

            for (std::size_t i = 0; i < num_tris; ++i) {
                std::size_t offset = 3 * i;
                const glm::vec4& v0 = vertex_buffer->data[index_buffer->data[offset]];
                const glm::vec4& v1 = vertex_buffer->data[index_buffer->data[offset + 1]];
                const glm::vec4& v2 = vertex_buffer->data[index_buffer->data[offset + 2]];

                glm::vec4 v0Clip = SimpleRender::vertex_shader(v0, object.model, view, projection);
                glm::vec4 v1Clip = SimpleRender::vertex_shader(v1, object.model, view, projection);
                glm::vec4 v2Clip = SimpleRender::vertex_shader(v2, object.model, view, projection);

                glm::vec4 v0Homogen = SimpleRender::viewport_transform(v0Clip, view_width, view_height);
                glm::vec4 v1Homogen = SimpleRender::viewport_transform(v1Clip, view_width, view_height);
                glm::vec4 v2Homogen = SimpleRender::viewport_transform(v2Clip, view_width, view_height);

                glm::mat3 M = {
                    { v0Homogen.x, v1Homogen.x, v2Homogen.x},
                    { v0Homogen.y, v1Homogen.y, v2Homogen.y},
                    { v0Homogen.w, v1Homogen.w, v2Homogen.w},
                };

                // Back-face culling
                float det = glm::determinant(M);
                if (det >= 0.0f)
                    continue;

                M = glm::inverse(M);

                // Edge function
                glm::vec3 E0 = M[0];
                glm::vec3 E1 = M[1];
                glm::vec3 E2 = M[2];

                // Constant to interpolate 1/w
                glm::vec3 C = M * glm::vec3(1, 1, 1);

                for (int y = 0; y < view_height; y++) {
                    for (int x = 0; x < view_width; x++) {
                        glm::vec3 sample = {x + 0.5, y + 0.5, 1.0};

                        bool inside0 = SimpleRender::EdgeFunction(E0, sample);
                        bool inside1 = SimpleRender::EdgeFunction(E1, sample);
                        bool inside2 = SimpleRender::EdgeFunction(E2, sample);

                        if (inside0 && inside1 && inside2) {
                            float oneOverW = (C.x * sample.x) + (C.y * sample.y) + C.z;

                            std::size_t offset = x + y * view_width;
                            if (oneOverW >= depth_buffer.data[offset]) {
                                depth_buffer.data[offset] = oneOverW;

                                if (!app_settings->depthView) {
                                    framebuffer.data[offset]   = colors[index_buffer->data[i] % 6]; 
                                } else {
                                    framebuffer.data[offset] = {oneOverW, oneOverW, oneOverW}; 
                                }

                            }
                        }
                    }
                }
            }
        }

        std::vector<RenderObject> empty;
        std::swap(render_queue, empty);

        return 0;
    }
};

static inline glm::vec4 SimpleRender::vertex_shader(const glm::vec4& pos, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    return projection * view * model * pos;
};

static inline glm::vec4 SimpleRender::viewport_transform(const glm::vec4& pos, std::size_t view_width, std::size_t view_height) {
    return glm::vec4((view_width * (pos.x + pos.w) / 2.0), view_height * (pos.w - pos.y) / 2, pos.z, pos.w);
}

static inline bool SimpleRender::EdgeFunction(const glm::vec3& E, const glm::vec3& sample) {
    float result = (E.x * sample.x) + (E.y * sample.y) + E.z;

    if (result > 0.0f) return true;
    else if (result < 0.0f) return false;

    if (E.x > 0.0f) return true;
    else if (E.x < 0.0f) return false;

    if ((E.x == 0.0f) && (E.y < 0.0f)) return false;

    return true;
}