#ifndef EGOISTICOS_RTC_H
#define EGOISTICOS_RTC_H

#include <stdint.h>

void get_rtc_datetime(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

#endif //EGOISTICOS_RTC_H
