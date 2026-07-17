#ifndef INCLUDE_KEYBOARD_H

#define KBD_DATA_PORT 0x60
#define KBD_MAX_ASCII 83 

unsigned char read_scan_code(void);
unsigned char kbd_scan_code_to_ascii(unsigned char);
#endif
