#include "utils.h"

extern char _inb_asm(unsigned short port);
char inb(unsigned short port) {
    return _inb_asm(port);
}

extern void _outb_asm(unsigned short port, unsigned char value);
void outb(unsigned short port, unsigned char value) {
    _outb_asm(port, value);
}

extern void _memcpy_asm(unsigned int dest, unsigned int src, unsigned int size); // see stdlib.asm
void memcpy(unsigned int dest, unsigned int src, unsigned int size) {
	_memcpy_asm(dest, src, size);
}

extern void _memset_asm(unsigned int addr, unsigned char fill, unsigned int size); // see stdlib.asm
void memset(unsigned int addr, unsigned char fill, unsigned int size) {
	_memset_asm(addr, fill, size);
}

// RNG state variable
static unsigned int seed = 42; // Default seed value

// Parameters for the Linear Congruential Generator (LCG)
const unsigned int a = 1664525;        // Multiplier
const unsigned int c = 1013904223;     // Increment
const unsigned int m = 0xFFFFFFFF;     // Modulus (2^32)

// Initializes the random number generator with a new seed
void srand(unsigned int new_seed) {
    seed = new_seed;
}

// Generates a pseudo-random number
unsigned int rand() {
    seed = (a * seed + c) % m;
    return seed;
}

// Generates a pseudo-random number within a specified range [min, max]
unsigned int rand_range(unsigned int min, unsigned int max) {
    if (max <= min) return min; // Handle edge case to prevent division by zero
    return (rand() % (max - min + 1)) + min;
}