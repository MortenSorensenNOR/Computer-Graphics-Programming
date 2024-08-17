#pragma once

#include <memory>
#include <queue>

#include <glm/glm.hpp>
#include "../../../core_utils/core_utils.h"
#include "../../engine_utils/texture.h"
#include "../../engine_utils/mesh.h"
#include "../../engine_utils/render_object.h"
#include "../../../state/settings.h"

class RenderBackend {
public:
    std::size_t view_width;
    std::size_t view_height;

    Buffer<glm::vec3> framebuffer;
    Buffer<float> depth_buffer;

    std::size_t render_queue_vertex_size;
    std::size_t render_queue_index_size;
    std::size_t render_queue_triangle_count;
    std::vector<RenderObject> render_queue;

    std::vector<glm::vec3> colors;

    Settings_t* app_settings;

public:
    RenderBackend(std::size_t render_resolution_width, std::size_t render_resolution_height, Settings_t* app_settings) {
        view_width = render_resolution_width;
        view_height = render_resolution_height;

        std::size_t fb_size = render_resolution_width * render_resolution_height * 3;
        framebuffer = buffer_allocate<glm::vec3>(fb_size); 

        std::size_t depth_buffer_size = render_resolution_width * render_resolution_height;
        depth_buffer = buffer_allocate<float>(depth_buffer_size);

        // Used for debugging
        colors.push_back(glm::vec3(0.8627, 0.0784, 0.2353));
        colors.push_back(glm::vec3(0.2549, 0.4118, 0.8824));
        colors.push_back(glm::vec3(0.1961, 0.8039, 0.1961));
        colors.push_back(glm::vec3(0.8549, 0.6471, 0.1255));
        colors.push_back(glm::vec3(0.7294, 0.3333, 0.8275));
        colors.push_back(glm::vec3(0.1843, 0.3098, 0.3098));

        this->app_settings = app_settings;
    }

    virtual ~RenderBackend() = default;

    Buffer<glm::vec3>* GetFramebuffer() {
        return &framebuffer;
    }

    int ClearBuffers(glm::vec3 clear_color = glm::vec3(0.0)) {
        buffer_fill<glm::vec3>(framebuffer, clear_color);
        buffer_fill<float>(depth_buffer, 0.0f);

        std::vector<RenderObject> empty;
        std::swap(render_queue, empty);
        render_queue_index_size = 0;
        render_queue_vertex_size = 0;
        render_queue_triangle_count = 0;
        
        return 0;
    }

    /**
     * @brief Renders a RenderObject to the framebuffer 
     * @return Returns 0 if success, else returns error code
     */
    void RenderQueueAdd(RenderObject& object) {
        RenderObject render_object_local = object;
        render_object_local.id = render_queue.size();

        render_queue_index_size += render_object_local.mesh->indices.size();
        render_queue_vertex_size = render_object_local.mesh->vertexes.size();
        render_queue_triangle_count = render_object_local.mesh->indices.size() / 3;

        render_queue.push_back(render_object_local);
    }

    int virtual Render(const glm::mat4& view, const glm::mat4& projection) = 0;

    void print_vec2(const glm::vec2& v, char* txt = NULL) {
        if (txt != NULL)
            printf("(%f, %f)\n", v.x, v.y);
        else
            printf("%s: (%f, %f)\n", txt, v.x, v.y);
    }
    
    void print_ivec2(const glm::ivec2& v, char* txt = NULL) {
        if (txt != NULL)
            printf("(%d, %d)\n", v.x, v.y);
        else
            printf("%s: (%d, %d)\n", txt, v.x, v.y);
    }

    template <typename T>
        void print_vec3(const glm::vec<3, T>& v, char* txt = NULL) {
            if (txt != NULL)
                printf("(%f, %f, %f)\n", v.x, v.y, v.z);
            else
                printf("%s: (%f, %f, %f)\n", txt, v.x, v.y, v.z);
        }

    template <typename T>
        void print_vec4(const glm::vec<4, T>& v, char* txt = NULL) {
            if (txt != NULL)
                printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
            else
                printf("%s: (%f, %f, %f, %f)\n", txt, v.x, v.y, v.z, v.w);
        }

    template <typename T>
        void print_mat4(const glm::mat<4, 4, T>& m, char* txt = NULL) {
            if (txt != NULL)
                printf("(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n\n", m[0][0], m[0][1], m[0][2], m[0][3], 
                        m[1][0], m[1][1], m[1][2], m[1][3],
                        m[2][0], m[2][1], m[2][2], m[2][3],
                        m[3][0], m[3][1], m[3][2], m[3][3]);
            else
                printf("%s: (%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n\n", txt, m[0][0], m[0][1], m[0][2], m[0][3], 
                        m[1][0], m[1][1], m[1][2], m[1][3],
                        m[2][0], m[2][1], m[2][2], m[2][3],
                        m[3][0], m[3][1], m[3][2], m[3][3]);
        }
};
