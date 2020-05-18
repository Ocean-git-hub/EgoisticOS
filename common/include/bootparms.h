#ifndef EGOISTICOS_BOOTPARMS_H
#define EGOISTICOS_BOOTPARMS_H

#include <stdint.h>
#include "framebuffer.h"

typedef struct {
    uint32_t type;
    uint64_t physicalStart;
    uint64_t virtualStart;
    uint64_t numberOfPages;
    uint64_t attribute;
} MemoryDescriptor;

typedef struct {
    MemoryDescriptor *memoryDescriptorBase;
    uint64_t memoryMapSize, mapKey, descriptorSize, totalMemory;
} MemoryMap;

typedef struct __attribute__((packed)) {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t nanosecond;
} Time;

typedef struct {
    uint64_t kernelEndAddress;
    MemoryMap memoryMap;
    FrameBuffer frameBuffer;
    void *acpi;
    Time time;
} BootParameter;

#endif //EGOISTICOS_BOOTPARMS_H
