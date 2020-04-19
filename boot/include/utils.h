#ifndef EGOISTICOS_UTILS_H
#define EGOISTICOS_UTILS_H

#include <windef.h>
#include <winnt.h>

#include "efi/efidef.h"
#include "stdbool.h"
#include "elf.h"

void hex_dump(uint64_t value, uint8_t digit);

void dump_status(uint64_t status, bool linefeed);

void print_decimal(uint64_t value, uint8_t digit, bool linefeed);

void check_error(EFI_STATUS status, CHAR16 *message);

uint64_t get_largest_screen_mode();

void shutdown();

void print_boot_info();

#endif //EGOISTICOS_UTILS_H
