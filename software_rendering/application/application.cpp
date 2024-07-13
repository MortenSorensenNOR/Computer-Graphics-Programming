#include "application.h"

int application_init(Application_t* app, size_t screen_width, size_t screen_height, std::string window_name, bool fullscreen) {
    // Initialize display
    int err = display_init(&app->display, screen_width, screen_height, window_name, fullscreen);
    if (err) {
        printf("Could not initialize display\n");
        return 1;
    }

    return 0;
}

int application_run(Application_t* app) {
    std::size_t buff_size = app->display.width * app->display.height;
    Buffer<glm::vec3> test_fb = buffer_allocate<glm::vec3>(buff_size);

    for (int i = 0; i < app->display.width * app->display.height; ++i) {
        test_fb.data[i] = glm::vec3(0.5);
    }

    int i = 0;
    while (i < 1000) {
        display_update(&app->display, test_fb);
        i++;
    }

    buffer_free(test_fb);

    return 0;
}

int application_free(Application_t* app) {
    display_free(&app->display);

    return 0;
}
