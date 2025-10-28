#include <arch/x86/interrupts/pic.h>

void sendEOI(uint8_t irq) {
    if (irq >= 8) {
        outB(PIC_SLAVE_CMD, PIC_EOI);
    }
    outB(PIC_MASTER_CMD, PIC_EOI);
}

void pic_remap(uint32_t offset1, uint32_t offset2) {
    __asm__ volatile("cli");
    outB(PIC_MASTER_CMD, ICW1_INIT | ICW1_ICW4);
    ioWait();
    outB(PIC_SLAVE_CMD, ICW1_INIT | ICW1_ICW4);
    ioWait();

    outB(PIC_MASTER_DATA, offset1);
    ioWait();
    outB(PIC_SLAVE_DATA, offset2);
    ioWait();

    outB(PIC_MASTER_DATA, 1 << CASCADE_IRQ);
    ioWait();
    outB(PIC_SLAVE_DATA, 2);
    ioWait();

    outB(PIC_MASTER_DATA, ICW4_8086);
    ioWait();
    outB(PIC_SLAVE_DATA, ICW4_8086);
    ioWait();

    outB(PIC_MASTER_DATA, 0);
    outB(PIC_SLAVE_DATA, 0);
    __asm__ volatile("sti");
}

void pic_disable() {
    outB(PIC_MASTER_DATA, 0xFF);
    outB(PIC_SLAVE_DATA, 0xFF);
}

void irq_mask(uint8_t irqLine) {
    uint16_t port;
    uint8_t value;

    if (irqLine < 8) {
        port = PIC_MASTER_DATA;
    } else {
        port = PIC_SLAVE_DATA;
        irqLine -= 8;
    }

    value = inB(port) | (1 << irqLine);
    outB(port, value);
}

void irq_unmask(uint8_t irqLine) {
    uint16_t port;
    uint8_t value;

    if (irqLine < 8) {
        port = PIC_MASTER_DATA;
    } else {
        port = PIC_SLAVE_DATA;
        irqLine -= 8;
    }

    value = inB(port); 
    value &= ~(1 << irqLine);
    outB(port, value);
}