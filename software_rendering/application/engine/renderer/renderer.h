#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "../../core_utils/core_utils.h"
#include "../engine_utils/render_object.h"

#include "backends/render_backend.h"

struct Renderer {
    std::size_t render_width, render_height;
    std::unique_ptr<RenderBackend> backend; 
};

int renderer_init(Renderer* renderer, std::size_t render_width, std::size_t render_height);

int renderer_set_backend(Renderer* renderer, std::unique_ptr<RenderBackend> backend);

int renderer_render_object(Renderer* renderer, RenderObject& object, const glm::mat4& view, const glm::mat4& projection);

Buffer<char>* renderer_get_fb(Renderer* renderer);

int renderer_free(Renderer* renderer);
