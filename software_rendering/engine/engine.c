#include "engine.h"

Engine_t* current_engine;

int engine_init(Engine_t* engine, int width, int height, char* window_name, float camera_fov) {
    current_engine = engine;

    // Initialize window
    int err = engine_window_init(engine, width, height, window_name);
    if (err != 0) {
        printf("Error: could not inizialize window. Error code %d\n", err);
        return 1;
    }

    // Initialize engine assets
    // TODO: Create better format
    engine->assets.textures = (texture_t*)malloc(engine->assets.n_textures * sizeof(texture_t));
    texture_t* test = &engine->assets.textures[0];
    load_texture("../resources/imgs/chad.jpg", &test->data, &test->width, &test->height);

    // Initialize renderer
    renderer_init(&engine->renderer, width, height, 0.1f, 100.0f);
    engine->camera_fov = camera_fov;

    return 0;
}

int engine_window_init(Engine_t* engine, int width, int height, char* window_name) {
    engine->screen_width = width;
    engine->screen_height = height;

    // Initlize display
    if (!glfwInit()) {
        fprintf(stderr, "Error: Could not initialize glfw\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, window_name, NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error: Could not initialize glfw window\n");
        glfwTerminate();
        return 2;
    }
    engine->window = window;
    glfwMakeContextCurrent(window);

    // Set user input callback
    glfwSetKeyCallback(engine->window, engine_get_key_input);

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    return 0;
}

static void engine_get_key_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int engine_run(Engine_t* engine) {
    while (!glfwWindowShouldClose(engine->window)) {
        // Render
        renderer_reset_buffers(&engine->renderer);
        renderer_render(&engine->renderer, engine->assets.n_textures, engine->assets.textures);

        // Show to display
        glRasterPos2i(0, 0);
        glDrawPixels(engine->screen_width, engine->screen_height, GL_RGB, GL_UNSIGNED_BYTE, engine->renderer.frame_buffer);
        glfwSwapBuffers(engine->window);
        glfwPollEvents();
    }
    return 0;
}

int engine_free(Engine_t* engine) {
    glfwTerminate();
    glfwDestroyWindow(engine->window);

    return 0;
}
