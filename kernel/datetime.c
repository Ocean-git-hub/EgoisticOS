#include <datetime.h>

#include <hpet.h>
#include <x64.h>
#include <graphics.h>

#define ONE_PER_MILLI 1000

Time_t time;

uint8_t timer0_irq_no;

void do_timer0_interrupt() {
    time.milliSecond++;
    if (time.milliSecond >= ONE_PER_MILLI) {
        time.second++;
        time.milliSecond = 0;
        if (time.second >= 60) {
            time.minute++;
            time.second = 0;
            if (time.minute >= 60) {
                time.hour++;
                time.minute = 0;
                if (time.hour > 24) {
                    time.day++;
                    time.hour = 0;
                }
            }
        }
        printf_coordinate(get_screen_width() - 8 * 19, 0, "%04u", time.year);
        printf_coordinate(get_screen_width() - 8 * 15, 0, "/%02u", time.month);
        printf_coordinate(get_screen_width() - 8 * 12, 0, "/%02u", time.day);
        printf_coordinate(get_screen_width() - 8 * 9, 0, " %02u", time.hour);
        printf_coordinate(get_screen_width() - 8 * 6, 0, ":%02u", time.minute);
        printf_coordinate(get_screen_width() - 8 * 3, 0, ":%02u", time.second);
    }
}

void timer0_interrupt() {
    enter_interrupt();
    do_timer0_interrupt();
    send_apic_eoi();
    leave_interrupt();
}

void init_datetime(Time_t _time) {
    time = _time;
    set_timer(0, 1 * MILLI_PER_MICRO, true, &timer0_irq_no, timer0_interrupt);
}

Time_t get_time() {
    return time;
}
