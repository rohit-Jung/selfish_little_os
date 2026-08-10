#include <kernel/io.h>
#include <kernel/keyboard.h>

/* kbd_read_scan_code
 * Reads a scancode from the keyboard controller.
 *
 * @return The scancode (NOT ASCII)
 */
u8 kbd_read_scan_code(void) { return inb(KBD_DATA_PORT); }

/* kbd_scan_code_to_ascii
 * Maps a US QWERTY make code to an ASCII character. Unshifted only; no
 * modifier state, and no handling of the 0xE0-prefixed extended codes.
 *
 * @return Corresponding ASCII code, or 0 if the key does not produce one
 */
u8 kbd_scan_code_to_ascii(u8 scan_code) {
  /* static so it lives in .rodata; a stack-local initializer makes gcc
   * emit a 256-byte inline copy (SSE, or a memcpy we do not have) */
  static const u8 ascii[256] = {
      0x0,  0x0, '1', '2',  '3', '4', '5', '6',  // 0 - 7
      '7',  '8', '9', '0',  '-', '=', 0x0, 0x0,  // 8 - 15
      'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',  // 16 - 23
      'o',  'p', '[', ']',  '\n', 0x0, 'a', 's', // 24 - 31
      'd',  'f', 'g', 'h',  'j', 'k', 'l', ';',  // 32 - 39
      '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v',  // 40 - 47
      'b',  'n', 'm', ',',  '.', '/', 0x0, '*',  // 48 - 55
      0x0,  ' ', 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  // 56 - 63
      0x0,  0x0, 0x0, 0x0,  0x0, 0x0, 0x0, '7',  // 64 - 71
      '8',  '9', '-', '4',  '5', '6', '+', '1',  // 72 - 79
      '2',  '3', '0', '.'                        // 80 - 83
  };

  return ascii[scan_code];
}
