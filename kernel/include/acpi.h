#ifndef EGOISTICOS_ACPI_H
#define EGOISTICOS_ACPI_H

#include <stdint.h>

typedef struct __attribute__ ((packed)) {
    int8_t signature[8];
    uint8_t checksum;
    int8_t OEMID[6];
    uint8_t revision;
    uint32_t RSDTAddress;
    uint32_t length;
    uint64_t XSDTAddress;
    uint8_t extendedChecksum;
    uint8_t reserved[3];
} RSDPStructure;

typedef struct __attribute__ ((packed)) {
    int8_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    int8_t OEMID[6];
    int8_t OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} SDTHeader;

typedef struct __attribute__ ((packed)) {
    SDTHeader header;
    SDTHeader *entry[];
} XSDT;

void init_acpi(void *rsdp);

SDTHeader *get_sdth(const char *signature);

#endif //EGOISTICOS_ACPI_H
