/* File: Main display driver 
 * Reference from https://stackoverflow.com/questions/64521652/create-xlib-window-with-a-frame-buffer-i-can-draw-directly-and-use-xputimage
 */

#pragma once
#include <stdio.h>
#include <utility>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace GAE {

class GAEDisplay {
private:
    Display* display;
    Visual* visual;
    Window window;
    Window parent;

    XVisualInfo vinfo;
    XVisualInfo* visual_list;
    XVisualInfo visual_template;
    XSetWindowAttributes attrs;

    XImage* ximage;
    XEvent event;

    int depth;
    int* framebuffer;
    int width, height;
    int nxvisuals;

    XGCValues gcv;
    unsigned long gcm;
    GC NormalGC;

public:
    /**
     * @brief Creates an X11 window object with an exposed framebuffer, color on the form RGBA
     */
    GAEDisplay() {
        // Display setup
        display = XOpenDisplay(NULL);

        nxvisuals = 0;
        visual_template.screen = DefaultScreen(display);
        visual_list = XGetVisualInfo(display, VisualScreenMask, &visual_template, &nxvisuals);
        
        if (!XMatchVisualInfo(display, XDefaultScreen(display), 24, TrueColor, &vinfo)) {
            fprintf(stderr, "No such visual\n");
            exit(1);
        }
        
        parent = XDefaultRootWindow(display);
        XSync(display, true);
        
        visual = vinfo.visual;
        depth = vinfo.depth;
        
        attrs.colormap = XCreateColormap(display, XDefaultRootWindow(display), visual, AllocNone);
        attrs.backing_pixel = 0xFFFFFFFF;
        attrs.border_pixel = 0xFFFFFFFF;

        // Set width and height of the framebuffer
        setFrameBufferSize();
        
        // Create the window
        window = XCreateWindow(display, parent, 0, 0, width, height, 0, depth, InputOutput, visual, 
                CWBackPixel | CWColormap | CWBorderPixel, &attrs);
        
        ximage = XCreateImage(display, vinfo.visual, depth, ZPixmap, 0, (char *)framebuffer, width, height, 8, width*4);

        if (ximage == 0)
        {
            printf("ximage is null!\n");
        }

        XSync(display, true);

        XSelectInput(display, window, ExposureMask | KeyPressMask);

        gcm = GCGraphicsExposures;
        gcv.graphics_exposures = 0;
        NormalGC = XCreateGC(display, parent, gcm, &gcv);

        XMapWindow(display, window);
    }

    void setFrameBufferSize() {
        int snum = DefaultScreen(display);
        width = DisplayWidth(display, snum);
        height = DisplayHeight(display, snum);

        // Allocate the framebuffer
        framebuffer = (int *)malloc((width * height) * 4);
    }

    void update() {
        /* if (!XNextEvent(display, &event)) { */
        /*     switch (event.type) { */
        /*         case Expose: */
        /*             printf("I have been exposed\n"); */
        /*             XPutImage(display, window, NormalGC, ximage, 0, 0, 0, 0, width, height); */
        /*             break; */
        /*         case ConfigureNotify: */
        /*             setFrameBufferSize(); */
        /*             break; */
        /*     } */
        /* } */
        XPutImage(display, window, NormalGC, ximage, 0, 0, 0, 0, width, height);
    }

    int* getFrameBuffer() {
        return framebuffer;
    }

    std::pair<int, int> getWindowDimentions() {
        return std::make_pair(width, height);
    }

    ~GAEDisplay() {
        /* Destroy window */
        XDestroyWindow(display, window);

        /* Close connection to server */
        XCloseDisplay(display);
    }
};




































}

