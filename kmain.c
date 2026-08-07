#include "framebuffer.h"
#include "interrupt.h"
#include "memory_segments.h"
#include "serial.h"

int kmain() {
  char message[] = "Sleep rokshh";

  segments_install_gdt();
  serial_init();

  fb_clear();
  fb_move_cursor(6 * 80);
  /* fb_write_simple(); */
  fb_write_str(message);
  serial_write_str(message);

  install_idt();
  __asm__ __volatile__("sti"); /* enable interrupts */

  while (1) {
  }; /* hang so keyboard interrupt can fire */

  return 0;
}
