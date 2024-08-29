global _inb_asm
global _outb_asm
global _memcpy_asm
global _memset_asm

_inb_asm:
    mov edx, [esp + 4]
    in al, dx
    ret

_outb_asm:
    mov edx, [esp + 4]
    mov eax, [esp + 8]
    out dx, al
    ret

_memcpy_asm:
    mov edi, [esp+0x4] ; arg1: esp+0x4 dest
    mov esi, [esp+0x8] ; arg2: esp+0x8 src
    mov ecx, [esp+0xc] ; arg3: esp+0xc size
    rep movsb
    ret

_memset_asm:
    mov edi, [esp+0x4] ; arg1: esp+0x4 addr
    mov eax, [esp+0x8] ; arg2: esp+0x8 fill
    mov ecx, [esp+0xc] ; arg3: esp+0xc size
    rep stosb
    ret