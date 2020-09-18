#ifndef EGOISTICOS_DATETIME_H
#define EGOISTICOS_DATETIME_H

#include <stdint.h>

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t milliSecond;
} DateTime;

void init_datetime();

void set_datetime(DateTime *dateTime);

void set_rtc_time();

DateTime get_datetime();

#endif //EGOISTICOS_DATETIME_H
