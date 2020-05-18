#ifndef EGOISTICOS_CPU_H
#define EGOISTICOS_CPU_H

#include <stdint.h>

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rdi, rsi;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint16_t cs, ss, ds;
    uint64_t rbp, rsp, rip;
} CpuContext;

extern CpuContext context;

void halt();

void enable_cpu_interrupt();

void read_cpuid(uint32_t input_eax, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

#endif //EGOISTICOS_CPU_H
