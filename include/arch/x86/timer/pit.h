#ifndef _ARCH_INT_TIMER_AXOCORE_H
#define _ARCH_INT_TIMER_AXOCORE_H 1

#include <stdint.h>

#define REG_TIMER_DATA0     0x40
#define REG_TIMER_DATA1     0x41
#define REG_TIMER_DATA2     0x42
#define REG_TIMER_CTRL      0x43

#define TIMER_BASE_FREQ 1193182

#define IRQ_CHANNEL0 0b00
#define IRQ_CHANNEL1 0b01
#define IRQ_CHANNEL2 0b10

#define ACCESS_LM_SB 0b11

#define TIMER_MODE_SQUARE_WAVE_GEN 0b011

#define TIMER_FORMAT_BIN 0
#define TIMER_FORMAT_BIN_DEC 1

static uint32_t tick = 0;
static uint32_t cur_freq = 20;
void pit_set_freq();    


#endif // _ARCH_INT_TIMER_AXOCORE_H