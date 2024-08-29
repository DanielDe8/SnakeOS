// from tetris-os by jdh

#ifndef __FONT_H
#define __FONT_H

#define font_width(_s) (strlen((_s)) * 8)
#define font_height() (8)
#define font_str_doubled(_s, _x, _y, _c) do {\
        const char *__s = (_s);\
        __typeof__(_x) __x = (_x);\
        __typeof__(_y) __y = (_y);\
        __typeof__(_c) __c = (_c);\
        font_str(__s, __x + 1, __y + 1, COLOR_ADD(__c, -2));\
        font_str(__s, __x, __y, __c);\
    } while (0);


void font_char(char c, unsigned int x, unsigned int y, unsigned char color);
void font_str(const char *s, unsigned int x, unsigned int y, unsigned char color);
void font_char_big(char c, unsigned int x, unsigned int y, unsigned char color);
void font_str_big(const char *s, unsigned int x, unsigned int y, unsigned char color);

#endif