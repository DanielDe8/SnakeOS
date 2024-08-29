#ifndef __IDT_H
#define __IDT_H

#define IDT_SIZE 256
#define KERNEL_CODE_SEGMENT_OFFSET 0x8
#define IDT_INT_GATE_32BIT         0x8e
#define PIC1_COMMAND_PORT          0x20
#define PIC1_DATA_PORT             0x21
#define PIC2_COMMAND_PORT          0xA0
#define PIC2_DATA_PORT             0xA1
#define KB_DATA_PORT               0x60
#define KB_STATUS_PORT             0x64
#define PIT_FREQUENCY              1193131.666 // idk what this is, from tetris-os by jdh
#define DESIRED_FREQUENCY          5

extern void kb_handler();
extern void pit_handler();
extern void load_idt(unsigned int* idt_adress);
extern void enable_ints();

struct IDT_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct IDT_entry {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
} __attribute__((packed));

extern struct IDT_entry IDT[IDT_SIZE];

void init_idt();
void init_kb();
void init_pit();

#endif