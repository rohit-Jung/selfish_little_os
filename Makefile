CC  := gcc
AS  := nasm
LD  := ld

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
ISO_DIR   := iso
KERNEL    := $(BUILD_DIR)/kernel.elf
ISO       := selfish_os.iso

C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
S_SRCS := $(shell find $(SRC_DIR) -name '*.s')

MODULE_SRCS := $(wildcard modules/*.s)
MODULE_BINS := $(MODULE_SRCS:modules/%.s=$(BUILD_DIR)/modules/%)

# The source extension is kept in the object name: gdt.c -> gdt.c.o and
# gdt.s -> gdt.s.o. Without it both would claim build/arch/x86/gdt.o, the
# .c rule would win, and the assembly would silently never be assembled.
ALL_OBJS := $(C_SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o) \
            $(S_SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)

# loader.s.o MUST link first: the multiboot header has to land within the
# first 8 KB of the ELF image, and `find` gives no ordering guarantee.
LOADER_OBJ := $(BUILD_DIR)/boot/loader.s.o
OBJS       := $(LOADER_OBJ) $(filter-out $(LOADER_OBJ),$(ALL_OBJS))
DEPS       := $(ALL_OBJS:.o=.d)

CFLAGS := -m32 -std=c11 -ffreestanding \
          -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
          -fno-pic -mgeneral-regs-only \
          -Wall -Wextra -Werror \
          -I$(INC_DIR) -MMD -MP

ASFLAGS := -f elf32
LDFLAGS := -T link.ld -melf_i386

.PHONY: all iso run debug clean

all: $(KERNEL)

$(KERNEL): $(OBJS) link.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/modules/%: modules/%.s
		@mkdir -p $(dir $@)
		$(AS) -f bin $< -o $@

iso: $(ISO)

$(ISO): $(KERNEL) $(MODULE_BINS) $(ISO_DIR)/boot/grub/grub.cfg
		@mkdir -p $(ISO_DIR)/modules
		cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
		cp $(MODULE_BINS) $(ISO_DIR)/modules/
		grub-mkrescue -o $@ $(ISO_DIR)

run: $(ISO)
	qemu-system-i386 -enable-kvm -boot d -cdrom $(ISO) -m 4 -serial stdio

# headless boot with full exception tracing
debug: $(ISO)
	qemu-system-i386 -boot d -cdrom $(ISO) -m 4 -display none \
        -serial stdio -no-reboot -d int,cpu_reset -D $(BUILD_DIR)/qemu.log

clean:
	rm -rf $(BUILD_DIR) $(ISO) $(ISO_DIR)/boot/kernel.elf

-include $(DEPS)
