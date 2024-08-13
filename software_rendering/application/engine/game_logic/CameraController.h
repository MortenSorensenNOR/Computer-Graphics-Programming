#pragma once

#include "../../state/state.h"
#include "../../state/input.h"
#include "../scene/camera.h"

class CameraController {
private:
    const float mouse_sensitivity = 0.1f;
    std::shared_ptr<Camera> controlled_camera;

public:
    CameraController(std::shared_ptr<Camera> camera) {
        controlled_camera = camera;
    }

    void ControllsUpdate(InputState* input, State_t* app_state, float dt) {
        // TODO: FIX CANNOT CHANGE PRIVATE PROPERTIES OF CAMERA
        if (input->getKeyState('w')) {
            controlled_camera->pos += 8 * dt * glm::normalize(controlled_camera->get_forward());
        }
        if (input->getKeyState('s')) {
            controlled_camera->pos -= 8 * dt * glm::normalize(controlled_camera->get_forward());
        }
        if (input->getKeyState('a')) {
            controlled_camera->pos -= 8 * dt * glm::normalize(controlled_camera->get_right());
        }
        if (input->getKeyState('d')) {
            controlled_camera->pos += 8 * dt * glm::normalize(controlled_camera->get_right());
        }
        if (input->getModifierKeyState(WindowModifierKeys::SPACE)) {
            controlled_camera->pos += 4 * dt * glm::normalize(controlled_camera->get_world_up()); 
        }
        if (input->getModifierKeyState(WindowModifierKeys::LSHIFT)) {
            controlled_camera->pos -= 4 * dt * glm::normalize(controlled_camera->get_world_up()); 
        }
        
        if (app_state->mouse_captured) {
            float xoffset = input->mouse_delta_x * mouse_sensitivity;
            float yoffset = -input->mouse_delta_y * mouse_sensitivity;
        
            controlled_camera->yaw += xoffset;
            controlled_camera->pitch += yoffset;
        
            if (controlled_camera->pitch > 89.0f)
                controlled_camera->pitch = 89.0f;
            if (controlled_camera->pitch < -89.0f)
                controlled_camera->pitch = -89.0f;
        }
    }
};
