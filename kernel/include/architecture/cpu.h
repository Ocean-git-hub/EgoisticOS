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

#endif //EGOISTICOS_CPU_H
