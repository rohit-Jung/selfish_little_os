#include <kernel/fb.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/serial.h>

int kmain(void) {
  char message[] = "Sleep rokshh";

  /* the gdt goes first: the idt's gate descriptors name a code segment
   * selector, which has to resolve against a gdt we control. */
  gdt_install();
  serial_init();

  fb_clear();
  fb_move_cursor(6 * FB_COLUMNS);
  fb_write_str(message);
  serial_write_str(message);

  idt_install();
  __asm__ __volatile__("sti"); /* enable interrupts */

  while (1) {
  }; /* hang so keyboard interrupt can fire */

  return 0;
}
