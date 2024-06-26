#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct {
    float fov;
    float aspect;
    int width, height;

    glm::vec3 pos;

    glm::vec3 forward;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 screen_space_transform;
} Camera_t;

int camera_init(Camera_t* camera, float fov, float aspect, int width, int height, glm::vec3 pos, glm::vec3 forward, glm::vec3 up);

int camera_update(Camera_t* camera, glm::vec3 new_pos, glm::vec3 new_forward, glm::vec3 new_up);

glm::mat4 camera_get_view(Camera_t* camera);
glm::mat4 camera_get_projection(Camera_t* camera);
glm::mat4 camera_get_screen_space_transform(Camera_t* camera);

glm::ivec2 camera_get_resolution(Camera_t* camera);
