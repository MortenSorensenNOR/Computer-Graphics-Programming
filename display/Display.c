#include "Display.h"
#include <X11/Xlib.h>
#include <omp.h>

/*========== Display ==========*/
int Display_init(Display_t* disp) {
    disp->display = XOpenDisplay(NULL);    

    disp->nxvisual = 0;
    disp->visual_template.screen = DefaultScreen(disp->display);
    disp->visual_list = XGetVisualInfo(disp->display, VisualScreenMask, &disp->visual_template, &disp->nxvisual);

    if (!XMatchVisualInfo(disp->display, XDefaultScreen(disp->display), 24, TrueColor, &disp->vinfo)) {
        fprintf(stderr, "No such visual\n");
        return 1;
    }

    disp->parent = XDefaultRootWindow(disp->display);
    XSync(disp->display, True);

    disp->visual = disp->vinfo.visual;
    disp->depth = disp->vinfo.depth;

    // Set display attributes
    disp->attrs.colormap = XCreateColormap(disp->display, XDefaultRootWindow(disp->display), disp->visual, AllocNone);
    disp->attrs.backing_pixel = 0xFFFFFF;    
    disp->attrs.border_pixel = 0xFFFFFF;

    // Set width and height of the framebuffer
    Display_setFrameBufferSize(disp);

    // Create the window
    disp->window = XCreateWindow(disp->display, disp->parent, 0, 0, disp->width, disp->height, 0, disp->depth, 
            InputOutput, disp->visual, CWBackPixel | CWColormap | CWBorderPixel, &disp->attrs);

    disp->ximage = XCreateImage(disp->display, disp->vinfo.visual, disp->depth, ZPixmap, 0, 
            (char *)disp->framebuffer, disp->width, disp->height, 8, disp->width * 4);

    if (disp->ximage == 0) {
        printf("ximage is null!\n");
    }

    XSync(disp->display, True);
    XSelectInput(disp->display, disp->window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);

    disp->gcm = GCGraphicsExposures;
    disp->gcv.graphics_exposures = 0;
    disp->NormalGC = XCreateGC(disp->display, disp->parent, disp->gcm, &disp->gcv);

    XMapWindow(disp->display, disp->window);

    return 0;
}

int Display_currentMonitorDimensions(Display* display, int* width, int* height) {
    XRRScreenResources *screenRes = XRRGetScreenResources(display, DefaultRootWindow(display));
    XRROutputInfo *outputInfo;
    XRRCrtcInfo *crtcInfo;
    int i;

    for (i = 0; i < screenRes->noutput; i++) {
        outputInfo = XRRGetOutputInfo(display, screenRes, screenRes->outputs[i]);
        if (outputInfo->connection == RR_Connected) {
            crtcInfo = XRRGetCrtcInfo(display, screenRes, outputInfo->crtc);
            *width = crtcInfo->width;
            *height = crtcInfo->height;
            XRRFreeCrtcInfo(crtcInfo);
            XRRFreeOutputInfo(outputInfo);
            break;
        }
        XRRFreeOutputInfo(outputInfo);
    }
    XRRFreeScreenResources(screenRes);

    return 0;
}

int Display_setFrameBufferSize(Display_t* disp) {
    int width, height;
    Display_currentMonitorDimensions(disp->display, &width, &height);

    // Set resolution to 720p
    // Uncomment for full display resolution
    width = 640;
    height = 480;

    disp->width = width;
    disp->height = height;

    // Free and allocate new memory to the framebuffer
    disp->framebuffer = (int *)malloc((disp->width * disp->height) * sizeof(int));
    disp->zbuffer = (double *)malloc((disp->width * disp->height) * sizeof(double));

    return disp->framebuffer == NULL || disp->zbuffer == NULL;
}

int Display_setPixel(Display_t* disp, int x, int y, int color) {
    if (x < 0 || x >= disp->width || y < 0 || y >= disp->height || disp->framebuffer == NULL) {
        perror("Pixel index out of range for framebuffer\n");
        return 1;
    }

    disp->framebuffer[x + y * disp->width] = color;
    return 0;
}

int Display_testAndSetZBuffer(Display_t* disp, int x, int y, double z) {
    if (x < 0 || x >= disp->width || y < 0 || y >= disp->height) {
        perror("Pixel index out of range\n");
        return 1;
    }

    int index = x + y * disp->width;
    if (disp->zbuffer != NULL && z < disp->zbuffer[index]) {
        disp->zbuffer[index] = z;
        return 1;
    }

    return 0;
}

int Display_update(Display_t* disp) {
    // Write the framebuffer to the display
    return XPutImage(disp->display, disp->window, disp->NormalGC, disp->ximage, 0, 0, 0, 0, disp->width, disp->height);
}

int Display_clear(Display_t* disp, int color) {
    if (!disp->framebuffer || !disp->zbuffer)
        return 1;

    memset(disp->framebuffer, color, disp->width * disp->height * sizeof(int));
    memset(disp->zbuffer, 1.0, disp->width * disp->height * sizeof(float));

    return 0;
}

int Display_destroy(Display_t* disp) {
    if (disp != NULL) {
        /* Destroy window */
        XDestroyWindow(disp->display, disp->window);

        /* Close connection to server */
        XCloseDisplay(disp->display);

        if (disp->framebuffer)
            free(disp->framebuffer);
        if (disp->zbuffer)
            free(disp->zbuffer);
    }

    return 0;
}

/*========== Input ==========*/
int Input_init(Input_t* input, Display_t* disp) {
    input->display = disp;
    memset(input->held_keys, 0, KEYSTATE_SIZE);
    return 0;
}

int Input_setKey(Input_t* input, int keycode) {
    input->held_keys[keycode / 8] |= (1 << (keycode % 8));
    return 0;
}

int Input_clearKey(Input_t* input, int keycode) {
    input->held_keys[keycode / 8] &= ~(1 << (keycode % 8));
    return 0;
}

int Input_update(Input_t* input) {
    // Check user keypress down
    XEvent event;
    int keyevent = XCheckWindowEvent(input->display->display, input->display->window, KeyPressMask | KeyReleaseMask, &event);
    if (keyevent) {
        if (event.xkey.type == KeyPress) {
            Input_setKey(input, event.xkey.keycode);
        } else if (event.xkey.type == KeyRelease) {
            Input_clearKey(input, event.xkey.keycode);
        }
    }

    int mouse_event = XCheckWindowEvent(input->display->display, input->display->window, PointerMotionMask, &event);
    if (mouse_event) {
        if (event.type == MotionNotify) {
            input->mouse_x = event.xmotion.x;
            input->mouse_y = event.xmotion.y;
        }
    }

    return 0;
}

int Input_getKeyState(const Input_t* input, char key) {
    unsigned long keysym = XStringToKeysym(&key);
    int keycode = XKeysymToKeycode(input->display->display, keysym);
    int byte_index = keycode / 8;
    int bit_index = keycode % 8;
    return input->held_keys[byte_index] & (1 << bit_index);
}

