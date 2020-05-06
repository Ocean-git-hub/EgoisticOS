#ifndef EGOISTICOS_INTERRUPT_H
#define EGOISTICOS_INTERRUPT_H

#include <stdint.h>

#define enter_interrupt() \
    __asm__ volatile ("push %rax\n"\
                      "push %rbx\n"\
                      "push %rcx\n"\
                      "push %rdx\n"\
                      "push %rsp\n"\
                      "push %rbp\n"\
                      "push %rsi\n"\
                      "push %rdi\n"\
                      "push %r8\n"\
                      "push %r9\n"\
                      "push %r10\n"\
                      "push %r11\n"\
                      "push %r12\n"\
                      "push %r13\n"\
                      "push %r14\n"\
                      "push %r15\n")

#define leave_interrupt() \
    __asm__ volatile ("pop %r15\n"\
                      "pop %r14\n"\
                      "pop %r13\n"\
                      "pop %r12\n"\
                      "pop %r11\n"\
                      "pop %r10\n"\
                      "pop %r9\n"\
                      "pop %r8\n"\
                      "pop %rdi\n"\
                      "pop %rsi\n"\
                      "pop %rbp\n"\
                      "pop %rsp\n"\
                      "pop %rdx\n"\
                      "pop %rcx\n"\
                      "pop %rbx\n"\
                      "pop %rax\n"\
                      "add $8, %rsp\n"\
                      "iretq\n")

typedef struct __attribute__((packed)) {
    uint16_t offset1;
    uint16_t selector;
    uint8_t ist: 3;
    uint8_t zero: 5;
    uint8_t type: 4;
    uint8_t zero2: 1;
    uint8_t dpl: 2;
    uint8_t present: 1;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t reserved;
} IDT;

extern IDT idt[];

void init_interrupt();

void set_interrupt_descriptor(uint8_t index, void *handler, uint8_t present);

#endif //EGOISTICOS_INTERRUPT_H
