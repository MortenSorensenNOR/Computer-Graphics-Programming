#pragma once

struct InputState {
    // Mouse state
    int mouse_pos_x;
    int mouse_pos_y;
    int mouse_pos_delta_x;
    int mouse_pos_delta_y;
    bool mouse_right_click;
    bool mouse_left_click;
     
    // Key state
    int active_keys[128];
    int active_modifiers[2]; // Shift and ctrl. Todo: add more if needed

    int getKeyState(char* c);
};
