#ifndef EGOISTICOS_EXECUTABLELIB_H
#define EGOISTICOS_EXECUTABLELIB_H

#include <stdbool.h>
#include <stdint.h>
#include <windef.h>
#include <winnt.h>

#include "elf.h"

bool is_elf(uint64_t start_address);

uint64_t get_text_start_for_elf(Elf64_Ehdr *ehdr);

bool is_pe64(uint64_t start_address);

uint64_t get_text_start_for_pe64(IMAGE_DOS_HEADER *image_dos_header);

void print_elf_section(Elf64_Ehdr *ehdr);

Elf64_Shdr *get_elf_section(Elf64_Ehdr *ehdr, int8_t *name);

bool is_ek(uint64_t start_address);

#endif //EGOISTICOS_EXECUTABLELIB_H
