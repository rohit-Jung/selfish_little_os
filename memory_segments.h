#ifndef INCLUDE_MEMORY_SEGMENTS
#define INCLUDE_MEMORY_SEGMENTS

#define SEGMENT_DESCRIPTOR_COUNT 3

#define SEGMENT_BASE 0
#define SEGMENT_LIMIT 0xFFFFF

#define SEGMENT_CODE_TYPE 0x9A
#define SEGMENT_DATA_TYPE 0x92

/*
 * Flags part of `limit_and_flags`.
 * 1100
 * 0 - Available for system use
 * 0 - Long mode
 * 1 - Size (0 for 16-bit, 1 for 32)
 * 1 - Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
 */
#define SEGMENT_FLAGS_PART 0x0C

struct GDT {
  unsigned short size;
  unsigned int address;
} __attribute__((packed));

struct GDTDescriptor {
  unsigned short limit_low;
  unsigned short base_low;
  unsigned char base_middle;
  unsigned char access_byte;
  unsigned char limit_and_flags;
  unsigned char base_high;
} __attribute__((packed));

void segments_init_descriptor(int index, unsigned int base_address,
                              unsigned int limit, unsigned char access_byte,
                              unsigned char flags);
void segments_install_gdt();

// asm wrappers
void segments_load_gdt(struct GDT gdt);
void segments_load_registers();

#endif // !INCLUDE_MEMORY_SEGMENTS
