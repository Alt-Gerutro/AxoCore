#ifndef _ARCH_INT_GDT_AXOCORE_H
#define _ARCH_INT_GDT_AXOCORE_H 1

#include <stdint.h>

#define GDT_SIZE    5

enum gdt_selector_table {
    GDT_SELECTOR_NULL,
    GDT_SELECTOR_R0_CODE,
    GDT_SELECTOR_R0_DATA,
    GDT_SELECTOR_R3_CODE,
    GDT_SELECTOR_R3_DATA
};

enum gdt_selector_index {
    KERNEL_CS = (GDT_SELECTOR_R0_CODE * sizeof(uint64_t)) | 0,
    KERNEL_DS = (GDT_SELECTOR_R0_DATA * sizeof(uint64_t)) | 0,
    USER_CS = (GDT_SELECTOR_R3_CODE * sizeof(uint64_t)) | 3,
    USER_DS = (GDT_SELECTOR_R3_DATA * sizeof(uint64_t)) | 3,

};

extern uint64_t gdt[GDT_SIZE];
struct gdtp;

#define SEG_DESCTYPE(x)     ((x) << 0x04)           /* Descriptor type. (0 - system, 1 - code/data) */
#define SEG_PRESENT(x)      ((x) << 0x07)           /* Present flag */
#define SEG_AVAILABLE(x)    ((x) << 0x0C)           /* Available flag */
#define SEG_LONG(x)         ((x) << 0x0D)           /* Long(64-bit) mode */
#define SEG_SIZE(x)         ((x) << 0x0E)           /* Size of segment. (0 - 16-bit, 1 - 32-bit) */
#define SEG_GRANULARITY(x)  ((x) << 0x0F)           /* Granularity. (0 - 1B-1MB, 1 - 4KB-4GB) */
#define SEG_PRIVILEGE(x)    (((x) & 0x03) << 0x05)  /* Privilege level. (rings 0-3) */

#define SEG_DATA_RD         0x00    /* Read data */
#define SEG_DATA_RDA        0x01    /* Read, accessed data */
#define SEG_DATA_RDWR       0x02    /* Read/Write data */
#define SEG_DATA_RDWRA      0x03    /* Read/Write, accessed data */
#define SEG_DATA_RDEXPD     0x04    /* Read, expand-down data */
#define SEG_DATA_RDEXPDA    0x05    /* Read, expand-down, accessed data */
#define SEG_DATA_RDWREXPD   0x06    /* Read/Write, expand-down data */
#define SEG_DATA_RDWREXPDA  0x07    /* Read/Write, expand-down, accessed data */

#define SEG_CODE_EX         0x08    /* Execute code */
#define SEG_CODE_EXA        0x09    /* Execute, accessed code */
#define SEG_CODE_EXRD       0x0A    /* Execute/Read code */
#define SEG_CODE_EXRDA      0x0B    /* Execute/Read, accessed code */
#define SEG_CODE_EXC        0x0C    /* Execute, confirming code */
#define SEG_CODE_EXCA       0x0D    /* Execute, confirming, accessed code */
#define SEG_CODE_EXRDC      0x0E    /* Execute/Read, confirming, accessed code */
#define SEG_CODE_EXRDCA     0x0F    /* Execute/Read, confirming, accessed code */

#define GDT_CODE_R0 SEG_DESCTYPE(1)     | SEG_PRESENT(1)        | \
                    SEG_AVAILABLE(0)    | SEG_LONG(0)           | \
                    SEG_SIZE(1)         | SEG_GRANULARITY(1)    | \
                    SEG_PRIVILEGE(0)    | SEG_CODE_EXRD

#define GDT_DATA_R0 SEG_DESCTYPE(1)     | SEG_PRESENT(1)        | \
                    SEG_AVAILABLE(0)    | SEG_LONG(0)           | \
                    SEG_SIZE(1)         | SEG_GRANULARITY(1)    | \
                    SEG_PRIVILEGE(0)    | SEG_DATA_RDWR

#define GDT_CODE_R3 SEG_DESCTYPE(1)     | SEG_PRESENT(1)        | \
                    SEG_AVAILABLE(0)    | SEG_LONG(0)           | \
                    SEG_SIZE(1)         | SEG_GRANULARITY(1)    | \
                    SEG_PRIVILEGE(3)    | SEG_CODE_EXRD

#define GDT_DATA_R3 SEG_DESCTYPE(1)     | SEG_PRESENT(1)        | \
                    SEG_AVAILABLE(0)    | SEG_LONG(0)           | \
                    SEG_SIZE(1)         | SEG_GRANULARITY(1)    | \
                    SEG_PRIVILEGE(3)    | SEG_DATA_RDWR

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flags);
void gdt_init();

#endif // _ARCH_INT_GDT_AXOCORE_H