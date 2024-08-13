#include "camera.h"

Camera::Camera(std::size_t res_x, std::size_t res_y) {
    this->resolution = glm::ivec2(res_x, res_y);
}

int Camera::set_config(glm::vec3 pos, glm::vec3 world_up, float pitch, float yaw, float fov) {
    this->pos = pos;
    this->world_up = world_up;
    this->fov = fov;
    this->yaw = yaw;
    this->pitch = pitch;

    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->forward = glm::normalize(forward);

    this->projection = glm::perspective(glm::radians(fov), (float)resolution.x / (float)resolution.y, 0.1f, 100.0f);
    Camera::update_vectors();
    Camera::update_view();

    return 0;
}

int Camera::update_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->forward = glm::normalize(front);
    this->right = glm::normalize(glm::cross(forward, world_up));
    this->up = glm::normalize(glm::cross(right, forward));

    return 0;
}

int Camera::update_view() {
    this->view = glm::lookAt(this->pos, this->forward + this->pos, this->up);
    return 0;
}

glm::mat4 Camera::get_view() const {
    return this->view;
}

glm::mat4 Camera::get_projection() const {
    return this->projection;
}

glm::vec3 Camera::get_forward() const {
    return this->forward;
}

glm::vec3 Camera::get_right() const {
    return this->right;
}

glm::vec3 Camera::get_world_up() const {
    return this->world_up;
}
