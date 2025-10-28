#include "arch/x86/interrupts/pic.h"
#include "drivers/video/screen.h"
#include <config/kernel/kernel_config.h>
#include <arch/x86/interrupts/interrupts.h>
#include <drivers/axoio.h>
#include <stdint.h>
#include <arch/x86/timer/pit.h>

MULTIBOOT_HEADER;

/**
 * @brief Triggers a kernel panic with formatted error message.
 * 
 * @param msg Format string for the error message (print_fmt-style)
 * @param ... Variable arguments for the format string
 * 
 * @note This function does not return (noreturn attribute).
 * @note The function is marked as used to prevent removal by linker optimizations.
 */
__attribute__((noreturn, used))
void kpanic(char* msg, ...) {
    clear_screen();
    char buf[2048];
    va_list args;
    va_start(args, msg);
    vformat(buf, 2048, msg, args);
    va_end(args);
    print_str(buf, make_color(VGA_BYTE_DARK_RED, VGA_BYTE_BLACK));

    __asm__ volatile("cli");
    while (1) __asm__ volatile("hlt");
}

/**
 * @brief Kernel entry point function.
 * 
 * @note This function does not return (noreturn attribute).
 * @note The function is marked as used to prevent removal by linker optimizations.
 */
__attribute__((noreturn, used))
void kmain() {
    int_init();
    pit_set_freq();
    uint32_t n = 20;
    if (tick == n) {
        pit_set_freq();
        tick = 0;
        cur_freq += 10;
        n = cur_freq;
    }
    irq_unmask(0);
    
    keyboard_init();
    

    while (1) __asm__ volatile ("hlt");
}