#include <arch/x86/interrupts/isr.h>
#include <arch/x86/interrupts/idt.h>
#include <utils/byte_io.h>
#include <arch/x86/timer/pit.h>
#include <stdint.h>
#include <drivers/axoio.h>

void pit_set_freq() {
    if (cur_freq == 0) return;
    
    uint16_t divisor = TIMER_BASE_FREQ / cur_freq;
    print_fmt("PIT divisor = %d\n", divisor);
    if (divisor == 0 || divisor > 0xFFFF) return;
    
    uint8_t cmd = (IRQ_CHANNEL0 << 6) | \
                  (ACCESS_LM_SB << 4) | \
                  (TIMER_MODE_SQUARE_WAVE_GEN << 1) | \
                  TIMER_FORMAT_BIN;

    __asm__ volatile ("cli");
    
    outB(REG_TIMER_CTRL, cmd);
    outB(REG_TIMER_DATA0, (uint8_t)(divisor & 0xFF));
    outB(REG_TIMER_DATA0, (uint8_t)((divisor >> 8) & 0xFF));
    
    idt_set_gate(0x20, (uint32_t)pit_handler, 0x08, 0x8E);
    
    __asm__ volatile ("sti");
}