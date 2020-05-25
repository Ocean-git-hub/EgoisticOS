#include <memory/virtualmemory.h>

#include <x64.h>
#include <memory/physicalmemory.h>
#include <graphics.h>

#define NUM_PML4_TABLE_ENTRIES 1024
#define PML4_TABLE_SIZE 8 * NUM_PML4_TABLE_ENTRIES

typedef struct {
    uint64_t present: 1;
    uint64_t readWrite: 1;
    uint64_t userSupervisor: 1;
    uint64_t pageLevelWriteThrough: 1;
    uint64_t pageLevelCacheDisable: 1;
    uint64_t accessed: 1;
    uint64_t dirty: 1;
    uint64_t pageAttributeTable: 1;
    uint64_t global: 1;
    uint64_t ignored: 3;
    uint64_t physicalAddressPageAndReserved: 40;
    uint64_t ignored2: 7;
    uint64_t protectionKey: 4;
    uint64_t executeDisable: 1;
} Format4LevelPageTableEntry;

typedef struct {
    uint64_t present: 1;
    uint64_t readWrite: 1;
    uint64_t userSupervisor: 1;
    uint64_t pageLevelWriteThrough: 1;
    uint64_t pageLevelCacheDisable: 1;
    uint64_t accessed: 1;
    uint64_t ignored: 1;
    uint64_t pageSize: 1;
    uint64_t ignored2: 4;
    uint64_t physicalAddressTableAndReserved: 40;
    uint64_t ignored3: 11;
    uint64_t executeDisable: 1;
} Format4LevelPageDirectory;

typedef struct {
    uint64_t present: 1;
    uint64_t readWrite: 1;
    uint64_t userSupervisor: 1;
    uint64_t pageLevelWriteThrough: 1;
    uint64_t pageLevelCacheDisable: 1;
    uint64_t accessed: 1;
    uint64_t ignored: 1;
    uint64_t pageSize: 1;
    uint64_t ignored2: 4;
    uint64_t physicalAddressDirectoryAndReserved: 40;
    uint64_t ignored3: 11;
    uint64_t executeDisable: 1;
} Format4LevelPageDirectoryPointerTableEntry;

typedef struct {
    uint64_t present: 1;
    uint64_t readWrite: 1;
    uint64_t userSupervisor: 1;
    uint64_t pageLevelWriteThrough: 1;
    uint64_t pageLevelCacheDisable: 1;
    uint64_t accessed: 1;
    uint64_t ignored: 1;
    uint64_t pageSize: 1;
    uint64_t ignored2: 4;
    uint64_t physicalAddressPageDirectoryPointerTableAndReserved: 40;
    uint64_t ignored3: 11;
    uint64_t executeDisable: 1;
} Format4LevelPageMapLevel4Entry;

uint8_t max_physical_address;
void *pml4_table;
void *page_directory_pointer_table;

void init_pml4_table() {
    pml4_table = allocate_physical_memory(PML4_TABLE_SIZE);
    Format4LevelPageMapLevel4Entry *pml_table_p = pml4_table;
    for (int i = 0; i < NUM_PML4_TABLE_ENTRIES; ++i) {
        pml_table_p->present = 0;
        pml4_table++;
    }
}

void enable_4level_paging() {
    CR cr0 = read_cr(CR_0);
    CR cr4 = read_cr(CR_4);
    MSR ia32_efer = read_msr(MSR_REGISTER_ADDRESS_IA32_EFER);
    if (cr0.CR0.PG && cr4.CR4.PAE && ia32_efer.IA32EFERMSRLayout.LME)
        return;
    cr0.CR0.PG = cr4.CR4.PAE = ia32_efer.IA32EFERMSRLayout.LME = 0;
    write_cr(CR_0, cr0);
    write_cr(CR_4, cr4);
    write_msr(MSR_REGISTER_ADDRESS_IA32_EFER, ia32_efer);
    cr4.CR4.PAE = 1;
    write_cr(CR_4, cr4);
    ia32_efer.IA32EFERMSRLayout.LME = 1;
    write_msr(MSR_REGISTER_ADDRESS_IA32_EFER, ia32_efer);
    cr0.CR0.PG = 1;
    write_cr(CR_0, cr0);
}

void init_4level_paging() {
    enable_4level_paging();
    uint32_t dummy, eax;
    read_cpuid(0x80000008, &eax, &dummy, &dummy, &dummy);
    max_physical_address = eax & 0xffu;
}

void init_virtual_memory() {
    init_4level_paging();
}

