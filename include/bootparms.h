#ifndef EGOISTICOS_BOOTPARMS_H
#define EGOISTICOS_BOOTPARMS_H

#include <stdint.h>

typedef struct {
    uint32_t type;
    uint64_t physicalStart;
    uint64_t virtualStart;
    uint64_t numberOfPages;
    uint64_t attribute;
} MemoryDescripter;

typedef struct {
    MemoryDescripter memoryDescriptor;
    uint64_t memoryMapSize, mapKey, descriptorSize;
    uint32_t descriptorVersion;
} MemoryMap;

typedef struct {
    uint64_t frameBufferBase, frameBufferSize, screenHeight, screenWidth;
} FrameBuffer;

typedef struct {
    MemoryMap memoryMap;
    FrameBuffer frameBuffer;
} BootParameter;

#endif //EGOISTICOS_BOOTPARMS_H
