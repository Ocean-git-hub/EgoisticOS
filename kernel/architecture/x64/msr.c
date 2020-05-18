#include <architecture/x64/msr.h>

#include <stdint.h>

MSR read_msr(uint32_t address) {
    union {
        uint64_t bits;
        struct {
            uint32_t eax;
            uint32_t edx;
        };
    } eax_edx;
    __asm__ volatile ("mov %2, %%ecx\n"
                      "rdmsr\n"
    :"=d"(eax_edx.edx), "=a"(eax_edx.eax):"m"(address));
    return (MSR) eax_edx.bits;
}

void write_msr(uint32_t address, MSR value) {
    union {
        uint64_t bits;
        struct {
            uint32_t eax;
            uint32_t edx;
        };
    } eax_edx;
    eax_edx.bits = value.bits;
    __asm__ volatile ("mov %0, %%edx\n"
                      "mov %1, %%eax\n"
                      "mov %2, %%ecx\n"
                      "wrmsr\n"
    ::"m"(eax_edx.edx), "m"(eax_edx.eax), "m"(address));
}

