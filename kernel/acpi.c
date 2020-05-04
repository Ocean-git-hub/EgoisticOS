#include <acpi.h>

RSDPStructure *rsdp_descriptor;
XSDT *xsdt;
int64_t num_sdts;

void init_acpi(void *rsdp) {
    rsdp_descriptor = rsdp;
    xsdt = (XSDT *) rsdp_descriptor->XSDTAddress;
    num_sdts = (xsdt->header.length - sizeof(SDTHeader)) / sizeof(void *);
}

SDTHeader *get_sdth(const char *signature) {
    for (uint64_t i = 0; i < num_sdts; ++i)
        if (*((uint32_t *) xsdt->entry[i]->signature) == *((uint32_t *) signature))
            return xsdt->entry[i];
    return (void *)0;
}
