#include <arch/x86/interrupts/isr.h>
#include <arch/x86/interrupts/pic.h>
#include <arch/x86/timer/pit.h>
#include <utils/byte_io.h>
#include <drivers/axoio.h>
#include <stdint.h>

void default_handler_body() {}
__attribute__((naked))
void default_handler() {
    __asm__ volatile(
        "pusha\n"
        "call default_handler_body\n"
        "popa\n"
        "iret"
    );
}

void keyboard_handler_body() {
    uint8_t sc = inB(REG_INPUT_DATA);
    // uint8_t attr = make_color(VGA_BYTE_BLACK, VGA_BYTE_LIGHT_GREY);
    // if (sc < 0x80) {
    //     uint32_t i = 0;
    //     char* key = find_key_by_scancode(sc);
    //     switch (sc) {
    //         inputBuffer[i] = sc;
    //         i++;
    //     }
    //     for (uint32_t j = 0; j < i; i++) {
    //         print_str(find_key_by_scancode(inputBuffer[j]), 0x0F);
    //     }
    // }
    
    print_fmt("sc: %X. ", sc);
    if (sc & 0x80) print_fmt("u\n");
    else print_fmt("p\n");
    
    sendEOI(1);
}
__attribute__((naked))
void keyboard_handler() {
    __asm__ volatile(
        "pusha\n"
        "call keyboard_handler_body\n"
        "popa\n"
        "iret\n"
    );
}

void pit_handler_body() {
    print_fmt("TICK = %d\n", tick);
    tick++;
    sendEOI(0);
}

__attribute__((naked))
void pit_handler() {
    __asm__ volatile(
        "pusha\n"
        "call pit_handler_body\n"
        "popa\n"
        "iret\n"
    );
}