#include <boot/multiboot.h>
#include <kernel/fb.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/serial.h>

int kmain(/* additional args */ unsigned int ebx) {
  char message[] = "Sleep rokshh";

  multiboot_info_t *mbinfo = (multiboot_info_t *)ebx;

  // GRUB telling mods_count/mods_addr are actually populated
  if ((mbinfo->flags & MULTIBOOT_INFO_MODS) && mbinfo->mods_count > 0) {
    multiboot_module_t *mod = (multiboot_module_t *)mbinfo->mods_addr;

    typedef void (*call_module_t)(void);
    call_module_t start_program = (call_module_t)mod->mod_start;
    start_program();
    /* we’ll never get here, unless the module code returns */
  }

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
