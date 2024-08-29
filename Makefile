.PHONY: all clean run run_img run_vbox

OUT_DIR = ./out

C_SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
ASM_SOURCES = $(wildcard src/*.asm)

OBJS = $(patsubst src/%.c, $(OUT_DIR)/%.o, $(C_SOURCES)) \
       $(patsubst src/%.asm, $(OUT_DIR)/%.o, $(ASM_SOURCES))

IMG = snakeos.img
VM = SnakeOS

CC = i386-elf-gcc
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy
QEMU = qemu-system-i386
CFLAGS = -g \
		-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		-Wall -Wextra

all: clean run_img

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/%.o: */%.asm | $(OUT_DIR)
	nasm $< -f elf -o $@

$(OUT_DIR)/%.bin: */%.asm | $(OUT_DIR)
	nasm $< -f bin -o $@

$(OUT_DIR)/%.o: src/%.c ${HEADERS} | $(OUT_DIR)
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

$(OUT_DIR)/kernel.elf: $(OUT_DIR)/load_kernel.o ${OBJS}
	${LD} -o $@ -Ttext 0x1000 $^

$(OUT_DIR)/kernel.bin: $(OUT_DIR)/kernel.elf
	${OBJCOPY} -O binary $< $@

$(OUT_DIR)/image.bin: $(OUT_DIR)/bootloader.bin $(OUT_DIR)/kernel.bin
	cat $^ > $@

$(IMG): $(OUT_DIR)/image.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$@ conv=notrunc

run: $(OUT_DIR)/image.bin
	${QEMU} -fda $< -no-reboot -d int,cpu_reset

run_img: $(IMG)
	${QEMU} -drive file=$<,format=raw,if=floppy -boot a -m 16M #-d int,cpu_reset

run_vbox: $(IMG)
	VBoxManage startvm $(VM)

clean:
	rm -rf $(OUT_DIR) *.img