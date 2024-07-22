#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../core_utils/core_utils.h"

typedef struct {
    glm::ivec2 resolution;

    glm::vec3 pos; 
    glm::vec3 forward;
    glm::vec3 up;
    float fov;

    glm::mat4 view;
    glm::mat4 projection;
} Camera_t;

int camera_init(Camera_t* camera, std::size_t camera_res_x, std::size_t camera_res_y);

int camera_set_config(Camera_t* camera, glm::vec3 pos, glm::vec3 forward, glm::vec3 up, float fov);

int camera_update_view(Camera_t* camera);

glm::mat4* camera_get_view(Camera_t* camera);

glm::mat4* camera_get_projection(Camera_t* camera);
