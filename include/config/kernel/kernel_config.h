#ifndef _CONFIG_AXOCORE_H
#define _CONFIG_AXOCORE_H

#define MULTIBOOT_MAGIC     0x1BADB002
#define MULTIBOOT_FLAGS     0b00000011
#define MULTIBOOT_CHECKSUM  -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

#define MULTIBOOT_HEADER    __attribute__((section(".multiboot"), used)) \
                            static const struct { \
                                    uint32_t magic; \
                                    uint32_t flags; \
                                    uint32_t checksum; \
                            } multiboot_header = { MULTIBOOT_MAGIC, MULTIBOOT_FLAGS, MULTIBOOT_CHECKSUM };


__attribute__((noreturn, used))
void kpanic(char* msg, ...);

#endif // _CONFIG_AXOCORE_H