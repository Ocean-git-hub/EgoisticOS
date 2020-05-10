#include <timer.h>

#include <stdint.h>
#include <x64.h>
#include <hpet.h>

#define ONE_PER_MILLI 1000

#define MAX_TIMER0_OBSERVER 30

void *milli_second_observer[MAX_TIMER0_OBSERVER / 2];
void *second_observer[MAX_TIMER0_OBSERVER / 2];

uint8_t num_milli_second_observers;
uint8_t num_second_observers;

uint64_t second_count;

void do_timer0_interrupt() {
    second_count++;
    for (int i = 0; i < num_milli_second_observers; ++i)
        ((void (*)()) milli_second_observer[i])();
    if (second_count > ONE_PER_MILLI) {
        for (int i = 0; i < num_second_observers; ++i)
            ((void (*)()) second_observer[i])();
        second_count = 0;
    }
}

void timer0_interrupt() {
    enter_interrupt();
    do_timer0_interrupt();
    send_apic_eoi();
    leave_interrupt();
}

void init_timer() {
    uint8_t timer0_irq_no;
    set_timer(0, 1 * MILLI_PER_MICRO, 1, &timer0_irq_no, timer0_interrupt);
}

bool add_timer_observer(TimerType timer_type, void *observer) {
    switch (timer_type) {
        case MilliSecond:
            if (num_milli_second_observers >= MAX_TIMER0_OBSERVER)
                return false;
            milli_second_observer[num_milli_second_observers++] = observer;
            return true;
        case Second:
            if (num_second_observers >= MAX_TIMER0_OBSERVER)
                return false;
            second_observer[num_second_observers++] = observer;
            return true;
        default:
            return false;
    }
}
