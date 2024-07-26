#pragma once

#include "render_backend.h"
#include "../../engine_utils/render_object.h"

namespace SimpleRender {
static inline glm::vec4 vertex_shader(const glm::vec4& pos, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);
static inline glm::vec4 viewport_transform(const glm::vec4& pos, std::size_t view_width, std::size_t view_height);
static inline int rasterizer();
static inline int fragment_shader();
}

class SimpleRenderer : public RenderBackend {
public:
    SimpleRenderer(std::size_t render_resolution_width, std::size_t render_resolution_height) 
        : RenderBackend(render_resolution_width, render_resolution_height) {}

    ~SimpleRenderer() override {
        buffer_free(framebuffer);
    }

    int RenderObject(struct RenderObject& object, const glm::mat4& view, const glm::mat4& projection) override {
        std::size_t num_tris = object.mesh.indices.size / 3;
        Buffer<glm::vec4>* vertex_buffer = &object.mesh.vertexes;
        Buffer<std::size_t>* index_buffer = &object.mesh.indices;

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

        }


        return 0;
    }

    int CreateTexture(Texture<glm::vec3>& texture) override {
        return 0;
    }

    int CreateMeshBuffer(Mesh& mesh) override {
        return 0;
    }

    int BindTexture(int texture_id, TextureType type) override {
        return 0;
    }

    int BindMesh(int mesh_id) override {
        return 0;
    }
};

static inline glm::vec4 SimpleRender::vertex_shader(const glm::vec4& pos, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection) {
    return projection * view * model * pos;
};

static inline glm::vec4 SimpleRender::viewport_transform(const glm::vec4& pos, std::size_t view_width, std::size_t view_height) {
    return glm::vec4((view_width * (pos.x + pos.w) / 2.0), view_height * (pos.w - pos.y) / 2, pos.y, pos.w);
}

static inline int SimpleRender::rasterizer() {

    return 0;
}
