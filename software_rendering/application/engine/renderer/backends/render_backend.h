#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "../../../core_utils/core_utils.h"
#include "../../engine_utils/texture.h"
#include "../../engine_utils/mesh.h"
#include "../../engine_utils/render_object.h"

class RenderBackend {
public:
    std::size_t view_width;
    std::size_t view_height;

    Buffer<char> framebuffer;
    Buffer<float> depth_buffer;

public:
    RenderBackend(std::size_t render_resolution_width, std::size_t render_resolution_height) {
        view_width = render_resolution_width;
        view_height = render_resolution_height;

        std::size_t fb_size = render_resolution_width * render_resolution_height * 3;
        framebuffer = buffer_allocate<char>(fb_size); 

        std::size_t depth_buffer_size = render_resolution_width * render_resolution_height;
        depth_buffer = buffer_allocate<float>(depth_buffer_size);
    }

    virtual ~RenderBackend() = default;

    Buffer<char>* GetFramebuffer() {
        return &framebuffer;
    }

    int ClearBuffers() {
        buffer_fill<char>(framebuffer, 0x00);
        buffer_fill<float>(depth_buffer, 0.0f);
        return 0;
    }

    /**
     * @brief Renders a RenderObject to the framebuffer 
     * @return Returns 0 if success, else returns error code
     */
    int virtual RenderObject(struct RenderObject& object, const glm::mat4& view, const glm::mat4& projection) = 0;

    /**
     * @brief Creates a texture and loads it into the memory of the render device, i.e. either CPU or GPU memory
     * @return The texture ID of the created texture
     */
    int virtual CreateTexture(Texture<glm::vec3>& texture) = 0;

    int virtual CreateMeshBuffer(Mesh& mesh) = 0;

    int virtual BindTexture(int texture_id, TextureType type) = 0;

    int virtual BindMesh(int mesh_id) = 0;
};
