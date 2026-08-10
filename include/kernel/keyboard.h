#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#include <kernel/types.h>

#define KBD_DATA_PORT 0x60

/* Highest scancode the translation table covers. Break codes (key release)
 * are the make code with bit 7 set, so every one of them is >= 0x80 and is
 * filtered out by this bound. */
#define KBD_MAX_ASCII 83

u8 kbd_read_scan_code(void);
u8 kbd_scan_code_to_ascii(u8 scan_code);

#endif /* KERNEL_KEYBOARD_H */
