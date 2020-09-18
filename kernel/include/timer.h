#ifndef EGOISTICOS_TIMER_H
#define EGOISTICOS_TIMER_H

#include <stdbool.h>

typedef enum {
    TwentyMilliSecond,
    Second
} TimerType;

void init_timer();

bool add_timer_observer(TimerType timer_type, void *observer);

#endif //EGOISTICOS_TIMER_H
