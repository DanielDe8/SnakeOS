#ifndef __VGA_H
#define __VGA_H

#define VGA_ADDRESS 0xA0000

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

void plot_pixel(int x, int y, unsigned short color);
void rect(int x, int y, int w, int h, unsigned short color);
void clear_screen(unsigned char color);

#endif