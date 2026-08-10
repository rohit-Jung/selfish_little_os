#include <kernel/gdt.h>

static struct gdt_entry gdt_entries[GDT_ENTRY_COUNT];

/* gdt_set_entry
 * Populate one 8-byte segment descriptor. The base and limit are each split
 * across non-adjacent fields, a leftover of the 286 descriptor format that
 * the 386 extended rather than redesigned.
 *
 * @param index        Index into the GDT
 * @param base_address Linear address the segment starts at
 * @param limit        Segment length, in units chosen by the granularity flag
 * @param access_byte  Present / privilege / type bits
 * @param flags        Granularity and size nibble
 */
void gdt_set_entry(int index, u32 base_address, u32 limit, u8 access_byte,
                   u8 flags) {
  gdt_entries[index].base_low = base_address & 0xFFFF;
  gdt_entries[index].base_middle = (base_address >> 16) & 0xFF;
  gdt_entries[index].base_high = (base_address >> 24) & 0xFF;

  gdt_entries[index].limit_low = limit & 0xFFFF;
  gdt_entries[index].limit_and_flags = (limit >> 16) & 0x0F;
  gdt_entries[index].limit_and_flags |= (flags << 4) & 0xF0;

  gdt_entries[index].access_byte = access_byte;
}

/* gdt_install
 * Build a flat memory model — one code and one data segment, both spanning
 * the whole 4 GB address space — and load it.
 */
void gdt_install(void) {
  gdt_entries[0].base_low = 0;
  gdt_entries[0].base_middle = 0;
  gdt_entries[0].base_high = 0;
  gdt_entries[0].limit_low = 0;
  gdt_entries[0].access_byte = 0;
  gdt_entries[0].limit_and_flags = 0;

  /* Entry 0 must be the null descriptor: the processor never reads it, but
   * some emulators complain if it is absent. Since it is unused, the 6-byte
   * GDTR value is parked inside it. See http://wiki.osdev.org/GDT_Tutorial
   */
  struct gdt_ptr *ptr = (struct gdt_ptr *)gdt_entries;
  ptr->address = (u32)gdt_entries;
  ptr->size = (sizeof(struct gdt_entry) * GDT_ENTRY_COUNT) - 1;

  gdt_set_entry(1, GDT_BASE, GDT_LIMIT, GDT_CODE_TYPE, GDT_FLAGS);
  gdt_set_entry(2, GDT_BASE, GDT_LIMIT, GDT_DATA_TYPE, GDT_FLAGS);

  gdt_load(*ptr);
  gdt_reload_segments();
}
