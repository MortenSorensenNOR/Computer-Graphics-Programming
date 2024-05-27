/* File: Main display driver 
 * Reference from https://stackoverflow.com/questions/64521652/create-xlib-window-with-a-frame-buffer-i-can-draw-directly-and-use-xputimage
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

#define KEYSTATE_SIZE 32    // 256 keys / 8 bits per char

/**
 * @brief Wrapper struct for exposing a framebuffer from X11/Xlib
 */
typedef struct {
    Display* display;
    Visual* visual;
    Window window;
    Window parent;

    XVisualInfo vinfo;
    XVisualInfo visual_template;
    XVisualInfo* visual_list;
    XSetWindowAttributes attrs;

    XImage* ximage;
    XEvent event;

    int depth;
    int* framebuffer;
    double* zbuffer;
    int width, height;
    int nxvisual;

    XGCValues gcv;
    unsigned long gcm;
    GC NormalGC;
} Display_t;

/**
 * @brief Sets up the Display struct for an X11 window
 * @param disp   The Display instance to initilize
 * @return Returns 0 for success and 1 for error
 */
int Display_init(Display_t* disp);

/**
 * @brief Gets the window dimensions of the current selected monitor
 * @param disp      The defualt X11 display
 * @param width     Pointer to the width int to be set
 * @param height    Pointer to the height int to be set
 * @return Returns 0 for success and 1 for error
 */
int Display_currentMonitorDimensions(Display* display, int* width, int* height);

/**
 * @brief Sets up the framebuffer for the Display struct, uses the current width and height of the display 
 * @param disp   The Display struct for which the framebuffer is being set up for
 * @return Returns 0 for success and 1 for error
 */
int Display_setFrameBufferSize(Display_t* disp);

/**
 * @brief Sets the pixel in the framebuffer at position (x, y) to color.
 * @param disp      The Display struct for which the pixel in the framebuffer is to be set
 * @param x         Horizontal index
 * @param y         Vertical index 
 * @param color     8-bit color - RED/GREEN/BLUE
 * @return Returns 0 for success and 1 for error
 */
int Display_setPixel(Display_t* disp, int x, int y, int color);

/**
 * @brief Checks if the z-buffer value at the pixel position is lower than the given value. 
 *      If not, it is set to that value and returns true.
 * @param disp      The Display struct for which the pixel in the framebuffer is to be set
 * @param x         Horizontal index
 * @param y         Vertical index 
 * @param z         Z-value being tested
 * @return Returns 1 if z < z_buffer[x][y], else 0
 */
int Display_testAndSetZBuffer(Display_t* disp, int x, int y, double z);

/**
 * @brief Main update function for the Display. Upon update, the new framebuffer gets displayed
 * @param disp   The Display struct being updated
 * @return Returns 0 for success and 1 for error
 */
int Display_update(Display_t* disp);

/**
 * @brief Clear the Display framebuffer.
 * @param disp   The Display struct for which the framebuffer is being cleared
 * @return Returns 0 for success and 1 for error
 */
int Display_clear(Display_t* disp, int color);

/**
 * @brief Desctructor for the Display struct. Calls free on the member varibales
 * @param disp   The Display struct being destroyed
 * @return Returns 0 for success and 1 for error
 */
int Display_destroy(Display_t* disp);

/**
 * @brief Wrapper struct for exposing user input from X11/Xlib
 */
typedef struct {
    Display_t* display;

    char held_keys[KEYSTATE_SIZE]; 
    
    int mouse_x;
    int mouse_y;
    char mouse_1_held;
    char mouse_2_held;
} Input_t;

/**
 * @brief 
 * @param input     Input wrapper struct
 * @param disp      The corresponding display wrapper struct
 * @return Returns 0 for success and 1 for error
 */
int Input_init(Input_t* input, Display_t* disp);

/**
 * @brief 
 * @param input     Input wrapper
 * @param keycode   The keycode to be set as active
 * @return Returns 0 for success and 1 for error
 */
int Input_setKey(Input_t* input, int keycode);

/**
 * @brief 
 * @param input     Input wrapper
 * @param keycode   The keycode to be set as inactive
 * @return Returns 0 for success and 1 for error
 */
int Input_clearKey(Input_t* input, int keycode);

/**
 * @brief Get user input for the display such as mouse and keyboard interaction
 * @param input     The input wrapper struct to be updated
 * @return Returns 0 for success and 1 for error
 */
int Input_update(Input_t* input);

/**
 * @brief 
 * @param input     The input wrapper struct to check keystate for
 * @param key       The key to be checked, in char format. i.e. "w" or "c"
 * @return Returns 1 if the key is held, 0 if not
 */
int Input_getKeyState(const Input_t* input, char key);

