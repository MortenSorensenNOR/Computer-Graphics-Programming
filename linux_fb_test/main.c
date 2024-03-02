#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

void draw_rectangle(char *fb_ptr, struct fb_var_screeninfo* vinfo, struct fb_fix_screeninfo* finfo, int x, int y, int width, int height, int* color) {
    for (int py = y; py < y + height; py++) {
        for (int px = x; px < x + width; px++) {
            if (px >= 0 && px < vinfo->xres && py >= 0 && py < vinfo->yres) {
                long int location = (px + vinfo->xoffset) * (vinfo->bits_per_pixel / 8) + 
                    (py + vinfo->yoffset) * finfo->line_length;
                *(fb_ptr + location + 0) = color[0];
                *(fb_ptr + location + 1) = color[1];
                *(fb_ptr + location + 2) = color[2];
                *(fb_ptr + location + 3) = color[3];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open frambuffer device");
        exit(1);
    }

    struct fb_fix_screeninfo finfo;
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        perror("Error: cannot read variable information");
        close(fbfd);
        exit(2);
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Error: cannot read variable information");
        close(fbfd);
        exit(3);
    }

    int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    char* fb_ptr = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fb_ptr == MAP_FAILED) {
        perror("Error: failed to map framebuffer device to memory");
        close(fbfd);
        exit(4);
    }

    int blue[4] = {0xFF, 0x00, 0x00, 0xFF};
    int red[4] = {0x00, 0x00, 0xFF, 0xFF};
    int green[4] = {0x00, 0xFF, 0x00, 0xFF};
    int white[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    draw_rectangle(fb_ptr, &vinfo, &finfo, 50, 50, 10, 10, blue);
    draw_rectangle(fb_ptr, &vinfo, &finfo, 150, 50, 10, 10, red);
    draw_rectangle(fb_ptr, &vinfo, &finfo, 10, 200, 25, 5, green);
    draw_rectangle(fb_ptr, &vinfo, &finfo, 300, 300, 10, 10, white);

    // Cleanup
    munmap(fb_ptr, screensize);
    close(fbfd);

    return 0;
}
