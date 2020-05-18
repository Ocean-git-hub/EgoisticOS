#ifndef EGOISTICOS_MSR_H
#define EGOISTICOS_MSR_H

#include <stdint.h>

#define MSR_REGISTER_ADDRESS_IA32_APIC_BASE 0x1b
#define MSR_REGISTER_ADDRESS_IA32_EFER 0xc0000080

typedef union {
    uint64_t bits;
    struct {
        uint64_t reserved: 8;
        uint64_t BSP: 1;
        uint64_t reserved2: 2;
        uint64_t xAPICGlobalEnable: 1;
        uint64_t APICBaseAddress: 24;
        uint64_t reserved3: 28;
    } IA32APICBaseMSRLayout;
    struct {
        uint64_t SCE: 1;
        uint64_t reserved: 7;
        uint64_t LME: 1;
        uint64_t reserved2: 1;
        uint64_t LMA: 1;
        uint64_t NXE: 1;
        uint64_t reserved3: 52;
    } IA32EFERMSRLayout;
} MSR;

MSR read_msr(uint32_t address);

void write_msr(uint32_t address, MSR value);

#endif //EGOISTICOS_MSR_H
