#include "camera.h"

int camera_init(Camera_t* camera, std::size_t camera_res_x, std::size_t camera_res_y) {
    camera->resolution = glm::ivec2(camera_res_x, camera_res_y);
    return 0;
}

int camera_set_config(Camera_t* camera, glm::vec3 pos, glm::vec3 forward, glm::vec3 up, float fov) {
    camera->pos = pos;
    camera->forward = forward;
    camera->up = up;
    camera->fov = fov;

    camera->projection = glm::perspective(glm::radians(fov), (float)camera->resolution.x / (float)camera->resolution.y, 0.1f, 100.0f);
    camera->view = glm::lookAt(pos, forward, up);

    return 0;
}

int camera_update_view(Camera_t* camera) {
    camera->view = glm::lookAt(camera->pos, camera->forward, camera->up);
    return 0;
}

glm::mat4* camera_get_view(Camera_t* camera) {
    return &camera->view;
}

glm::mat4* camera_get_projection(Camera_t* camera) {
    return &camera->projection;
}
