#include "idt.h"
#include "utils.h"
#include "kernel.h"

struct IDT_entry IDT[IDT_SIZE];

static unsigned long long ticks;

void init_idt() {
    unsigned int kb_offset = (unsigned int) kb_handler;
    IDT[0x21].offset_low = kb_offset & 0x0000FFFF;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = IDT_INT_GATE_32BIT;
    IDT[0x21].offset_high = (kb_offset & 0xFFFF0000) >> 16;

    unsigned int pit_offset = (unsigned int) pit_handler;
    IDT[0x20].offset_low = pit_offset & 0x0000FFFF;
    IDT[0x20].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x20].zero = 0;
    IDT[0x20].type_attr = IDT_INT_GATE_32BIT;
    IDT[0x20].offset_high = (pit_offset & 0xFFFF0000) >> 16;

    outb(PIC1_COMMAND_PORT, 0x11);
    outb(PIC2_COMMAND_PORT, 0x11);

    outb(PIC1_DATA_PORT, 0x20);
    outb(PIC2_DATA_PORT, 0x28);

    outb(PIC1_DATA_PORT, 0x0);
    outb(PIC2_DATA_PORT, 0x0);

    outb(PIC1_DATA_PORT, 0x1);
    outb(PIC2_DATA_PORT, 0x1);

    outb(PIC1_DATA_PORT, 0xfc);
    outb(PIC2_DATA_PORT, 0xff);

    struct IDT_ptr idt_ptr;
    idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE - 1);
    idt_ptr.base = (unsigned int) &IDT;

    load_idt(&idt_ptr);
}

void init_kb() {
    // outb(PIC1_DATA_PORT, 0xFD);
}

void init_pit() {
    unsigned short divisor = PIT_FREQUENCY / TIMER_TPS;

    // Send the command byte to the PIT
    outb(0x43, 0x36);

    // Send the frequency divisor
    outb(0x40, divisor & 0xFF);      // Low byte of divisor
    outb(0x40, (divisor >> 8) & 0xFF); // High byte of divisor
}

void handle_kb_int() {
    outb(PIC1_COMMAND_PORT, 0x20);
    unsigned char status = inb(KB_STATUS_PORT);
    if (status & 0x1) {
        char keycode = inb(KB_DATA_PORT);
        if (keycode < 0) { return; }
        unsigned char* VGA = (unsigned char*) (0xb8000);
        // VGA[0] = keycode;
        key_press(keycode);
    }
}

void handle_pit_int() {
    outb(PIC1_COMMAND_PORT, 0x20);
    // tick();
    ticks++;
}

unsigned long long get_ticks() {
    return ticks;
}
