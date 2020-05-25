#include <memory/physicalmemory.h>

#include <stdint.h>
#include <graphics.h>
#include <keyboard.h>

#define MEMORY_DESCRIPTOR_TYPE_GENERAL_USE 7
#define PHYSICAL_BLOCK_SIZE (4 * 1024)
#define KERNEL_HEAP_PAGE_SIZE 256

typedef struct {
    uint64_t totalMemorySize;
    uint64_t totalMemoryPages;
    uint64_t numAllocatedPages;
    uint64_t numFreePages;
    uint64_t sizePerPage;
    uint64_t *memoryMap;
    uint64_t memoryMapSize;
} PhysicalMemoryInfo;

PhysicalMemoryInfo physical_memory_info;

void init_kernel_heap();

bool is_allocated_page_memory_map_bitmap(const uint64_t *memory_map, uint64_t page_no) {
    return (memory_map[page_no / 64] & 1ULL << (page_no % 64)) != 0;
}

void set_memory_map_bitmap(uint64_t *memory_map, bool is_allocated, uint64_t page_no) {
    if (is_allocated)
        memory_map[page_no / 64] |= 1ULL << (page_no % 64);
    else
        memory_map[page_no / 64] &= ~(1ULL << (page_no % 64));
}

void dump_physical_memory_info() {
    kernel_printf("PhysicalMemory Info: [totalMemorySize: %lu,totalMemoryPages: %lu,numAllocatedPages: %lu,"
                  "numFreePages: %lu,sizePerPage: %lu,memoryMap: 0x%lx,memoryMapSize: %lu]\n",
                  physical_memory_info.totalMemorySize, physical_memory_info.totalMemoryPages,
                  physical_memory_info.numAllocatedPages, physical_memory_info.numFreePages,
                  physical_memory_info.sizePerPage, physical_memory_info.memoryMap,
                  physical_memory_info.memoryMapSize);
}

void init_physical_memory(MemoryMap *memory_map, uint64_t kernel_end_address) {
    physical_memory_info.memoryMap = (uint64_t *) (kernel_end_address / 8 * 8 + 8);
    physical_memory_info.totalMemorySize = memory_map->totalMemory;
    physical_memory_info.sizePerPage = PHYSICAL_BLOCK_SIZE;
    physical_memory_info.numAllocatedPages = 0;
    physical_memory_info.numFreePages = 0;
    physical_memory_info.totalMemoryPages = 0;
    MemoryDescriptor *memory_descriptor = memory_map->memoryDescriptorBase;
    for (uint64_t i = 0; i < memory_map->memoryMapSize / memory_map->descriptorSize; ++i) {
        uint64_t page_no_base = memory_descriptor->physicalStart / physical_memory_info.sizePerPage;
        if (memory_descriptor->type == MEMORY_DESCRIPTOR_TYPE_GENERAL_USE) {
            physical_memory_info.numFreePages += memory_descriptor->numberOfPages;
            for (uint64_t j = 0; j < memory_descriptor->numberOfPages; ++j)
                set_memory_map_bitmap(physical_memory_info.memoryMap, false, page_no_base + j);
        } else {
            physical_memory_info.numAllocatedPages += memory_descriptor->numberOfPages;
            for (uint64_t j = 0; j < memory_descriptor->numberOfPages; ++j)
                set_memory_map_bitmap(physical_memory_info.memoryMap, true, page_no_base + j);
        }
        physical_memory_info.totalMemoryPages =
                physical_memory_info.numFreePages + physical_memory_info.numAllocatedPages;
        memory_descriptor = (MemoryDescriptor *) ((uint64_t) memory_descriptor + memory_map->descriptorSize);
    }
    physical_memory_info.memoryMapSize = physical_memory_info.totalMemoryPages / 8 + 8;
    for (uint64_t k = physical_memory_info.totalMemoryPages; k < physical_memory_info.totalMemoryPages * 8; ++k)
        set_memory_map_bitmap(physical_memory_info.memoryMap, true, k);
    //init_kernel_heap();
}

double get_physical_memory_usage() {
    return (double) physical_memory_info.numAllocatedPages / physical_memory_info.totalMemoryPages * 100;
}

