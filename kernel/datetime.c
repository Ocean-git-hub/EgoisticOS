#include <datetime.h>

#include <rtc.h>
#include <timer.h>
#include <graphics.h>

DateTime time;

void increment_time() {
    time.second++;
    time.milliSecond = 0;
    if (time.second >= 60) {
        time.minute++;
        time.second = 0;
        if (time.minute >= 60) {
            time.hour++;
            time.minute = 0;
            if (time.hour >= 24) {
                get_rtc_datetime(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second);
            }
        }
    }
    printf_coordinate(get_screen_width() - 8 * 19, 0, "%04u/%02u/%02u %02u:%02u:%02u", time.year,
                      time.month, time.day, time.hour, time.minute, time.second);
}

void init_datetime() {
    get_rtc_datetime(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second);
    add_timer_observer(Second, increment_time);
}

DateTime get_datetime() {
    return time;
}
