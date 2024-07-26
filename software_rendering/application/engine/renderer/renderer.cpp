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

int renderer_render_object(Renderer* renderer, RenderObject& object, const glm::mat4& view, const glm::mat4& projection) {
    return renderer->backend->RenderObject(object, view, projection);
}

Buffer<char>* renderer_get_fb(Renderer* renderer) {
    return renderer->backend->GetFramebuffer();
}

int renderer_free(Renderer* renderer) {
    return 0;
}
