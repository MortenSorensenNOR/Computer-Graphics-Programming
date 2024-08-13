#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../core_utils/core_utils.h"


class Camera {
public:
    glm::vec3 pos; 
    float pitch, yaw;
    
private:
    glm::ivec2 resolution;

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 world_up;
    float fov;

    glm::mat4 view;
    glm::mat4 projection;

public:
    Camera(std::size_t res_x, std::size_t res_y);

    /**
     * @brief Set a new configuration for the camera
     * @param pos       The position of the camera
     * @param world_up  The global world up vector
     * @param pitch     The pitch ange of the camera relative to world_up
     * @param yaw       The yaw angle of the camera relative to world_up
     * @param fov       The fov of the camera
     */
    int set_config(glm::vec3 pos, glm::vec3 world_up, float pitch, float yaw, float fov);

    int update_vectors();

    int update_view();

    glm::mat4 get_view() const;

    glm::mat4 get_projection() const;

    glm::vec3 get_forward() const;

    glm::vec3 get_right() const;

    glm::vec3 get_world_up() const;
};



