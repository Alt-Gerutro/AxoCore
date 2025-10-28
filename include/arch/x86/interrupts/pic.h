#ifndef _ARCH_INT_PIC_AXOCORE_H
#define _ARCH_INT_PIC_AXOCORE_H 1

#include <stdint.h>
#include <utils/byte_io.h>

#define PIC_MASTER_CMD      0x20
#define PIC_MASTER_DATA     0x21
#define PIC_SLAVE_CMD       0xA0
#define PIC_SLAVE_DATA      0xA1

#define PIC_EOI             0x20

#define ICW1_ICW4           0x01
#define ICW1_SINGLE         0x02
#define ICW1_INTERVAL4      0x04
#define ICW1_LEVEL          0x08
#define ICW1_INIT           0x10

#define ICW4_8086           0x01
#define ICW4_AUTO           0x02
#define ICW4_BUF_SLAVE      0x08
#define ICW4_BUF_MASTER     0x0C
#define ICW4_SFNM           0x10

#define CASCADE_IRQ         2

void pic_remap(uint32_t offset1, uint32_t offset2);
void pic_disable();
void irq_mask(uint8_t irqLine);
void irq_unmask(uint8_t irqLine);

void sendEOI(uint8_t irq);

#endif // _ARCH_INT_PIC_AXOCORE_H