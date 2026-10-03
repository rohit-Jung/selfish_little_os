#ifndef KERNEL_SERIAL_H
#define KERNEL_SERIAL_H

#include <kernel/types.h>

/* All the I/O ports are calculated relative to the data port, because all the
 * serial ports (COM1, COM2, COM3, COM4) have their registers in the same
 * order but start at different base addresses.
 */
#define SERIAL_COM1_BASE 0x3F8

#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_INTERRUPT_ENABLE_PORT(base) ((base) + 1)
#define SERIAL_FIFO_COMMAND_PORT(base) ((base) + 2)
#define SERIAL_LINE_COMMAND_PORT(base) ((base) + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) ((base) + 4)
#define SERIAL_LINE_STATUS_PORT(base) ((base) + 5)

/* SERIAL_LINE_ENABLE_DLAB
 * Bit 7 of the line control register. While set, offsets +0 and +1 stop
 * being the data and interrupt-enable registers and become the low and high
 * halves of the baud rate divisor.
 */
#define SERIAL_LINE_ENABLE_DLAB 0x80

/* Bit 5 of the line status register: transmitter holding register empty. */
#define SERIAL_LINE_STATUS_THR_EMPTY 0x20

void serial_init(void);
void serial_configure_baud_rate(u16 com, u16 divisor);
void serial_configure_line(u16 com);
int serial_is_transmit_fifo_empty(u16 com);
int serial_write_str(char *buf);
void serial_write_hex(u32 value);

#endif /* KERNEL_SERIAL_H */
