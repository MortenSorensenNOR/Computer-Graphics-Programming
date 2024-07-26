#pragma once

#include "render_backend.h"

class SimpleRenderer : public RenderBackend {
public:
    SimpleRenderer(std::size_t render_resolution_width, std::size_t render_resolution_height) 
        : RenderBackend(render_resolution_width, render_resolution_height) {}

    ~SimpleRenderer() override {
        buffer_free(framebuffer);
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
