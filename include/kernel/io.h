#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <kernel/types.h>

/* outb
 * Sends a byte to the given I/O port. Defined in src/arch/x86/io.s
 *
 * @param port  The I/O port to send data to
 * @param data  The data to send to the I/O port
 */
void outb(u16 port, u8 data);

/* inb
 * Reads a byte from an I/O port. Defined in src/arch/x86/io.s
 *
 * @param  port The address of the I/O port
 * @return      The byte read
 */
u8 inb(u16 port);

#endif /* KERNEL_IO_H */
