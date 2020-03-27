#include <int.h>
#include "include/eficonst.h"
#include "include/efistruct.h"
#include "include/efiguid.h"

void efi_main(void *image_handle __attribute__ ((unused)), EFI_SYSTEM_TABLE *system_table) {
    system_table->ConOut->OUT_PUT_STRING(system_table->ConOut, L"Hello UEFI!\n");
    while (1);
}