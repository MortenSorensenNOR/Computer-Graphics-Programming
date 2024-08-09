#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../core_utils/core_utils.h"

const float camera_mouse_sensitivity = 0.1f;

typedef struct {
    glm::ivec2 resolution;

    float pitch, yaw;

    glm::vec3 pos; 
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 world_up;
    float fov;

    glm::mat4 view;
    glm::mat4 projection;
} Camera_t;

int camera_init(Camera_t* camera, std::size_t camera_res_x, std::size_t camera_res_y);

int camera_set_config(Camera_t* camera, glm::vec3 pos, glm::vec3 world_up, float pitch, float yaw, float fov);

int camera_update_vectors(Camera_t* camera);

int camera_update_view(Camera_t* camera);

glm::mat4* camera_get_view(Camera_t* camera);

glm::mat4* camera_get_projection(Camera_t* camera);
