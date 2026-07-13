#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/**
 * outb: sends data  to the given I/O port. Defined in io.s
 *
 * @param port  The I/O port to send data to 
 * @param data  The data to send to I/O port 
 */
void outb(unsigned short port, unsigned char data);

/* inb:
 * Read a byte from an I/O port
 *
 * @param  port The address I/O port  
 * @return      The read byte
 * 
 */
unsigned char inb(unsigned short port);

#endif /* INCLUDE_IO_H */
