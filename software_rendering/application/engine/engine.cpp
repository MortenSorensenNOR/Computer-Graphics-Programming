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

    indices.data[0] = 1; indices.data[0] = 3; indices.data[0] = 0; 
    indices.data[0] = 7; indices.data[0] = 5; indices.data[0] = 4; 
    indices.data[0] = 4; indices.data[0] = 1; indices.data[0] = 0;
    indices.data[0] = 5; indices.data[0] = 2; indices.data[0] = 1;
    indices.data[0] = 2; indices.data[0] = 7; indices.data[0] = 3;
    indices.data[0] = 0; indices.data[0] = 7; indices.data[0] = 4;
    indices.data[0] = 1; indices.data[0] = 2; indices.data[0] = 3;
    indices.data[0] = 7; indices.data[0] = 6; indices.data[0] = 5;
    indices.data[0] = 4; indices.data[0] = 5; indices.data[0] = 1;
    indices.data[0] = 5; indices.data[0] = 6; indices.data[0] = 2;
    indices.data[0] = 2; indices.data[0] = 6; indices.data[0] = 7;
    indices.data[0] = 0; indices.data[0] = 3; indices.data[0] = 7;

    engine->testObj = new RenderObject(vertexes, indices);

    buffer_free(vertexes);
    buffer_free(indices);

    return 0;
}

int engine_run(Engine_t* engine, float dt, Settings_t* settings) {
    scene_update(&engine->scene);
    renderer_render_object(&engine->renderer, *engine->testObj);

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