uint64_t get_physical_free_memory_size() {
    return physical_memory_info.numFreePages * physical_memory_info.sizePerPage;
}

uint64_t get_physical_allocated_memory_size() {
    return physical_memory_info.numAllocatedPages * physical_memory_info.sizePerPage;
}

uint64_t get_physical_total_memory_size() {
    return physical_memory_info.totalMemorySize;
}

int64_t get_free_page_no_from_beginning(PhysicalMemoryInfo *memory_info) {
    for (uint64_t i = 0; i < memory_info->memoryMapSize / 8; ++i) {
        if (memory_info->memoryMap[i] != 0xffffffffffffffff) {
            if (((uint32_t *) memory_info->memoryMap)[i * 2] != 0xffffffff) {
                if (((uint16_t *) memory_info->memoryMap)[i * 4] != 0xffff) {
                    for (int j = 0; j < 16; ++j)
                        if (!is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j))
                            return i * 64 + j;
                } else {
                    for (int j = 16; j < 32; ++j)
                        if (!is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j))
                            return i * 64 + j;
                }
            } else {
                if (((uint16_t *) memory_info->memoryMap)[i * 4 + 2] != 0xffff) {
                    for (int j = 32; j < 48; ++j)
                        if (!is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j))
                            return i * 64 + j;
                } else {
                    for (int j = 48; j < 64; ++j)
                        if (!is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j))
                            return i * 64 + j;
                }
            }
        }
    }
    return -1;
}

int64_t get_free_page_no(PhysicalMemoryInfo *memory_info, uint64_t num_pages) {
    if (num_pages == 0)
        return -1;
    bool is_find;
    for (uint64_t i = 0; i < memory_info->memoryMapSize / 8; ++i) {
        if (memory_info->memoryMap[i] != 0xffffffffffffffff) {
            if (((uint32_t *) memory_info->memoryMap)[i * 2] != 0xffffffff) {
                if (((uint16_t *) memory_info->memoryMap)[i * 4] != 0xffff) {
                    for (int j = 0; j < 16; ++j) {
                        is_find = true;
                        for (uint64_t k = 0; k < num_pages; ++k)
                            if (is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j + k)) {
                                is_find = false;
                                j += k;
                                break;
                            }
                        if (is_find)
                            return i * 64 + j;
                    }
                } else {
                    for (int j = 16; j < 32; ++j) {
                        is_find = true;
                        for (uint64_t k = 0; k < num_pages; ++k)
                            if (is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j + k)) {
                                is_find = false;
                                j += k;
                                break;
                            }
                        if (is_find)
                            return i * 64 + j;
                    }
                }
            } else {
                if (((uint16_t *) memory_info->memoryMap)[i * 4 + 2] != 0xffff) {
                    for (int j = 32; j < 48; ++j) {
                        is_find = true;
                        for (uint64_t k = 0; k < num_pages; ++k)
                            if (is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j + k)) {
                                is_find = false;
                                j += k;
                                break;
                            }
                        if (is_find)
                            return i * 64 + j;
                    }
                } else {
                    for (int j = 48; j < 64; ++j) {
                        is_find = true;
                        for (uint64_t k = 0; k < num_pages; ++k)
                            if (is_allocated_page_memory_map_bitmap(memory_info->memoryMap, i * 64 + j + k)) {
                                is_find = false;
                                j += k;
                                break;
                            }
                        if (is_find)
                            return i * 64 + j;
                    }
                }
            }
        }
    }
    return -1;
}

void *allocate_physical_memory_page() {
    if (physical_memory_info.numFreePages == 0)
        return 0;
    int64_t free_page_no = get_free_page_no_from_beginning(&physical_memory_info);
    if (free_page_no == -1)
        return 0;
    physical_memory_info.numAllocatedPages++;
    physical_memory_info.numFreePages--;
    set_memory_map_bitmap(physical_memory_info.memoryMap, true, free_page_no);
    return (void *) (free_page_no * physical_memory_info.sizePerPage);
}

