#ifndef EGOISTICOS_DATETIME_H
#define EGOISTICOS_DATETIME_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t milliSecond;
} Time_t;

void init_datetime(Time_t _time);

Time_t get_time();

#endif //EGOISTICOS_DATETIME_H
