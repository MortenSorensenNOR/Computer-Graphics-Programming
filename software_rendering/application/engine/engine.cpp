#include "engine.h"

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height) {
    scene_init(&engine->scene, screen_width, screen_height);
    renderer_init(&engine->renderer, screen_width, screen_height);

    // Create a new renderer and assign it as the render backend
    std::unique_ptr<RenderBackend> backend = std::make_unique<TiledRenderer>(screen_width, screen_height);
    renderer_set_backend(&engine->renderer, std::move(backend));

    // Create a new camera controller
    engine->camera_controller = std::make_unique<CameraController>(&engine->scene.camera);

    return 0;
}

int engine_run(Engine_t* engine, float dt, State_t* app_state, Settings_t* settings, InputState* input) {
    // Update the scene transforms
    scene_update(&engine->scene);

    // Update camera
    engine->camera_controller->ControllsUpdate(input, app_state, dt);

    camera_update_vectors(&engine->scene.camera);
    camera_update_view(&engine->scene.camera);

    // Get camera matrices
    glm::mat4* view = camera_get_view(&engine->scene.camera);
    glm::mat4* projection = camera_get_projection(&engine->scene.camera);

    // Clear render buffers
    engine->renderer.backend->ClearBuffers();

    // Add scene objects to the render queue
    for (std::size_t i = 0; i < engine->scene.scene_root->children->size(); i++) {
        SceneObject_t* so = engine->scene.scene_root->children->at(i);
    
        for (std::size_t mesh_index = 0; mesh_index < so->mesh_indices->size(); mesh_index++) {
            Mesh* mesh = &engine->scene.meshes->at(so->mesh_indices->at(mesh_index));
            RenderObject ro(mesh);
            ro.model = so->model;
    
            renderer_add_to_render_queue(&engine->renderer, ro);
        }
    }

    // Render
    renderer_render(&engine->renderer, *view, *projection);

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
