#ifndef EGOISTICOS_X64_H
#define EGOISTICOS_X64_H

#include <stdint.h>

#define SEGMENT_SELECTOR_CODE 0x8
#define SEGMENT_SELECTOR_DATA 0x10

typedef struct __attribute__((packed)) {
    uint16_t offset1;
    uint16_t selector;
    uint8_t ist: 3;
    uint8_t zero: 5;
    uint8_t type: 4;
    uint8_t zero2: 1;
    uint8_t dpl: 2;
    uint8_t present: 1;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t reserved;
} IDTDescriptor;

void halt();

uint8_t io_read_b(uint16_t address);

void io_write_b(uint16_t address, uint8_t data);

void init_gdt();

void enable_cpu_interrupt();

#endif //EGOISTICOS_X64_H
