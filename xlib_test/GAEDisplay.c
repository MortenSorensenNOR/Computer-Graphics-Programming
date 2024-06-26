#include "GAEDisplay.h"

int GAEDisplay_init(struct GAEDisplay* disp) {
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
    XSelectInput(disp->display, disp->window, ExposureMask);

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

int GAEDisplay_setFrameBufferSize(struct GAEDisplay* disp) {
    // Get display dimentions
    /* int snum = DefaultScreen(disp->display); */
    /* disp->width = DisplayWidth(disp->display, snum); */
    /* disp->height = DisplayHeight(disp->display, snum); */

    int width, height;
    GAEDisplay_currentMonitorDimensions(disp->display, &width, &height);
    disp->width = width;
    disp->height = height;
    printf("1. %d %d\n", width, height);

    // Free and allocate new memory to the framebuffer
    free(disp->framebuffer);
    disp->framebuffer = (int *)malloc((disp->width * disp->height) * 4);

    return disp->framebuffer == NULL;
}

int GAEDisplay_setPixel(struct GAEDisplay* disp, int x, int y, int color) {
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

int GAEDisplay_update(struct GAEDisplay* disp) {
    // Uncomment for input
    /* if (!XNextEvent(disp->display, &disp->event)) { */
    /*     switch (disp->event.type) { */
    /*         case Expose: */
    /*             printf("I have been exposed\n"); */
    /*             XPutImage(disp->display, disp->window, disp->NormalGC, disp->ximage, 0, 0, 0, 0, disp->width, disp->height); */
    /*             break; */
    /*         case ConfigureNotify: */
    /*             GAEDisplay_setFrameBufferSize(disp); */
    /*             break; */
    /*     } */
    /* } */

    // Write the framebuffer to the display
    return XPutImage(disp->display, disp->window, disp->NormalGC, disp->ximage, 0, 0, 0, 0, disp->width, disp->height);
}

int GAEDisplay_clear(struct GAEDisplay* disp, int color) {
    for (int y = 0; y < disp->height; y++) {
        for (int x = 0; x < disp->width; x++) {
            int index = x + y * disp->width;
            disp->framebuffer[index] = color;
        }
    }
    return 0;
}

int GAEDisplay_destroy(struct GAEDisplay* disp) {
    if (disp != NULL) {
        /* Destroy window */
        XDestroyWindow(disp->display, disp->window);

        /* Close connection to server */
        XCloseDisplay(disp->display);

        free(disp->framebuffer);

        /* Finnaly free the GAEDisplay struct */
        free(disp);
    }
}
