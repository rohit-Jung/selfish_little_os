#include <kernel/isr.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <kernel/pic.h>
#include <kernel/serial.h>

/* isr_dispatch
 * The C side of every interrupt. Reached from common_interrupt_handler in
 * isr.s, which has already saved the registers and normalised the stack so
 * that all 256 vectors arrive here looking identical.
 *
 * @param cpu       Register snapshot pushed by the stub (unused for now)
 * @param stack     Error code and CPU-pushed frame (unused for now)
 * @param interrupt Vector number identifying what fired
 */
void isr_dispatch(struct cpu_state *cpu, struct stack_state *stack,
                  u32 interrupt) {
  (void)cpu;
  (void)stack;

  u8 scan_code;
  u8 ascii;

  switch (interrupt) {
  case INTERRUPTS_KEYBOARD:
    /* Reading the data port is mandatory, not merely useful: the keyboard
     * controller will not deliver another scancode until its output buffer
     * has been drained. */
    scan_code = kbd_read_scan_code();

    if (scan_code <= KBD_MAX_ASCII) {
      ascii = kbd_scan_code_to_ascii(scan_code);

      char str[2];
      str[0] = (char)ascii;
      str[1] = '\0';
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
