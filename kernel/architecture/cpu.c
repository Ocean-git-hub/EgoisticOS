#include <architecture/cpu.h>

CpuContext context;

void halt() {
    __asm__ volatile ("hlt\n");
}

void enable_cpu_interrupt() {
    __asm__ volatile ("sti\n");
}
