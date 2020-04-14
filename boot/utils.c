#include "include/utils.h"
#include <stdbool.h>
#include <stdint.h>

#include "include/efi/efidef.h"
#include "include/efisystab.h"
#include "include/efiio.h"

#define TIME_ZONE_TOKYO 9
#define MAX_DECIMAL_BUFFER 30

void hex_dump(uint64_t value, uint8_t digit) {
    uint8_t digit_tmp = digit;
    if (digit > 16)
        return;
    CHAR16 buf[17] = {0}, unit_val;
    for (int8_t i = 15; i >= 0 && digit-- != 0; i--) {
        unit_val = value & 0xfU;
        buf[(unsigned char) i] = unit_val + ((unit_val < 0xa) ? L'0' : L'A' - 0xa);
        value >>= 4U;
    }
    print_string(&buf[16 - digit_tmp]);
}

void dump_status(uint64_t status, bool linefeed) {
    print_string(L"0x");
    hex_dump(status, 16);
    if (linefeed)
        print_string_n(L"");
}

void print_decimal(uint64_t value, uint8_t digit, bool linefeed) {
    int8_t i = 0;
    uint8_t j = 0;
    CHAR16 buf_tmp[MAX_DECIMAL_BUFFER] = {0}, buf[MAX_DECIMAL_BUFFER] = {0};
    do
        buf_tmp[(uint8_t) i++] = L'0' + value % 10;
    while ((value /= 10) > 0 && i < MAX_DECIMAL_BUFFER - 1);
    if (digit >= i)
        while (digit - i != j)
            buf[j++] = '0';
    else
        i = digit;
    while (i >= 0)
        buf[j++] = buf_tmp[(uint8_t) --i];
    print_string(buf);
    if (linefeed)
        print_string_n(L"");
}

void check_error(EFI_STATUS status, CHAR16 *message) {
    if (status != EFI_SUCCESS) {
        print_string(L"Error: ");
        print_string(message);
        print_string(L": ");
        dump_status(status, true);
        while (true);
    }
}

uint64_t get_largest_screen_mode() {
    uint64_t columns = 0, rows = 0, mode = 0;
    for (uint64_t i = 0; i < system_table->ConOut->Mode->MaxMode; i++) {
        uint64_t c, r;
        EFI_STATUS Status = system_table->ConOut->EFI_TEXT_QUERY_MODE(system_table->ConOut, i, &c, &r);
        if (Status == EFI_SUCCESS) {
            if (c >= columns || r >= rows) {
                columns = c;
                rows = r;
                mode = i;
            }
        }
    }
    return mode;
}

void shutdown() {
    system_table->RuntimeServices->EFI_RESET_SYSTEM(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

void print_uefi_version() {
    switch (system_table->Hdr.Revision) {
        case EFI_2_70_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.7.0");
            break;
        case EFI_2_60_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.6.0");
            break;
        case EFI_2_40_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.4.0");
            break;
        case EFI_2_50_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.5.0");
            break;
        case EFI_2_31_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.3.1");
            break;
        case EFI_2_30_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.3.0");
            break;
        case EFI_2_20_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.2.0");
            break;
        case EFI_2_10_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.1.0");
            break;
        case EFI_2_00_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.2.0.0");
            break;
        case EFI_1_10_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.1.1.0");
            break;
        case EFI_1_02_SYSTEM_TABLE_REVISION:
            print_string(L"Ver.1.0.2");
            break;
    }
}

void get_time(EFI_TIME *time, uint8_t *hour, uint8_t *minute, uint16_t time_zone) {
    *hour = time->Hour;
    *minute = time->Minute;
    if (*hour > 24)
        *hour -= 24;
}

void get_datetime(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute) {
    EFI_TIME time;
    system_table->RuntimeServices->EFI_GET_TIME(&time, NULL);
    *year = time.Year;
    *month = time.Month;
    *day = time.Day;
    get_time(&time, hour, minute, time.TimeZone);
}

void print_datetime() {
    uint16_t year;
    uint8_t month, day, hour, minute;
    get_datetime(&year, &month, &day, &hour, &minute);
    print_decimal(year, 4, false);
    print_string(L"/");
    print_decimal(month, 2, false);
    print_string(L"/");
    print_decimal(day, 2, false);
    print_string(L" ");
    print_decimal(hour, 2, false);
    print_string(L":");
    print_decimal(minute, 2, false);
}

void print_boot_info() {
    print_string(L"Egoistic Boot Loader ver.");
    print_string(L"0.1");
    print_string(L" [FirmwareVendor: ");
    print_string(system_table->FirmwareVendor);
    print_string(L"(");
    print_uefi_version();
    print_string(L")]  (Now: ");
    print_datetime();
    print_string_n(L")");
}
