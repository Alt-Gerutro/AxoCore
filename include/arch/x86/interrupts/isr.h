#ifndef _ARCH_INT_ISR_AXOCORE_H
#define _ARCH_INT_ISR_AXOCORE_H 1

void default_handler_body();
void default_handler();

void keyboard_handler_body();
void keyboard_handler();

void pit_handler_body();
void pit_handler();

#endif // _ARCH_INT_ISR_AXOCORE_H