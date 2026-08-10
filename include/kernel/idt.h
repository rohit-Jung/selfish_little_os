#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <kernel/types.h>

#define IDT_ENTRY_COUNT 256

/* Vectors this kernel currently installs a gate for. The keyboard sits on
 * IRQ 1, which the remapped master PIC delivers as 32 + 1. */
#define INTERRUPTS_PAGING   14
#define INTERRUPTS_KEYBOARD 33

/* type_and_attr byte
 *   bit  7     P    present
 *   bits 6..5  DPL  privilege level required to invoke the gate
 *   bit  4     S    0 for gate descriptors
 *   bits 3..0  type 0xE = 32-bit interrupt gate (clears IF on entry)
 */
#define IDT_GATE_PRESENT 0x80
#define IDT_GATE_DPL0    0x00
#define IDT_GATE_INT32   0x0E

/* Loaded into the IDTR by `lidt`. `size` is a limit: bytes minus one. */
struct idt_ptr {
  u16 size;
  u32 address;
} __attribute__((packed));

struct idt_gate {
  u16 offset_low;  /* handler address, bits 0..15  */
  u16 selector;    /* code segment selector in the GDT */
  u8  reserved;    /* always zero */
  u8  type_and_attr;
  u16 offset_high; /* handler address, bits 16..31 */
} __attribute__((packed));

void idt_set_gate(int index, u32 address);
void idt_install(void);

/* Defined in src/arch/x86/idt.s */
void idt_load(u32 idt_address);

#endif /* KERNEL_IDT_H */
