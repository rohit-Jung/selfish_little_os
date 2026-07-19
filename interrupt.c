#include "interrupt.h"
#include "keyboard.h"
#include "pic.h"
#include "serial.h"

struct IDT idt;
struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];

/*
 * interrupts_init_descriptor
 * Populate one entry in the idt (interrupt descriptor table).
 * each entry is an 8-byte gate descriptor pointing to the handler at `address`.
 *
 * @param index   Index into the IDT descriptor array
 * @param address 32-bit linear address of the interrupt handler
 */
void interrupts_init_descriptor(int index, unsigned int address) {
  /*
   * split the 32-bit handler address into two 16-bit halves:
   *   offset_high — bits 16..31
   *   offset_low  — bits  0..15
   */
  idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF;
  idt_descriptors[index].offset_low  = address & 0xFFFF;

  /*
   * Kernel code segment selector (index 1 in the gdt, rpl=0).
   * The reserved byte must be zero per Intel spec.
   */
  idt_descriptors[index].segement_selector = 0x08;
  idt_descriptors[index].reserved = 0x00;

  /*
   * type_and_attr byte layout (bits 15..0 of the gate descriptor):
   *   Bit  | 15           | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
   *   Field| P (present)  | DPL   | S  | D     GateType  | 0 0 0 | reserved  |
   *
   *   P        = 1  (handler present in memory)
   *   DPL      = 00 (ring 0 only — kernel privilege)
   *   S        = 0  (interrupt/trap gate, not storage segment)
   *   D        = 1  (32-bit gate)
   *   GateType = 0xE (0b1110 = 32-bit interrupt gate)
   */
  idt_descriptors[index].type_and_attr = (0x01 << 7) | (0x00 << 6) | (0x00 << 5) | 0xE;
}

/*
 * install_idt
 * Populate the IDT entries for keyboard and paging interrupts,
 * then load the IDT address into the CPU via lidt.
 * Also remaps the PIC so hardware IRQs don't collide with CPU exceptions.
 */
void install_idt(){
  interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (unsigned int) interrupt_handler_33);
  interrupts_init_descriptor(INTERRUPTS_PAGING, (unsigned int) interrupt_handler_14);

  idt.address  = (unsigned int) &idt_descriptors;
  idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
  load_idt((unsigned int) &idt);

  pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
}

/*
 * interrupt_handler
 * Generic C-level interrupt dispatcher called from the asm stub.
 *
 * @param cpu        Register snapshot pushed by the asm stub (unused for now).
 * @param stack      Stack frame pushed by the CPU on exception/interrupt (unused for now).
 * @param interrupt  Interrupt vector number identifying which handler to run.
 */
void interrupt_handler(struct cpu_state *cpu, struct stack_state *stack, unsigned int interrupt) {
  (void)cpu;
  (void)stack;

  unsigned char scan_code;
  unsigned char ascii;

  switch (interrupt) {
  case INTERRUPTS_KEYBOARD:
    scan_code = read_scan_code();

    if (scan_code <= KBD_MAX_ASCII) {
      ascii = kbd_scan_code_to_ascii(scan_code);
      serial_configure_baud_rate(SERIAL_COM1_BASE, 4);
      serial_configure_line(SERIAL_COM1_BASE);

      char str[1];
      str[0] = ascii;
      serial_write_str(str);
    }

    pic_acknowledge(interrupt);
    break;

  case INTERRUPTS_PAGING:
    break;

  default:
    break;
  }
}
