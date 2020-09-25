#ifndef EGOISTICOS_MEMORY_H
#define EGOISTICOS_MEMORY_H

#include <stdint.h>
#include "../../common/include/bootparms.h"

void init_memory(MemoryMap *memory_map, uint64_t kernel_end_address);

void *allocate_memory(uint64_t size);

void free_memory(void *pointer, uint64_t size);

void print_memory_info();

#endif //EGOISTICOS_MEMORY_H
