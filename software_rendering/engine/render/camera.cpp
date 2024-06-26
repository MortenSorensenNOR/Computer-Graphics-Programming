#include "camera.h"

int camera_init(Camera_t* camera, float fov, float aspect, int width, int height, glm::vec3 pos, glm::vec3 forward, glm::vec3 up) {
    camera->fov = fov;
    camera->aspect = aspect;

    camera->width = width;
    camera->height = height;

    camera->pos = pos;
    camera->forward = forward;
    camera->up = up;

    glm::mat4 tmp = glm::mat4(1.0f);
    tmp[0][3] = 1.0f;
    tmp[1][3] = -1.0f;

    camera->screen_space_transform = glm::mat4(1.0f);
    camera->screen_space_transform *= glm::vec4(width, height, 1.0f, 1.0f);
    camera->screen_space_transform *= glm::vec4(0.5f, -0.5f, 1.0f, 1.0f);
    camera->screen_space_transform = glm::transpose(tmp * camera->screen_space_transform);

    camera->projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 1000.0f);

    return 0;
}

int camera_update(Camera_t* camera, glm::vec3 new_pos, glm::vec3 new_forward, glm::vec3 new_up) {
    camera->pos = new_pos;
    camera->forward = new_forward;
    camera->up = new_up;

    return 0;
}

glm::mat4 camera_get_view(Camera_t* camera) {
    return glm::lookAt(camera->pos, camera->pos + camera->forward, camera->up);
}

glm::mat4 camera_get_projection(Camera_t* camera) {
    return camera->projection;
}

glm::mat4 camera_get_screen_space_transform(Camera_t* camera) {
    return camera->screen_space_transform;
}

glm::ivec2 camera_get_resolution(Camera_t* camera) {
    return glm::ivec2(camera->width, camera->height);
}
