#ifndef EGOISTICOS_SCHEDULER_H
#define EGOISTICOS_SCHEDULER_H

#include <stdint.h>

typedef struct Task Task;

struct Task {
    uint64_t rsp;
    void *memoryPointer;
    Task *nextTask;
};

void init_scheduler();

void scheduler();

void create_task(Task *task, void *function);

#endif //EGOISTICOS_SCHEDULER_H
