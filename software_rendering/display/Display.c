#include "Display.h"
#include <X11/Xlib.h>

/*========== GAEDisplay ==========*/
int GAEDisplay_init(GAEDisplay_t* disp) {
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
    GAEDisplay_setFrameBufferSize(disp);

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

int GAEDisplay_currentMonitorDimensions(Display* display, int* width, int* height) {
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

int GAEDisplay_setFrameBufferSize(GAEDisplay_t* disp) {
    int width, height;
    GAEDisplay_currentMonitorDimensions(disp->display, &width, &height);
    disp->width = width;
    disp->height = height;

    // Free and allocate new memory to the framebuffer
    free(disp->framebuffer);
    free(disp->zbuffer);
    disp->framebuffer = (int *)malloc((disp->width * disp->height) * sizeof(int));
    disp->zbuffer = (double *)malloc((disp->width * disp->height) * sizeof(double));

    return disp->framebuffer == NULL || disp->zbuffer == NULL;
}

int GAEDisplay_setPixel(GAEDisplay_t* disp, int x, int y, int color) {
    if (x < 0 || x >= disp->width || y < 0 || y >= disp->height) {
        perror("Pixel index out of range\n");
        return 1;
    }

    int index = x + y * disp->width;
    if (disp->framebuffer != NULL) {
        disp->framebuffer[index] = color;
    }

    return 0;
}

int GAEDisplay_testAndSetZBuffer(GAEDisplay_t* disp, int x, int y, double z) {
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

int GAEDisplay_update(GAEDisplay_t* disp) {

    // Write the framebuffer to the display
    return XPutImage(disp->display, disp->window, disp->NormalGC, disp->ximage, 0, 0, 0, 0, disp->width, disp->height);
}

int GAEDisplay_clear(GAEDisplay_t* disp, int color) {
    for (int y = 0; y < disp->height; y++) {
        for (int x = 0; x < disp->width; x++) {
            int index = x + y * disp->width;
            disp->framebuffer[index] = color;
            disp->zbuffer[index] = 1.0;
        }
    }
    return 0;
}

int GAEDisplay_destroy(GAEDisplay_t* disp) {
    if (disp != NULL) {
        /* Destroy window */
        XDestroyWindow(disp->display, disp->window);

        /* Close connection to server */
        XCloseDisplay(disp->display);

        free(disp->framebuffer);

        /* Finnaly free the GAEDisplay_t struct */
        free(disp);
    }

    return 0;
}

/*========== GAEInput ==========*/
int GAEInput_init(GAEInput_t* input, GAEDisplay_t* disp) {
    input->display = disp;
    memset(input->held_keys, 0, KEYSTATE_SIZE);
    return 0;
}

int GAEInput_setKey(GAEInput_t* input, int keycode) {
    input->held_keys[keycode / 8] |= (1 << (keycode % 8));
    return 0;
}

int GAEInput_clearKey(GAEInput_t* input, int keycode) {
    input->held_keys[keycode / 8] &= ~(1 << (keycode % 8));
    return 0;
}

int GAEInput_update(GAEDisplay_t* disp, GAEInput_t* input) {
    // Check user keypress down
    XEvent event;
    int keyevent = XCheckWindowEvent(disp->display, disp->window, KeyPressMask | KeyReleaseMask, &event);
    if (keyevent) {
        if (event.xkey.type == KeyPress) {
            GAEInput_setKey(input, event.xkey.keycode);
        } else if (event.xkey.type == KeyRelease) {
            GAEInput_clearKey(input, event.xkey.keycode);
        }
    }

    int mouse_event = XCheckWindowEvent(disp->display, disp->window, PointerMotionMask, &event);
    if (mouse_event) {
        if (event.type == MotionNotify) {
            input->mouse_x = event.xmotion.x;
            input->mouse_y = event.xmotion.y;
        }
    }

    return 0;
}

int GAEInput_getKeyState(const GAEInput_t* input, char key) {
    unsigned long keysym = XStringToKeysym(&key);
    int keycode = XKeysymToKeycode(input->display->display, keysym);
    int byte_index = keycode / 8;
    int bit_index = keycode % 8;
    return input->held_keys[byte_index] & (1 << bit_index);
}

