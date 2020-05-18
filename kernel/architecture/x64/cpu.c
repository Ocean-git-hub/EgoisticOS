#include <architecture/x64/cpu.h>

CpuContext context;

inline __attribute__((always_inline)) void halt() {
    __asm__ volatile ("hlt\n");
}

inline __attribute__((always_inline)) void enable_cpu_interrupt() {
    __asm__ volatile ("sti\n");
}

void read_cpuid(uint32_t input_eax, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    __asm__ volatile ("mov %4, %%eax\n"
                      "cpuid"
    :"=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
    :"m"(input_eax));
}
