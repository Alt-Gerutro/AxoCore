#ifndef _UTILS_BYTEIO_AXOCORE_H
#define _UTILS_BYTEIO_AXOCORE_H

#include <stdint.h>

static inline uint8_t inB(uint16_t port) {
    uint8_t v;
    __asm__ volatile("inb %1, %0"
        : "=a"(v)
        : "Nd"(port)
    );
    return v;
}

static inline void outB(uint16_t port, uint8_t byte) {
    __asm__ volatile("outb %0, %1"
        :
        : "a"(byte),
        "Nd"(port)
    );
}

static inline uint8_t inW(uint16_t port) {
    uint16_t v;
    __asm__ volatile("inw %1, %0"
        : "=a"(v)
        : "Nd"(port)
    );
    return v;
}

static inline void outW(uint16_t port, uint16_t word) {
    __asm__ volatile("outw %0, %1"
        :
        : "a"(word),
        "Nd"(port)
    );
}

static inline uint32_t inDW(uint16_t port) {
    uint32_t v;
    __asm__ volatile("indw %1, %0"
        : "=a"(v)
        : "Nd"(port)
    );
    return v;
}

static inline void outDW(uint16_t port, uint32_t dw) {
    __asm__ volatile("outdw %0, %1"
        :
        : "a"(dw),
        "Nd"(port)
    );
}

static inline void ioWait(void) {
    __asm__ volatile("outb %%al, $0x80"
        :
        :
        "a"(0)
    );
}

static inline void mem_copy(uint8_t* src, uint8_t* dest, uint32_t bytes) {
    uint32_t i = 0;
    while (i < bytes) {
        dest[i] = src[i];
        i++;
    }
}

#endif // _UTILS_BYTEIO_AXOCORE_H