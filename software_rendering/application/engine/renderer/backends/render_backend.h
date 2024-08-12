#pragma once

#include <memory>
#include <queue>

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

    std::vector<RenderObject> render_queue;

    std::vector<glm::vec<3, char, glm::lowp>> colors;

public:
    RenderBackend(std::size_t render_resolution_width, std::size_t render_resolution_height) {
        view_width = render_resolution_width;
        view_height = render_resolution_height;

        std::size_t fb_size = render_resolution_width * render_resolution_height * 3;
        framebuffer = buffer_allocate<char>(fb_size); 

        std::size_t depth_buffer_size = render_resolution_width * render_resolution_height;
        depth_buffer = buffer_allocate<float>(depth_buffer_size);

        // Used for debugging
        colors.push_back(glm::vec<3, char, glm::lowp>(0xdc, 0x14, 0x3c));
        colors.push_back(glm::vec<3, char, glm::lowp>(0x41, 0x69, 0xe1));
        colors.push_back(glm::vec<3, char, glm::lowp>(0x32, 0xcd, 0x32));
        colors.push_back(glm::vec<3, char, glm::lowp>(0xda, 0xa5, 0x20));
        colors.push_back(glm::vec<3, char, glm::lowp>(0xba, 0x55, 0xd3));
        colors.push_back(glm::vec<3, char, glm::lowp>(0x2f, 0x4f, 0x4f));

    }

    virtual ~RenderBackend() = default;

    Buffer<char>* GetFramebuffer() {
        return &framebuffer;
    }

    int ClearBuffers() {
        buffer_fill<char>(framebuffer, 0x00);
        buffer_fill<float>(depth_buffer, 0.0f);

        std::vector<RenderObject> empty;
        std::swap(render_queue, empty);
        
        return 0;
    }

    /**
     * @brief Renders a RenderObject to the framebuffer 
     * @return Returns 0 if success, else returns error code
     */
    int RenderQueueAdd(struct RenderObject& object) {
        render_queue.push_back(object);
        return 0;
    }

    int virtual Render(const glm::mat4& view, const glm::mat4& projection) = 0;

    /**
     * @brief Creates a texture and loads it into the memory of the render device, i.e. either CPU or GPU memory
     * @return The texture ID of the created texture
     */
    int virtual CreateTexture(Texture<glm::vec3>& texture) = 0;

    int virtual CreateMeshBuffer(Mesh& mesh) = 0;

    int virtual BindTexture(int texture_id, TextureType type) = 0;

    int virtual BindMesh(int mesh_id) = 0;
};
