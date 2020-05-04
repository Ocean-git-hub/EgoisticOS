#include <architecture/msr.h>

#include <stdint.h>

uint64_t read_msr(uint32_t address) {
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
    return eax_edx.bits;
}

void write_msr(uint32_t address, uint64_t value) {
    union {
        uint64_t bits;
        struct {
            uint32_t eax;
            uint32_t edx;
        };
    } eax_edx;
    eax_edx.bits = value;
    __asm__ volatile ("mov %0, %%edx\n"
                      "mov %1, %%eax\n"
                      "mov %2, %%ecx\n"
                      "wrmsr\n"
    ::"m"(eax_edx.edx), "m"(eax_edx.eax), "m"(address));
}