void free_physical_memory_page(void *pointer) {
    physical_memory_info.numAllocatedPages--;
    physical_memory_info.numFreePages++;
    set_memory_map_bitmap(physical_memory_info.memoryMap, false,
                          (uint64_t) pointer / physical_memory_info.sizePerPage);
}

void *allocate_physical_memory(uint64_t size) {
    if (physical_memory_info.numFreePages == 0 || size == 0)
        return 0;
    uint64_t num_pages = (size + physical_memory_info.sizePerPage - 1) / physical_memory_info.sizePerPage;
    int64_t free_page_no = get_free_page_no(&physical_memory_info, num_pages);
    if (free_page_no == -1)
        return 0;
    for (uint64_t i = 0; i < num_pages; ++i) {
        physical_memory_info.numAllocatedPages++;
        physical_memory_info.numFreePages--;
        set_memory_map_bitmap(physical_memory_info.memoryMap, true, free_page_no + i);
    }
    return (void *) (free_page_no * physical_memory_info.sizePerPage);
}

void free_physical_memory(void *pointer, uint64_t size) {
    uint64_t num_pages = (size + physical_memory_info.sizePerPage - 1) / physical_memory_info.sizePerPage;
    for (uint64_t i = 0; i < num_pages; ++i)
        free_physical_memory_page(pointer + physical_memory_info.sizePerPage * i);
}

void print_physical_memory_usage() {
    dump_physical_memory_info();
    kernel_printf("Memory: %luMB/%luMB(usage: %f%%)\n", get_physical_allocated_memory_size() / 1024 / 1024,
                  get_physical_total_memory_size() / 1024 / 1024, get_physical_memory_usage());
}

/*
 *   --- Kernel  MemoryMap ---
 *  |-------------------------|
 *  |         Kernel          |
 *  |-------------------------|
 *  |    PhysicalMemoryMap    |
 *  |-------------------------|
 *  |   KernelHeapMemoryMap   |
 *  |-------------------------|
 *  |          Heap           |
 *  |-------------------------|
 *  |          stack          |
 *  |-------------------------|
 */

PhysicalMemoryInfo kernel_heap_info;

void init_kernel_heap() {
    kernel_heap_info.memoryMap = (uint64_t *)
            (((uint64_t) physical_memory_info.memoryMap + physical_memory_info.memoryMapSize) / 8 * 8 + 8);
    kernel_heap_info.sizePerPage = KERNEL_HEAP_PAGE_SIZE;
    uint64_t rsp;
    __asm__ volatile ("mov %%rsp, %0":"=m"(rsp));
    kernel_heap_info.totalMemoryPages = (rsp - (uint64_t) kernel_heap_info.memoryMap) / kernel_heap_info.sizePerPage;
    kernel_heap_info.memoryMapSize = kernel_heap_info.totalMemoryPages / 8 + 8;
    for (uint64_t i = 0; i < kernel_heap_info.memoryMapSize * 8 / kernel_heap_info.sizePerPage + 1; ++i)
        set_memory_map_bitmap(kernel_heap_info.memoryMap, true, i);
    for (uint64_t i = kernel_heap_info.memoryMapSize * 8 / kernel_heap_info.sizePerPage + 1;
         i < kernel_heap_info.totalMemoryPages; ++i)
        set_memory_map_bitmap(kernel_heap_info.memoryMap, false, i);
}

void *allocate_kernel_heap_page() {
    int64_t free_page_no = get_free_page_no_from_beginning(&kernel_heap_info);
    if (free_page_no == -1)
        return 0;
    uint64_t rsp;
    __asm__ volatile ("mov %%rsp, %0":"=m"(rsp));
    uint64_t allocate_address = (uint64_t) (kernel_heap_info.memoryMap + free_page_no * kernel_heap_info.sizePerPage);
    if (allocate_address + kernel_heap_info.sizePerPage > rsp)
        return 0;
    set_memory_map_bitmap(kernel_heap_info.memoryMap, true, free_page_no);
    return (void *) allocate_address;
}

void free_kernel_heap_page(void *pointer) {
    set_memory_map_bitmap(kernel_heap_info.memoryMap, false,
                          (uint64_t) pointer / kernel_heap_info.sizePerPage - (uint64_t) kernel_heap_info.memoryMap);
}
