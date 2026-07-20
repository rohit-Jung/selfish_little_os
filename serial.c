#include "serial.h"
#include "io.h"

/* serial_configure_baud_rate:
 * Sets the speed of the data being sent. The default speed of a serial
 * port is 115200 bits/s. The argument is a divisor of that number, hence
 * the resulting speed becomes (115200 / divisor) bits/s.
 *
 * @param com  		The COM port to configure
 * @param divisor The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
  outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
  outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/* serial_configure_line
 * Configures the line of the given serial port. The port is set
 * to have the data length of 8 bytes, no parity bits, one stop bit
 * and break control disabled.
 *
 * @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com) {

  /* Bit: 	  | 7 | 6 | 5 4 3 | 2 | 1 0 |
   * Content: | d | b | prty  | s | dl  |
   * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
   */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/* serial_is_transmit_fifo_empty
 * Checks whether the transmit fifo is emtpy for the given com
 *
 * @param   com The COM port
 * @return  0   if the transmit FIFO queue is not empty
 *  		    1 	if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned short com) {
  /* 0x20 = 0010 0000 */
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/* serial_write_str
 * Writes a null-terminated string to COM1, one character at a time,
 * busy-waiting until the transmit FIFO is ready for each byte.
 *
 * @param buf  The null-terminated string to transmit
 * @return     The number of characters transmitted
 */
int serial_write_str(char *buf) {
  serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
  serial_configure_line(SERIAL_COM1_BASE);

  unsigned int i = 0;
  while (buf[i] != '\0') {
    while (!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE)) {
      /* busy-wait: spin until FIFO has room */
    }
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), buf[i]);
    i++;
  }

  return i;
}
