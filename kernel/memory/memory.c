#include <memory.h>

void init_memory(MemoryMap *memory_map, uint64_t kernel_end_address) {
    init_physical_memory(memory_map, kernel_end_address);
    //init_virtual_memory();
}

