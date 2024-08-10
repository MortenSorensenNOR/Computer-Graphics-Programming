#include "engine.h"

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height) {
    scene_init(&engine->scene, screen_width, screen_height);
    renderer_init(&engine->renderer, screen_width, screen_height);

    std::unique_ptr<RenderBackend> backend = std::make_unique<SimpleRenderer>(screen_width, screen_height);
    renderer_set_backend(&engine->renderer, std::move(backend));

    return 0;
}

int engine_run(Engine_t* engine, float dt, State_t* app_state, Settings_t* settings, InputState* input) {
    scene_update(&engine->scene);

    // TODO: Put camera movement into a seperate file -> prob. inside an engine logic submodule
    if (input->getKeyState('w')) {
        engine->scene.camera.pos += 8 * dt * glm::normalize(engine->scene.camera.forward);
    }
    if (input->getKeyState('s')) {
        engine->scene.camera.pos -= 8 * dt * glm::normalize(engine->scene.camera.forward);
    }
    if (input->getKeyState('a')) {
        engine->scene.camera.pos -= 8 * dt * glm::normalize(engine->scene.camera.right);
    }
    if (input->getKeyState('d')) {
        engine->scene.camera.pos += 8 * dt * glm::normalize(engine->scene.camera.right);
    }
    if (input->getModifierKeyState(WindowModifierKeys::SPACE)) {
        engine->scene.camera.pos += 4 * dt * glm::normalize(engine->scene.camera.world_up); 
    }
    if (input->getModifierKeyState(WindowModifierKeys::LSHIFT)) {
        engine->scene.camera.pos -= 4 * dt * glm::normalize(engine->scene.camera.world_up); 
    }

    if (app_state->mouse_captured) {
        float xoffset = input->mouse_delta_x * camera_mouse_sensitivity;
        float yoffset = -input->mouse_delta_y * camera_mouse_sensitivity;

        engine->scene.camera.yaw += xoffset;
        engine->scene.camera.pitch += yoffset;

        if (engine->scene.camera.pitch > 89.0f)
            engine->scene.camera.pitch = 89.0f;
        if (engine->scene.camera.pitch < -89.0f)
            engine->scene.camera.pitch = -89.0f;
    }

    camera_update_vectors(&engine->scene.camera);
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
