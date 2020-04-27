#include "include/handler.h"
#include "include/keyboard.h"

#include "include/graphics.h"

static inline void enter_interrupt() {
    __asm__ volatile ("push %rax\n"
                      "push %rbx\n"
                      "push %rcx\n"
                      "push %rdx\n"
                      "push %rbp\n"
                      "push %rsp\n"
                      "push %rsi\n"
                      "push %rdi\n");
}

static inline void exit_interrupt() {
    __asm__ volatile ("pop %rdi\n"
                      "pop %rsi\n"
                      "pop %rsp\n"
                      "pop %rbp\n"
                      "pop %rdx\n"
                      "pop %rcx\n"
                      "pop %rbx\n"
                      "pop %rax\n"
                      "iretq\n");
}

//void keyboard_interrupt() {
//    __asm__ volatile ("push %rax\n"
//                      "push %rbx\n"
//                      "push %rcx\n"
//                      "push %rdx\n"
//                      "push %rbp\n"
//                      "push %rsi\n"
//                      "push %rdi\n");
//    do_keyboard_interrupt();
//    __asm__ volatile ("pop %rdi\n"
//                      "pop %rsi\n"
//                      "pop %rbp\n"
//                      "pop %rdx\n"
//                      "pop %rcx\n"
//                      "pop %rbx\n"
//                      "pop %rax\n"
//                      "iretq\n");
//}
