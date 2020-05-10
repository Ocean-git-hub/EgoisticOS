#include <rtc.h>

#include <stdint.h>
#include <x64.h>

#define RTC_INDEX_SECONDS 0
#define RTC_INDEX_MINUTES 0x2
#define RTC_INDEX_HOURS 0x4
#define RTC_INDEX_DAY_OF_WEEK 0x6
#define RTC_INDEX_DAY_OF_MONTH 0x7
#define RTC_INDEX_MONTH 0x8
#define RTC_INDEX_YEAR 0x9

void get_rtc_datetime(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second) {
    union {
        struct {
            uint8_t low: 4;
            uint8_t high: 4;
        };
        uint8_t bits;
    } bcd;
    io_write_b(IO_PORT_RTC_INDEX_REGISTER, RTC_INDEX_SECONDS);
    bcd.bits = io_read_b(IO_PORT_RTC_TARGET_REGISTER);
    *second = bcd.low + bcd.high * 10;
    io_write_b(IO_PORT_RTC_INDEX_REGISTER, RTC_INDEX_MINUTES);
    bcd.bits = io_read_b(IO_PORT_RTC_TARGET_REGISTER);
    *minute = bcd.low + bcd.high * 10;
    io_write_b(IO_PORT_RTC_INDEX_REGISTER, RTC_INDEX_HOURS);
    bcd.bits = io_read_b(IO_PORT_RTC_TARGET_REGISTER);
    *hour = bcd.low + bcd.high * 10;
    io_write_b(IO_PORT_RTC_INDEX_REGISTER, RTC_INDEX_DAY_OF_MONTH);
    bcd.bits = io_read_b(IO_PORT_RTC_TARGET_REGISTER);
    *day = bcd.low + bcd.high * 10;
    io_write_b(IO_PORT_RTC_INDEX_REGISTER, RTC_INDEX_MONTH);
    bcd.bits = io_read_b(IO_PORT_RTC_TARGET_REGISTER);
    *month = bcd.low + bcd.high * 10;
    io_write_b(IO_PORT_RTC_INDEX_REGISTER, RTC_INDEX_YEAR);
    bcd.bits = io_read_b(IO_PORT_RTC_TARGET_REGISTER);
    *year = bcd.low + bcd.high * 10 + 2000;
}
