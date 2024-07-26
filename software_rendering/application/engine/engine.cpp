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

    glm::mat4 model = glm::mat4(1.0f);
    engine->testObj->model = glm::translate(model, glm::vec3(0, 0, 2.0f));

    buffer_free(vertexes);
    buffer_free(indices);

    return 0;
}

int engine_run(Engine_t* engine, float dt, Settings_t* settings) {
    scene_update(&engine->scene);

    float nearPlane = 0.1f;
    float farPlane = 100.f;
    glm::vec3 eye(0, 3.75, 6.5);
    glm::vec3 lookat(0, 0, 0);
    glm::vec3 up(0, 1, 0);

    glm::mat4 view = glm::lookAt(eye, lookat, up);
    glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)engine->renderer.render_width / (float)engine->renderer.render_height, nearPlane, farPlane);

    engine->testObj->model = glm::rotate(engine->testObj->model, glm::radians(45.0f * dt), glm::vec3(0, 1, 0));

    engine->renderer.backend->ClearBuffers();
    renderer_render_object(&engine->renderer, *engine->testObj, view, projection);

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
