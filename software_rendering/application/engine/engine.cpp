#include "engine.h"

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height) {
    scene_init(&engine->scene, screen_width, screen_height);
    renderer_init(&engine->renderer, screen_width, screen_height);

    std::unique_ptr<RenderBackend> backend = std::make_unique<SimpleRenderer>(screen_width, screen_height);
    renderer_set_backend(&engine->renderer, std::move(backend));

    Buffer<glm::vec4> vertexes = buffer_allocate<glm::vec4>(8);
    Buffer<std::size_t> indices = buffer_allocate<std::size_t>(36);

    buffer_free(vertexes);
    buffer_free(indices);

    return 0;
}

int engine_run(Engine_t* engine, float dt, Settings_t* settings) {
    scene_update(&engine->scene);

    engine->scene.camera.forward.x = settings->lookAt_x;
    engine->scene.camera.forward.y = settings->lookAt_y;
    engine->scene.camera.forward.z = settings->lookAt_z;
    camera_update_view(&engine->scene.camera);

    glm::mat4* view = camera_get_view(&engine->scene.camera);
    glm::mat4* projection = camera_get_projection(&engine->scene.camera);

    engine->renderer.backend->ClearBuffers();

    std::queue<SceneObject_t*> render_queue = {};
    for (std::size_t i = 0; i < engine->scene.scene_root->children->size(); i++) {
        render_queue.push(engine->scene.scene_root->children->at(i));
    }

    std::size_t j = 0;
    while (!render_queue.empty()) {
        SceneObject_t* so = render_queue.front();
        render_queue.pop();

        so->model = glm::rotate(so->model, glm::radians(45.0f * dt), glm::vec3(0, 1, 0));
        so->model = glm::rotate(so->model, glm::radians(25.0f * dt), glm::vec3(1, 0, 0));
        so->model = glm::rotate(so->model, glm::radians(-55.0f * dt), glm::vec3(0, 0, 1));
        for (std::size_t i = 0; i < so->mesh_indices->size(); i++) {
            Mesh* mesh = &engine->scene.meshes->at(so->mesh_indices->at(i));
            RenderObject ro(mesh);
            ro.model = so->model;

            renderer_render_object(&engine->renderer, ro, *view, *projection);
        }
    }

    return 0;
}

Buffer<char>* engine_get_fb(Engine_t* engine) {
    return renderer_get_fb(&engine->renderer);
}

int engine_free(Engine_t* engine) {
    scene_free(&engine->scene);
    renderer_free(&engine->renderer);

    return 0;
}
