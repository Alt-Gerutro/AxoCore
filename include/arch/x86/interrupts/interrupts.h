#ifndef _ARCH_INTERRUPTS_AXOCORE_H
#define _ARCH_INTERRUPTS_AXOCORE_H 1

#include <arch/x86/interrupts/pic.h>
#include <arch/x86/interrupts/gdt.h>
#include <arch/x86/interrupts/idt.h>

void int_init();

#endif // _ARCH_INTERRUPTS_AXOCORE_H