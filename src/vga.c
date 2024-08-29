#include "vga.h"
#include "utils.h"

static unsigned char *VGA = (unsigned char*) VGA_ADDRESS;

void plot_pixel(int x, int y, unsigned short color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

	unsigned short offset = x + SCREEN_WIDTH * y;
    VGA[offset] = color;
}

void rect(int x, int y, int w, int h, unsigned short color) {
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            plot_pixel(x + col, y + row, color);
        }
    }
}

void clear_screen(unsigned char color) {
    memset(VGA_ADDRESS, color, SCREEN_SIZE);
}
