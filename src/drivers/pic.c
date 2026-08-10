#include <kernel/io.h>
#include <kernel/pic.h>

/* pic_acknowledge
 * Sends End Of Interrupt so the PIC will deliver on that line again. Until
 * this arrives the controller keeps its in-service bit latched and stays
 * silent.
 *
 * @param interrupt The vector number that fired
 */
void pic_acknowledge(u32 interrupt) {
  if (interrupt < PIC_1_START_INTERRUPT || interrupt > PIC_2_END_INTERRUPT) {
    return;
  }

  /* TODO: an IRQ from the slave passed through the master on its way here,
   * so both controllers need the EOI. Latent while the slave is fully
   * masked; a bug the moment any of IRQ 8-15 is enabled. */
  if (interrupt < PIC_2_START_INTERRUPT) {
    outb(PIC_1_COMMAND, PIC_ACK);
  } else {
    outb(PIC_2_COMMAND, PIC_ACK);
  }
}

/* pic_remap
 * Reinitialises both 8259s and moves their vector bases.
 * ICW1: begin initialisation, an ICW4 will follow
 * ICW2: master and slave vector offsets
 * ICW3: wire master IRQ2 to the slave; tell the slave its cascade identity
 * ICW4: 8086/88 mode
 * OCW1: interrupt mask — keyboard only on the master, nothing on the slave
 *
 * @param offset1 Master PIC base vector (typically 0x20)
 * @param offset2 Slave PIC base vector  (typically 0x28)
 */
void pic_remap(u8 offset1, u8 offset2) {
  outb(PIC_1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
  outb(PIC_2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);

  outb(PIC_1_DATA, offset1); /* ICW2: master vector offset */
  outb(PIC_2_DATA, offset2); /* ICW2: slave vector offset  */

  /* ICW3 uses two different encodings for the same fact. The master takes a
   * bitmask — 0000 0100, "a slave hangs off my IRQ 2". The slave takes a
   * plain number — "your cascade identity is 2". */
  outb(PIC_1_DATA, 0x04);
  outb(PIC_2_DATA, 0x02);

  outb(PIC_1_DATA, PIC_ICW4_8086);
  outb(PIC_2_DATA, PIC_ICW4_8086);

  /* Interrupt mask register: a set bit disables that IRQ. */
  outb(PIC_1_DATA, 0xFD); /* 1111 1101 — IRQ 1 (keyboard) only */
  outb(PIC_2_DATA, 0xFF); /* everything masked                 */
}
