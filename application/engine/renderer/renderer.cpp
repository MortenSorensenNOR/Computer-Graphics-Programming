#include "renderer.h"

int renderer_init(Renderer* renderer, std::size_t render_width, std::size_t render_height) {
    renderer->render_width = render_width;
    renderer->render_height = render_height;
    return 0;
}

int renderer_set_backend(Renderer* renderer, std::unique_ptr<RenderBackend> backend) {
    renderer->backend = std::move(backend);
    return 0;
}

void renderer_add_to_render_queue(Renderer* renderer, RenderObject& object) {
    return renderer->backend->RenderQueueAdd(object);
}

int renderer_render(Renderer* renderer, const glm::mat4& view, const glm::mat4& projection) {
    return renderer->backend->Render(view, projection);
}

Buffer<glm::vec3>* renderer_get_fb(Renderer* renderer) {
    return renderer->backend->GetFramebuffer();
}

int renderer_free(Renderer* renderer) {
    return 0;
}
