#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H

#include <kernel/types.h>

/* Null descriptor, kernel code, kernel data */
#define GDT_ENTRY_COUNT 3

#define GDT_BASE  0
#define GDT_LIMIT 0xFFFFF

/* access_byte values
 *   0x9A = 1001 1010: present, ring 0, code segment, execute/read
 *   0x92 = 1001 0010: present, ring 0, data segment, read/write
 */
#define GDT_CODE_TYPE 0x9A
#define GDT_DATA_TYPE 0x92

/*
 * Flags nibble of `limit_and_flags`.
 * 1100
 * 0 - Available for system use
 * 0 - Long mode
 * 1 - Size (0 for 16-bit, 1 for 32-bit)
 * 1 - Granularity (0 for 1B units, 1 for 4KB units)
 */
#define GDT_FLAGS 0x0C

/* Loaded into the GDTR by `lgdt`. `size` is a limit: bytes minus one. */
struct gdt_ptr {
  u16 size;
  u32 address;
} __attribute__((packed));

struct gdt_entry {
  u16 limit_low;
  u16 base_low;
  u8  base_middle;
  u8  access_byte;
  u8  limit_and_flags;
  u8  base_high;
} __attribute__((packed));

void gdt_set_entry(int index, u32 base_address, u32 limit, u8 access_byte,
                   u8 flags);
void gdt_install(void);

/* Defined in src/arch/x86/gdt.s */
void gdt_load(struct gdt_ptr ptr);
void gdt_reload_segments(void);

#endif /* KERNEL_GDT_H */
