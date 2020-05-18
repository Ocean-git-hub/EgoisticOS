#ifndef EGOISTICOS_APIC_H
#define EGOISTICOS_APIC_H

#include <stdint.h>
#include <stdbool.h>

#include <acpi.h>
#include <architecture/x64/pic.h>

#define INTERRUPT_NUMBER_BASE 0x20

typedef struct __attribute__ ((packed)) {
    SDTHeader header;
    uint32_t localInterruptControllerAddress;
    union {
        uint32_t flags;
        struct {
            uint32_t PCAT_COMPAT: 1;
            uint32_t reserved: 31;
        } Flags;
    };
    uint8_t interruptControllerStructure[];
} MADT;

bool is_support_x2apic();

void set_apic_irq(bool is_enable, uint8_t irq_no);

void send_apic_eoi();

void init_apic(SDTHeader *apic_header);

#endif //EGOISTICOS_APIC_H
