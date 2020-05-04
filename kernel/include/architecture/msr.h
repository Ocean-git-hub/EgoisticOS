#ifndef EGOISTICOS_MSR_H
#define EGOISTICOS_MSR_H

#include <stdint.h>

#define  MSR_IA32_APIC_BASE 0x1b

typedef union {
    uint64_t bits;
    struct {
        uint64_t reserved:8;
        uint64_t BSP: 1;
        uint64_t reserved2: 2;
        uint64_t xAPICGlobalEnable: 1;
        uint64_t APICBaseAddress:24;
        uint64_t reserved3:28;
    };
} IA32APICBaseMSRLayout;

uint64_t read_msr(uint32_t address);

void write_msr(uint32_t address, uint64_t value);

#endif //EGOISTICOS_MSR_H
