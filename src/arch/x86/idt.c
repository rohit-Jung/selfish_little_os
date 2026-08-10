#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/pic.h>

static struct idt_ptr idt;
static struct idt_gate idt_gates[IDT_ENTRY_COUNT];

/* idt_set_gate
 * Populate one 8-byte gate descriptor pointing at the handler at `address`.
 *
 * @param index   Vector number this gate serves
 * @param address 32-bit linear address of the handler stub
 */
void idt_set_gate(int index, u32 address) {
  /* The handler address is stored as two 16-bit halves at opposite ends of
   * the descriptor. */
  idt_gates[index].offset_low = address & 0xFFFF;
  idt_gates[index].offset_high = (address >> 16) & 0xFFFF;

  /* 0x08 = GDT index 1, TI=0 (GDT), RPL=0 — the kernel code segment. */
  idt_gates[index].selector = 0x08;
  idt_gates[index].reserved = 0x00;

  idt_gates[index].type_and_attr =
      IDT_GATE_PRESENT | IDT_GATE_DPL0 | IDT_GATE_INT32;
}

/* idt_install
 * Fill in the gates this kernel handles, load the IDT, and remap the PIC so
 * hardware IRQs stop colliding with the CPU's reserved exception vectors.
 *
 * Gates left zeroed have their present bit clear. Taking one of those raises
 * #GP, which — with no gate 13 either — becomes a double and then a triple
 * fault. Filling the remaining 254 is still outstanding.
 */
void idt_install(void) {
  idt_set_gate(INTERRUPTS_KEYBOARD, (u32)interrupt_handler_33);
  idt_set_gate(INTERRUPTS_PAGING, (u32)interrupt_handler_14);

  idt.address = (u32)&idt_gates;
  /* the limit field is the size in bytes minus one */
  idt.size = (sizeof(struct idt_gate) * IDT_ENTRY_COUNT) - 1;
  idt_load((u32)&idt);

  pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
}
