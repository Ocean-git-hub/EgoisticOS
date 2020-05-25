#ifndef EGOISTICOS_PHYSICALMEMORY_H
#define EGOISTICOS_PHYSICALMEMORY_H

#include <stdint.h>
#include "../../common/include/bootparms.h"

void init_physical_memory(MemoryMap *memory_map, uint64_t kernel_end_address);

double get_physical_memory_usage();

uint64_t get_physical_free_memory_size();

uint64_t get_physical_allocated_memory_size();

uint64_t get_physical_total_memory_size();

void *allocate_physical_memory_page();

void free_physical_memory_page(void *pointer);

void print_physical_memory_usage();

void *allocate_kernel_heap_page();

void free_kernel_heap_page(void *pointer);

void *allocate_physical_memory(uint64_t size);

void free_physical_memory(void *pointer, uint64_t size);

#endif //EGOISTICOS_PHYSICALMEMORY_H
