#ifndef EGOISTICOS_CPU_H
#define EGOISTICOS_CPU_H

#include <stdint.h>

void halt();

void enable_cpu_interrupt();

void read_cpuid(uint32_t input_eax, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

#endif //EGOISTICOS_CPU_H
