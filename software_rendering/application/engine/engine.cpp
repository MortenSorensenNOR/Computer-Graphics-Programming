#include "engine.h"

int engine_init(Engine_t* engine, std::size_t screen_width, std::size_t screen_height) {
    scene_init(&engine->scene, screen_width, screen_height);
    renderer_init(&engine->renderer, screen_width, screen_height);

    std::unique_ptr<RenderBackend> backend = std::make_unique<SimpleRenderer>(screen_width, screen_height);
    renderer_set_backend(&engine->renderer, std::move(backend));

    Buffer<glm::vec4> vertexes = buffer_allocate<glm::vec4>(8);
    Buffer<std::size_t> indices = buffer_allocate<std::size_t>(36);

    vertexes.data[0] = {  1.0f, -1.0f, -1.0f, 1.0f };
    vertexes.data[1] = {  1.0f, -1.0f,  1.0f, 1.0f };
    vertexes.data[2] = { -1.0f, -1.0f,  1.0f, 1.0f };
    vertexes.data[3] = { -1.0f, -1.0f, -1.0f, 1.0f };
    vertexes.data[4] = {  1.0f,  1.0f, -1.0f, 1.0f };
    vertexes.data[5] = {  1.0f,  1.0f,  1.0f, 1.0f };
    vertexes.data[6] = { -1.0f,  1.0f,  1.0f, 1.0f };
    vertexes.data[7] = { -1.0f,  1.0f, -1.0f, 1.0f };

    indices.data[0] = 1; indices.data[1] = 3; indices.data[2] = 0; 
    indices.data[3] = 7; indices.data[4] = 5; indices.data[5] = 4; 
    indices.data[6] = 4; indices.data[7] = 1; indices.data[8] = 0;
    indices.data[9] = 5; indices.data[10] = 2; indices.data[11] = 1;
    indices.data[12] = 2; indices.data[13] = 7; indices.data[14] = 3;
    indices.data[15] = 0; indices.data[16] = 7; indices.data[17] = 4;
    indices.data[18] = 1; indices.data[19] = 2; indices.data[20] = 3;
    indices.data[21] = 7; indices.data[22] = 6; indices.data[23] = 5;
    indices.data[24] = 4; indices.data[25] = 5; indices.data[26] = 1;
    indices.data[27] = 5; indices.data[28] = 6; indices.data[29] = 2;
    indices.data[30] = 2; indices.data[31] = 6; indices.data[32] = 7;
    indices.data[33] = 0; indices.data[34] = 3; indices.data[35] = 7;

    engine->testObj = new RenderObject(vertexes, indices);

    engine->model1 = glm::mat4(1.0f);
    engine->model1 = glm::translate(engine->model1, glm::vec3(0, 0, 2.0f));

    engine->model2 = glm::mat4(1.0f);
    engine->model2 = glm::translate(engine->model2, glm::vec3(-3.75f, 0, 0));

    engine->model3 = glm::mat4(1.0f);
    engine->model3 = glm::translate(engine->model3, glm::vec3(3.75f, 0, 0));

    engine->model4 = glm::mat4(1.0f);
    engine->model4 = glm::translate(engine->model4, glm::vec3(0, 0, -2.0f));

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

    engine->model1 = glm::rotate(engine->model1, glm::radians(45.f * dt), glm::vec3(0, 1, 0));
    engine->model2 = glm::rotate(engine->model2, glm::radians(30.f * dt), glm::vec3(1, 0, 0));
    engine->model3 = glm::rotate(engine->model3, glm::radians(60.f * dt), glm::vec3(0, 1, 0));
    engine->model4 = glm::rotate(engine->model4, glm::radians(90.f * dt), glm::vec3(0, 0, 1));
    
    engine->renderer.backend->ClearBuffers();

    engine->testObj->model = engine->model1;
    renderer_render_object(&engine->renderer, *engine->testObj, *view, *projection);

    engine->testObj->model = engine->model2;
    renderer_render_object(&engine->renderer, *engine->testObj, *view, *projection);

    engine->testObj->model = engine->model3;
    renderer_render_object(&engine->renderer, *engine->testObj, *view, *projection);

    engine->testObj->model = engine->model4;
    renderer_render_object(&engine->renderer, *engine->testObj, *view, *projection);

    return 0;
}

Buffer<char>* engine_get_fb(Engine_t* engine) {
    return renderer_get_fb(&engine->renderer);
}

int engine_free(Engine_t* engine) {
    scene_free(&engine->scene);
    renderer_free(&engine->renderer);

    buffer_free(engine->testObj->mesh.vertexes);
    buffer_free(engine->testObj->mesh.indices);

    return 0;
}
