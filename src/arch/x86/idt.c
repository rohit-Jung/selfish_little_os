#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/pic.h>
#include <kernel/serial.h>

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
  for (int i = 0; i < IDT_ENTRY_COUNT; i++) {
    idt_set_gate(i, interrupt_handler_table[i]);
  }

  idt.address = (u32)&idt_gates;
  /* the limit field is the size in bytes minus one */
  idt.size = (sizeof(struct idt_gate) * IDT_ENTRY_COUNT) - 1;
  idt_load((u32)&idt);

  pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
}

/* returns exception name for different u32 according to x86 */
const char *exception_name(u32 v) {
  switch (v) {
  case 0:
    return "#DE Divide error";
  case 1:
    return "#DB Debug error";
  case 2:
    return "NM";
  case 3:
    return "#BP Breakpoint Error";
  case 4:
    return "#OF Overflow Error";
  case 5:
    return "#BR Bound Range";
  case 6:
    return "#UD invalid opcode";
  case 7:
    return "#NM device not available";
  case 8:
    return "#DF double fault";
  case 10:
    return "#TS invalid TSS";
  case 11:
    return "#NP segment not present";
  case 12:
    return "#SS stack fault";
  case 13:
    return "#GP general protection";
  case 14:
    return "#PF Page Fault";
  case 16:
    return "#MF x87 fd";
  case 17:
    return "#AC alignement check";
  case 18:
    return "#MC machine check";
  case 19:
    return "#XM simd fd";
  default:
    return "reserved /unknown";
  }
}

void panic(struct cpu_state *cpu, struct stack_state *stack, u32 v) {
  serial_write_str("\n **EXCEPTION**\n");
  serial_write_hex(v);
  serial_write_str("  ");
  serial_write_str((char *)exception_name(v));
  serial_write_str("\n err=");
  serial_write_hex(stack->error_code);
  serial_write_str("\n eip=");
  serial_write_hex(stack->eip);
  serial_write_str("\n cs=");
  serial_write_hex(stack->cs);
  serial_write_str("\n flags=");
  serial_write_hex(stack->eflags);
  serial_write_str("\n eax=");
  serial_write_hex(cpu->eax);
  serial_write_str("\n ebx=");
  serial_write_hex(cpu->ebx);
  serial_write_str("\n esp=");
  serial_write_hex(cpu->esp);
  serial_write_str("\n ***halted \n");

  for (;;) {
    __asm__ __volatile__("cli; hlt");
  } // right way to stop instead of bare while
}
