#include "engine.h"

Engine::Engine(std::size_t screen_width, std::size_t screen_height, Settings_t* app_settings) : scene(screen_width, screen_height) {
    renderer_init(&renderer, screen_width, screen_height);

    // TODO: Make this part of instansiation of application and the rest of the systems
    std::size_t low_res_render_width = screen_width;
    std::size_t low_res_render_height = screen_height;

    // Create a new renderer and assign it as the render backend
    std::unique_ptr<RenderBackend> backend = std::make_unique<SimpleRenderer>(low_res_render_width, low_res_render_height, app_settings);
    renderer_set_backend(&renderer, std::move(backend));

    std::unique_ptr<Bilinear> upscaler = std::make_unique<Bilinear>(low_res_render_width, low_res_render_height, screen_width, screen_height);
    renderer.upscaler = std::move(upscaler);

    std::unique_ptr<FXAA> fxaa = std::make_unique<FXAA>(screen_width, screen_height);
    renderer.fxaa = std::move(fxaa);

    output_frame_buffer = buffer_allocate<char>(3 * screen_width * screen_height);

    // Create a new camera controller
    camera_controller = std::make_unique<CameraController>(scene.get_camera());
}

Engine::~Engine() {
    buffer_free(output_frame_buffer);
}

int Engine::load_scene_from_file(std::string assets_path, std::string file_name) {
    return scene.load_from_file(assets_path, file_name);
}

int Engine::run(float dt, State_t* app_state, Settings_t* settings, InputState* input) {
    // Update camera
    camera_controller->ControllsUpdate(input, app_state, dt);

    // Update the scene transforms
    scene.update();
    
    // Get camera matrices
    glm::mat4 view = scene.get_camera()->get_view();
    glm::mat4 projection = scene.get_camera()->get_projection();

    // Clear render buffers
    renderer.backend->ClearBuffers(glm::vec3(0.0314, 0.0941, 0.2078));

    static glm::vec3 angle = {0, 0, 0};
    // angle.x += glm::radians(dt * 30.0f);
    // angle.y += glm::radians(dt * 45.0f);
    // angle.z -= glm::radians(dt * 25.0f);

    // Add scene objects to the render queue
    std::shared_ptr<SceneObject> scene_root = scene.get_scene_root();
    for (std::size_t i = 0; i < scene_root->children.size(); i++) {
        std::size_t id = scene_root->children.at(i);
        std::shared_ptr<SceneObject> so = scene.get_scene_object(id);

        for (std::size_t mesh_index = 0; mesh_index < so->mesh_indices.size(); mesh_index++) {
            std::size_t mesh_idx = so->mesh_indices.at(mesh_index);
            std::shared_ptr<Mesh> mesh = scene.get_mesh(mesh_idx);

            RenderObject ro(mesh.get());
            ro.model = glm::rotate(glm::rotate(
                            glm::rotate(so->model, (float)std::pow(-1, i) * angle.x, glm::vec3(1,0,0)), 
                            (float)std::pow(-1, i+1) * angle.y, glm::vec3(0,1,0)), angle.z, glm::vec3(0,0,1));
    
            renderer_add_to_render_queue(&renderer, ro);
        }
    }

    // Render
    renderer_render(&renderer, view, projection);


    // Upscale
    Buffer<glm::vec3>* upscaled_fb = renderer.upscaler->upscale(*renderer_get_fb(&renderer));

    if (settings->enableFXAA) {
        // Anti-aliasing
        static Timer timer;
        timer.start();
        Buffer<glm::vec3>* fxaa_fb = renderer.fxaa->fxaa(*upscaled_fb, settings->fxaa_threshold);
        timer.stop();
        printf("FXAA took: %f us\n", timer.get());

        // TODO: Make this better
        for (std::size_t i = 0; i < upscaled_fb->size(); i++) {
            output_frame_buffer.data[3 * i + 0] = static_cast<char>(255 * fxaa_fb->data[i].x);
            output_frame_buffer.data[3 * i + 1] = static_cast<char>(255 * fxaa_fb->data[i].y);
            output_frame_buffer.data[3 * i + 2] = static_cast<char>(255 * fxaa_fb->data[i].z);
        }
    } else {
        for (std::size_t i = 0; i < upscaled_fb->size(); i++) {
            output_frame_buffer.data[3 * i + 0] = static_cast<char>(255 * upscaled_fb->data[i].x);
            output_frame_buffer.data[3 * i + 1] = static_cast<char>(255 * upscaled_fb->data[i].y);
            output_frame_buffer.data[3 * i + 2] = static_cast<char>(255 * upscaled_fb->data[i].z);
        }
    }


    return 0;
}

Buffer<char>* Engine::get_fb() {
    return &output_frame_buffer;
}
