#include <acpi.h>

#include <graphics.h>

RSDPStructure *rsdp;
XSDT *xsdt;
uint64_t num_sdts;

void init_acpi(void *_rsdp) {
    rsdp = _rsdp;
    xsdt = (XSDT *) rsdp->XSDTAddress;
    num_sdts = (xsdt->header.length - sizeof(SDTHeader)) / sizeof(void *);
}

void dump_acpi_info() {
    kernel_printf("ACPI Info: [num_SDT=%ld,STDs[", num_sdts);
    for (uint64_t i = 0; i < num_sdts; ++i) {
        for (uint8_t j = 0; j < 4; ++j)
            kernel_print_char(xsdt->entry[i]->signature[j]);
        kernel_print_char(' ');
    }
    kernel_printf("]]\n");
}

SDTHeader *get_sdth(const char *signature) {
    for (uint64_t i = 0; i < num_sdts; ++i)
        if (*((uint32_t *) xsdt->entry[i]->signature) == *((uint32_t *) signature))
            return xsdt->entry[i];
    return (void *) 0;
}
