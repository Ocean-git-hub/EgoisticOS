#ifndef EGOISTICOSBETA_EFILOCATEDPROTOCOL_H
#define EGOISTICOSBETA_EFILOCATEDPROTOCOL_H

#include "efi/efistruct.h"

extern EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *simple_file_system_protocol;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics_output_protocol;

void init_located_protocol();

#endif //EGOISTICOSBETA_EFILOCATEDPROTOCOL_H
