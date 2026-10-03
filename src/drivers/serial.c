#include <kernel/io.h>
#include <kernel/serial.h>

void serial_init(void) {
  serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
  serial_configure_line(SERIAL_COM1_BASE);

  /* enable FIFO, clear both queues, 14-byte trigger level */
  outb(SERIAL_FIFO_COMMAND_PORT(SERIAL_COM1_BASE), 0xC7);
  /* RTS + DTR set, OUT2 enabled (required for IRQs on real hardware) */
  outb(SERIAL_MODEM_COMMAND_PORT(SERIAL_COM1_BASE), 0x0B);
}

/* serial_configure_baud_rate
 * Sets the speed of the data being sent. The base clock is 115200 bits/s and
 * the argument divides it, so the resulting speed is (115200 / divisor).
 *
 * Setting DLAB remaps offsets +0 and +1 onto the two halves of the divisor
 * latch; serial_configure_line clears it again.
 *
 * @param com     The COM port to configure
 * @param divisor The divisor
 */
void serial_configure_baud_rate(u16 com, u16 divisor) {
  outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);

  outb(SERIAL_DATA_PORT(com), divisor & 0xFF);           /* DLL */
  outb(SERIAL_INTERRUPT_ENABLE_PORT(com), divisor >> 8); /* DLM */
}

/* serial_configure_line
 * Configures the line to 8 data bits, no parity, one stop bit, break control
 * disabled. Writing this also clears DLAB, which is why the baud rate has to
 * be set first.
 *
 * @param com The serial port to configure
 */
void serial_configure_line(u16 com) {
  /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
   * Content: | d | b | prty  | s | dl  |
   * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
   */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/* serial_is_transmit_fifo_empty
 * Checks whether the transmit FIFO has room for another byte.
 *
 * @param  com The COM port
 * @return 0   if the transmit FIFO queue is not empty
 *         1   if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(u16 com) {
  return inb(SERIAL_LINE_STATUS_PORT(com)) & SERIAL_LINE_STATUS_THR_EMPTY;
}

/* serial_write_str
 * Writes a null-terminated string to COM1, one character at a time,
 * busy-waiting until the transmit FIFO is ready for each byte.
 *
 * @param  buf The null-terminated string to transmit
 * @return     The number of characters transmitted
 */
int serial_write_str(char *buf) {
  u32 i = 0;
  while (buf[i] != '\0') {
    while (!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE)) {
      /* busy-wait: spin until FIFO has room */
    }
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), buf[i]);
    i++;
  }

  return i;
}

/* serial_write_hex
 * Writes a 32-bit value to COM1 as 0x-prefixed hexadecimal.
 *
 * @param value The value to print
 */
void serial_write_hex(u32 value) {
  static const char hex[] = "0123456789ABCDEF";

  serial_write_str("0x");

  for (int i = 28; i >= 0; i -= 4) {
    u32 digit = (value >> i) & 0xF;
    char c[2];
    c[0] = hex[digit];
    c[1] = '\0';
    serial_write_str(c);
  }
}
