#include <memory.h>

#include <memory/physicalmemory.h>
#include <memory/virtualmemory.h>

void init_memory(MemoryMap *memory_map, uint64_t kernel_end_address) {
    init_physical_memory(memory_map, kernel_end_address);
    //init_virtual_memory();
}

void *allocate_memory(uint64_t size) {
    return allocate_physical_memory(size);
}

void free_memory(void *pointer, uint64_t size) {
    free_physical_memory(pointer, size);
}

void print_memory_info() {
    print_physical_memory_usage();
}
