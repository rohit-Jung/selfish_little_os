#ifndef KERNEL_PIC_H
#define KERNEL_PIC_H

#include <kernel/types.h>

/* I/O base addresses of the two cascaded 8259 controllers */
#define PIC_1 0x20 /* master */
#define PIC_2 0xA0 /* slave  */

#define PIC_1_COMMAND PIC_1
#define PIC_1_DATA (PIC_1 + 1)
#define PIC_2_COMMAND PIC_2
#define PIC_2_DATA (PIC_2 + 1)

/* Vector offsets the PICs are remapped to. The BIOS leaves IRQ 0-7 mapped
 * onto vectors 8-15, which collide with Intel's reserved exception range,
 * so the master is moved to 32 and the slave to 40.
 */
#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28

/* Inclusive range of vectors the PICs deliver after remapping */
#define PIC_1_START_INTERRUPT PIC_1_OFFSET
#define PIC_2_START_INTERRUPT PIC_2_OFFSET
#define PIC_2_END_INTERRUPT (PIC_2_START_INTERRUPT + 7)

/* Initialisation Command Word 1 */
#define PIC_ICW1_ICW4 0x01      /* an ICW4 will follow          */
#define PIC_ICW1_SINGLE 0x02    /* single, rather than cascade  */
#define PIC_ICW1_INTERVAL4 0x04 /* call address interval 4      */
#define PIC_ICW1_LEVEL 0x08     /* level-triggered, not edge    */
#define PIC_ICW1_INIT 0x10      /* required; identifies an ICW1 */

/* Initialisation Command Word 4 */
#define PIC_ICW4_8086 0x01       /* 8086/88 mode              */
#define PIC_ICW4_AUTO 0x02       /* automatic EOI             */
#define PIC_ICW4_BUF_SLAVE 0x08  /* buffered mode, slave      */
#define PIC_ICW4_BUF_MASTER 0x0C /* buffered mode, master     */
#define PIC_ICW4_SFNM 0x10       /* special fully nested mode */

/* End Of Interrupt, written to a command port */
#define PIC_ACK 0x20

void pic_remap(u8 offset1, u8 offset2);
void pic_acknowledge(u32 interrupt);

#endif /* KERNEL_PIC_H */
