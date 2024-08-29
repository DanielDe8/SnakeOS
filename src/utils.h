#ifndef __UTILS_H
#define __UTILS_H

char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);

void memcpy(unsigned int dest, unsigned int src, unsigned int size);
void memset(unsigned int addr, unsigned char fill, unsigned int size);

void srand(unsigned int new_seed);
unsigned int rand();
unsigned int rand_range(unsigned int min, unsigned int max);

static inline char* strcat(char* dest, const char* src) {
    // Pointer to the end of dest
    char* dest_end = dest;

    // Move the pointer to the end of dest
    while (*dest_end != '\0') {
        dest_end++;
    }

    // Copy each character from src to the end of dest
    while (*src != '\0') {
        *dest_end = *src;
        dest_end++;
        src++;
    }

    // Null-terminate the concatenated string
    *dest_end = '\0';

    // Return the destination string
    return dest;
}

// from tetris-os by jdh
static inline unsigned int strlen(const char *str) {
    unsigned int l = 0;
    while (*str++ != 0) {
        l++;
    }
    return l;
}

static inline char *itoa(int x, char *s, unsigned int sz) {
    // TODO: holy god this is bad code we need some error handling here
    // if (sz < 20) {
    //     extern void panic(const char *);
    //     panic("ITOA BUFFER TOO SMALL");
    // }

    unsigned int tmp;
    int i, j;

    tmp = x;
    i = 0;

    do {
        tmp = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
}

#endif