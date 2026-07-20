OBJECTS = loader.o kmain.o framebuffer.o io.o serial.o \
					interrupt.o keyboard.o pic.o idt.o interrupt_handler.o
CC = gcc 
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
				 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o selfish_os.iso iso

run: os.iso
	qemu-system-i386 -enable-kvm -boot d -cdrom selfish_os.iso -m 4 -serial stdio

run_debug: os.iso
	qemu-system-i386 -enable-kvm -boot d -cdrom selfish_os.iso -m 4 -monitor stdio

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso
