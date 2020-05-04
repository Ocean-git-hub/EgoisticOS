#ifndef EGOISTICOS_INTERRUPT_H
#define EGOISTICOS_INTERRUPT_H

#include <stdint.h>

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
} IDT;

extern IDT idt[];

void init_interrupt();

void set_interrupt_descriptor(uint8_t index, void *handler, uint8_t present);

#endif //EGOISTICOS_INTERRUPT_H
