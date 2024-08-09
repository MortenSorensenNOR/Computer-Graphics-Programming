#include "camera.h"

int camera_init(Camera_t* camera, std::size_t camera_res_x, std::size_t camera_res_y) {
    camera->resolution = glm::ivec2(camera_res_x, camera_res_y);
    return 0;
}

int camera_set_config(Camera_t* camera, glm::vec3 pos, glm::vec3 world_up, float pitch, float yaw, float fov) {
    camera->pos = glm::vec3(0,0,3);
    camera->world_up = glm::vec3(0, 1, 0);
    camera->forward = glm::vec3(0, 0, -1);
    camera->fov = fov;
    camera->yaw = -90.0;
    camera->pitch = 0.0;

    camera->projection = glm::perspective(glm::radians(fov), (float)camera->resolution.x / (float)camera->resolution.y, 0.1f, 100.0f);
    camera_update_vectors(camera);
    camera_update_view(camera);

    return 0;
}

int camera_update_vectors(Camera_t* camera) {
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->forward = glm::normalize(front);
    camera->right = glm::normalize(glm::cross(camera->forward, camera->world_up));
    camera->up = glm::normalize(glm::cross(camera->right, camera->forward));

    return 0;
}

int camera_update_view(Camera_t* camera) {
    camera->view = glm::lookAt(camera->pos, camera->forward + camera->pos, camera->up);
    return 0;
}

glm::mat4* camera_get_view(Camera_t* camera) {
    return &camera->view;
}

glm::mat4* camera_get_projection(Camera_t* camera) {
    return &camera->projection;
}
