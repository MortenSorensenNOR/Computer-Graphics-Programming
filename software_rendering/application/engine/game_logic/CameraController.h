#pragma once

#include "../../state/state.h"
#include "../../state/input.h"
#include "../scene/camera.h"

class CameraController {
private:
    Camera_t* controlled_camera;

public:
    CameraController(Camera_t* camera) {
        controlled_camera = camera;
    }

    void ControllsUpdate(InputState* input, State_t* app_state, float dt) {
        if (input->getKeyState('w')) {
            controlled_camera->pos += 8 * dt * glm::normalize(controlled_camera->forward);
        }
        if (input->getKeyState('s')) {
            controlled_camera->pos -= 8 * dt * glm::normalize(controlled_camera->forward);
        }
        if (input->getKeyState('a')) {
            controlled_camera->pos -= 8 * dt * glm::normalize(controlled_camera->right);
        }
        if (input->getKeyState('d')) {
            controlled_camera->pos += 8 * dt * glm::normalize(controlled_camera->right);
        }
        if (input->getModifierKeyState(WindowModifierKeys::SPACE)) {
            controlled_camera->pos += 4 * dt * glm::normalize(controlled_camera->world_up); 
        }
        if (input->getModifierKeyState(WindowModifierKeys::LSHIFT)) {
            controlled_camera->pos -= 4 * dt * glm::normalize(controlled_camera->world_up); 
        }

        if (app_state->mouse_captured) {
            float xoffset = input->mouse_delta_x * camera_mouse_sensitivity;
            float yoffset = -input->mouse_delta_y * camera_mouse_sensitivity;

            controlled_camera->yaw += xoffset;
            controlled_camera->pitch += yoffset;

            if (controlled_camera->pitch > 89.0f)
                controlled_camera->pitch = 89.0f;
            if (controlled_camera->pitch < -89.0f)
                controlled_camera->pitch = -89.0f;
        }
    }
};
