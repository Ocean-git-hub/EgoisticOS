#include <int.h>
#include <io.h>
#include <bool.h>
#include "include/eficonst.h"
#include "include/efistruct.h"
#include "include/efiguid.h"

EFI_SYSTEM_TABLE *st;

#define PRINT_STRING(s) (st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, L ## s))

void print_string(CHAR16 *str) {
    st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, str);
}

void dump_status(uint64_t status, bool linefeed) {
    CHAR16 buf[17] = {0}, unit_val;
    for (int8_t i = 15; i >= 0; i--) {
        unit_val = status & 0xfU;
        buf[i] = unit_val + ((unit_val < 0xa) ? L'0' : L'A' - 0xa);
        status >>= 4U;
    }
    PRINT_STRING("0x");
    print_string(buf);
    if (linefeed)
        PRINT_STRING("\r\n");
}

#define MAX_DECIMAL_BUFFER 30

void print_decimal(uint64_t value, bool linefeed) {
    int8_t i = 0, j = 0;
    CHAR16 buf_tmp[MAX_DECIMAL_BUFFER] = {0}, buf[MAX_DECIMAL_BUFFER] = {0};
    do
        buf_tmp[i++] = L'0' + value % 10;
    while ((value /= 10) > 0 && i < MAX_DECIMAL_BUFFER - 1);
    while (i >= 0)
        buf[j++] = buf_tmp[--i];
    print_string(buf);
    if (linefeed)
        PRINT_STRING("\r\n");
}

void check_error(uint64_t status, CHAR16 *message) {
    if (status != EFI_SUCCESS) {
        print_string(message);
        PRINT_STRING(": ");
        dump_status(status, true);
    }
}

#define TIME_ZONE_TOKYO 9

void get_time(EFI_TIME *time, uint8_t *hour, uint8_t *minute) {
    *hour = time->Hour + TIME_ZONE_TOKYO;
    *minute = time->Minute;
    if (*hour > 24)
        *hour -= 24;
}

void get_datetime(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute) {
    EFI_TIME time;
    EFI_TIME_CAPABILITIES time_capabilities;
    st->RuntimeServices->EFI_GET_TIME(&time, &time_capabilities);
    *year = time.Year;
    *month = time.Month;
    *day = time.Day;
    get_time(&time, hour, minute);
}

void print_datetime() {
    uint16_t year;
    uint8_t month, day, hour, minute;
    get_datetime(&year, &month, &day, &hour, &minute);
    print_decimal(year, false);
    PRINT_STRING("/");
    print_decimal(month, false);
    PRINT_STRING("/");
    print_decimal(day, false);
    PRINT_STRING(" ");
    print_decimal(hour, false);
    PRINT_STRING(":");
    print_decimal(minute, false);
}

void efi_main(void *image_handle, EFI_SYSTEM_TABLE *system_table) {
    st = system_table;
    st->ConOut->EFI_TEXT_CLEAR_SCREEN(st->ConOut);
    st->ConOut->EFI_TEXT_ENABLE_CURSOR(st->ConOut, true);

    PRINT_STRING("Egoistic Boot Loader [ver 0.1]   (Now: ");
    print_datetime();
    PRINT_STRING("(UTC+0900))\r\n");


    EFI_MEMORY_DESCRIPTOR *mmap = NULL;
    UINTN mmap_size = 0;
    UINTN map_key, descriptor_size;
    UINT32 descriptor_version;
    EFI_STATUS status;
    do {
        status = st->BootServices->EFI_GET_MEMORY_MAP(&mmap_size, mmap, &map_key, &descriptor_size,
                                                      &descriptor_version);
        while (status == EFI_BUFFER_TOO_SMALL) {
            if (mmap != 0)
                st->BootServices->EFI_FREE_POOL(mmap);
            st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, mmap_size, (void **) &mmap);
            status = st->BootServices->EFI_GET_MEMORY_MAP(&mmap_size, mmap, &map_key, &descriptor_size,
                                                          &descriptor_version);
        }
        status = st->BootServices->EFI_EXIT_BOOT_SERVICES(image_handle, map_key);
    } while (status != EFI_SUCCESS);

    while (1);
}