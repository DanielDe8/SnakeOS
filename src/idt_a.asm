global load_idt
global kb_handler
global pit_handler
global enable_ints

extern handle_kb_int
extern handle_pit_int

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    ret

enable_ints:
    sti
    ret

kb_handler:
    pushad
    cld
    call handle_kb_int
    popad
    iret

pit_handler:
    pushad
    cld
    call handle_pit_int
    popad
    iret