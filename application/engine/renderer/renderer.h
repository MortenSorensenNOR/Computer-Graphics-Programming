#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "../../core_utils/core_utils.h"
#include "../engine_utils/render_object.h"

#include "backends/render_backend.h"
#include "post_process/bilinear_upscale.h"
#include "post_process/fxaa.h"
#include "post_process/lanczos_upscale.h"

struct Renderer {
    std::size_t render_width, render_height;
    std::unique_ptr<RenderBackend> backend; 
    std::unique_ptr<Bilinear> upscaler;
    std::unique_ptr<FXAA> fxaa;
};

int renderer_init(Renderer* renderer, std::size_t render_width, std::size_t render_height);

int renderer_set_backend(Renderer* renderer, std::unique_ptr<RenderBackend> backend);

void renderer_add_to_render_queue(Renderer* renderer, RenderObject& object);

int renderer_render(Renderer* renderer, const glm::mat4& view, const glm::mat4& projection);

Buffer<glm::vec3>* renderer_get_fb(Renderer* renderer);

int renderer_free(Renderer* renderer);
