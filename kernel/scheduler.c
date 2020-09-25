#include <scheduler.h>

#include <memory.h>
#include <hpet.h>
#include <graphics.h>
#include <x64.h>

#define TASK_STACK_SIZE 4096
#define TIME_SLICE 20 * MILLI_PER_MICRO

Task root_task;
Task *current_task;

void scheduler() {
    enter_interrupt();
    __asm__ volatile ("mov %%rsp, %0":"=a"(current_task->rsp));
    current_task = current_task->nextTask;
    if (current_task == 0)
        current_task = &root_task;
    __asm__ volatile ("mov %0, %%rsp"::"a"(current_task->rsp));
    send_apic_eoi();
    leave_interrupt();
}

void init_scheduler() {
    current_task = &root_task;
//    uint8_t timer1_irq_no;
//    set_timer(1, TIME_SLICE, 1, &timer1_irq_no, scheduler);
}

void create_task(Task *task, void *function) {
    task->memoryPointer = allocate_memory(TASK_STACK_SIZE);
    task->nextTask = 0;
    Task *parent_task = &root_task;
    while (parent_task->nextTask != 0)
        parent_task = parent_task->nextTask;
    parent_task->nextTask = task;

    uint64_t *stack_pointer = (uint64_t *) (task->memoryPointer + TASK_STACK_SIZE) - 1;
    uint64_t base_stack_pointer = (uint64_t) stack_pointer;
    *--stack_pointer = 0x10;
    *--stack_pointer = base_stack_pointer;
    *--stack_pointer = 0x202;
    *--stack_pointer = 8;
    *--stack_pointer = (uint64_t) function;
    for (int i = 0; i < 15; ++i)
        *--stack_pointer = 0;
    task->rsp = (uint64_t) stack_pointer;
}
