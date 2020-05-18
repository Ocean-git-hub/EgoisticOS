#ifndef EGOISTICOS_MEMORY_H
#define EGOISTICOS_MEMORY_H

#include <stdint.h>
#include <memory/physicalmemory.h>
#include <memory/virtualmemory.h>
#include "../../common/include/bootparms.h"

void init_memory(MemoryMap *memory_map, uint64_t kernel_end_address);

#endif //EGOISTICOS_MEMORY_H
