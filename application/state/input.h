#pragma once

#include "../window/window.h"

struct InputState {
    // Mouse state
    int mouse_x;
    int mouse_y;
    int mouse_delta_x;
    int mouse_delta_y;
    bool mouse_right;
    bool mouse_left;

    Window_t* active_window;

    int updateKeyState() {
        return window_update_key_state(active_window);
    }

    int updateMouseState() {
        int err = window_update_mouse_state(active_window);
        if (err)
            return err;

        mouse_x = active_window->mouse_pos_x;
        mouse_y = active_window->mouse_pos_y;
        mouse_delta_x = active_window->mouse_delta_x;
        mouse_delta_y = active_window->mouse_delta_y;
        mouse_right = active_window->mouse_right_btn_pressed;
        mouse_left = active_window->mouse_left_btn_pressed;
        return 0;
    }

    int getKeyState(char key) {
        return window_get_key_state(active_window, key);
    }

    int getModifierKeyState(WindowModifierKeys modifier) {
        return window_get_modifier_key_state(active_window, modifier);
    }
};
