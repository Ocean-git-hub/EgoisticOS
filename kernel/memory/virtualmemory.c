#include <memory/virtualmemory.h>

#include <x64.h>
#include <memory/physicalmemory.h>

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
}

void init_virtual_memory() {
    init_4level_paging();
}

