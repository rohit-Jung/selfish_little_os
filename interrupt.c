#include "interrupt.h"
#include "keyboard.h"
#include "pic.h"
#include "io.h"
#include "serial.h"

/* interrupt_handler
 * Generic wrapper for different interrupts
 *
 * @param struct cpu_state   The State of CPU before the interrupt
 * @param struct stack_state The State of Stack before the interrupt
 */
void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt)
{
	unsigned char scan_code;
	unsigned char ascii;

	switch(interrupt){
		case INTERRUPTS_KEYBOARD:
			scan_code = read_scan_code();

			if (scan_code <= KBD_MAX_ASCII){
				ascii  = kbd_scan_code_to_ascii(scan_code);
				serial_configure_baud_rate(SERIAL_COM1_BASE, 4);
				serial_configure_line(SERIAL_COM1_BASE);

				char str[1];
				str[0] = ascii;
				serial_write_str(str);
			}

			pic_acknowledge(interrupt);
			break;

		case INTERRUPTS_PAGING:
			break;

		default:
			break;
	}
}

