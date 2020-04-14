#ifndef EGOISTICOSBETA_EXECUTABLELIB_H
#define EGOISTICOSBETA_EXECUTABLELIB_H

#include <stdbool.h>
#include <stdint.h>
#include <windef.h>
#include <winnt.h>

#include "elf.h"

bool is_elf(uint64_t start_address);

uint64_t get_text_start_for_elf(Elf64_Ehdr *ehdr);

bool is_pe64(uint64_t start_address);

uint64_t get_text_start_for_pe64(IMAGE_DOS_HEADER *image_dos_header);

#endif //EGOISTICOSBETA_EXECUTABLELIB_H
