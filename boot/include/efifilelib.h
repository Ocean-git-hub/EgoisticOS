#ifndef EGOISTICOS_EFIFILELIB_H
#define EGOISTICOS_EFIFILELIB_H

#include "efi/efistruct.h"

void get_root_file_protocol(EFI_FILE_PROTOCOL **root);

EFI_FILE_INFO *get_file_info(CHAR16 *file_name);

void read_file_to_address(uint64_t address, CHAR16 *file_name);

#endif //EGOISTICOS_EFIFILELIB_H
