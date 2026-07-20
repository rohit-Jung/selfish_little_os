#include "io.h"
#include "pic.h"

/* pic_acknowledge
 * Acknowledges an interrupt from either PIC 1 or PIC 2
 *
 * @param num The interrupt number 
 */
void pic_acknowledge(unsigned int interrupt) 
{
	if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
		return;
	}

	if (interrupt < PIC2_START_INTERRUPT) {
		outb(PIC_1, PIC_ACK);
	} else {
		outb(PIC_2, PIC_ACK);
	}
}

/* pic_remap
 * Remaps the 8259 PIC IRQ base vectors and initializes both controllers.
 * ICW1: init + ICW4 needed
 * ICW2: set master vector offset to `offset1`, slave to `offset2`
 * ICW3: wire master IRQ2 to slave; tell slave its cascade identity
 * ICW4: 8086/88 (MCS-80/85) mode
 * IMR:  mask all IRQs except keyboard (IRQ1) on master, all masked on slave
 *
 * @param offset1  Master PIC base vector (typ. 0x20)
 * @param offset2  Slave  PIC base vector (typ. 0x28)
 */
void pic_remap(int offset1, int offset2)
{
  outb(PIC_1_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4); // starts the initialization sequence (in cascade mode)
  outb(PIC_2_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4); 
  outb(PIC_1_DATA, offset1);           // ICW2 master PIC vector offset
  outb(PIC_2_DATA, offset2);           // ICW2 slave PIC vector offset
  outb(PIC_1_DATA, 4);                 // ICW3: tell the master PIC of slave PIC at IRQ2 (0000 0100)
  outb(PIC_2_DATA, 2);                 // ICW3: tell slave it's cascade indentity (0000 0100) 

  outb(PIC_1_DATA, PIC_ICW4_8086);     // tell PIC using 0x01 - 8086style CPU
  outb(PIC_2_DATA, PIC_ICW4_8086);

  // setup interrupt mask register (IMR)
  outb(PIC_1_DATA, 0xFD); // 1111 1101 enable IRQ 1 only (keyboard)
  outb(PIC_2_DATA, 0xFF);

  __asm__ __volatile__("sti"); /* enable interrupts */
}
