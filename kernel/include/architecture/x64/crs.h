#ifndef EGOISTICOS_CRS_H
#define EGOISTICOS_CRS_H

#include <stdint.h>

typedef enum {
    CR_0,
    CR_2,
    CR_3,
    CR_4
} ControlRegisterType;

typedef union {
    uint64_t bits;
    struct {
        uint64_t PE: 1;
        uint64_t MP: 1;
        uint64_t EM: 1;
        uint64_t TS: 1;
        uint64_t ET: 1;
        uint64_t NE: 1;
        uint64_t reserved: 10;
        uint64_t WP: 1;
        uint64_t reserved2: 1;
        uint64_t AM: 1;
        uint64_t reserved3: 10;
        uint64_t NW: 1;
        uint64_t CD: 1;
        uint64_t PG: 1;
        uint64_t reserved4: 32;
    } CR0;
    struct {
        uint64_t pageFaultLinearAddress;
    } CR2;
    union {
        struct {
            uint64_t reserved: 3;
            uint64_t PWT: 1;
            uint64_t PCD: 1;
            uint64_t reserved2: 7;
            uint64_t PML4: 40;
            uint64_t reserved3: 12;
        } PCIDD;
        struct {
            uint64_t PCID: 12;
            uint64_t PML4: 40;
            uint64_t reserved3: 12;
        } PCIDE;
    } CR3;
    struct {
        uint64_t VME: 1;
        uint64_t PVI: 1;
        uint64_t TSD: 1;
        uint64_t DE: 1;
        uint64_t PSE: 1;
        uint64_t PAE: 1;
        uint64_t MCE: 1;
        uint64_t PGE: 1;
        uint64_t PCE: 1;
        uint64_t OSFXSR: 1;
        uint64_t OSXMMEEXCPT: 1;
        uint64_t UMIP: 1;
        uint64_t reserved: 1;
        uint64_t VMXE: 1;
        uint64_t SMXE: 1;
        uint64_t reserved2: 1;
        uint64_t FSGSBASE: 1;
        uint64_t PCIDE: 1;
        uint64_t OSXSAVE: 1;
        uint64_t reserved3: 1;
        uint64_t SMEP: 1;
        uint64_t SMAP: 1;
        uint64_t PKE: 1;
        uint64_t reserved4: 41;
    } CR4;
} CR;

CR read_cr(ControlRegisterType control_register_type);

void write_cr(ControlRegisterType control_register_type, CR value);

#endif //EGOISTICOS_CRS_H
