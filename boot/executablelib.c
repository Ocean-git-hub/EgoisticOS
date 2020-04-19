#include "include/executablelib.h"

#include <windef.h>
#include <winnt.h>
#include <stdbool.h>

#include "include/elf.h"
#include "include/utils.h"
#include "include/efiio.h"
#include "include/string.h"
#include "include/ekf.h"

bool is_elf(uint64_t start_address) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) start_address;
    return ehdr->e_ident[0] == 0x7f && ehdr->e_ident[1] == 'E' && ehdr->e_ident[2] == 'L' &&
           ehdr->e_ident[3] == 'F';
}

uint64_t get_text_start_for_elf(Elf64_Ehdr *ehdr) {
    return (uint64_t) ehdr + (uint64_t) ehdr->e_entry;
}

bool is_pe64(uint64_t start_address) {
    IMAGE_DOS_HEADER *image_dos_header = (IMAGE_DOS_HEADER *) start_address;
    if (image_dos_header->e_magic != 0x5a4d && image_dos_header->e_magic != 0x4d5a) return false;
    IMAGE_NT_HEADERS *image_nt_headers = (IMAGE_NT_HEADERS * )((uint64_t) image_dos_header->e_lfanew +
                                                               start_address);
    IMAGE_OPTIONAL_HEADER *image_optional_header = &image_nt_headers->OptionalHeader;
    return image_optional_header->Magic == 0x20b;
}

uint64_t get_text_start_for_pe64(IMAGE_DOS_HEADER *image_dos_header) {
    IMAGE_NT_HEADERS *image_nt_headers = (IMAGE_NT_HEADERS *) ((uint64_t) image_dos_header->e_lfanew +
                                                               (uint64_t) image_dos_header);
    IMAGE_OPTIONAL_HEADER *image_optional_header = &image_nt_headers->OptionalHeader;
    return (uint64_t) image_dos_header + image_optional_header->SizeOfHeaders;
}

void print_elf_section(Elf64_Ehdr *ehdr) {
    int8_t *p = (int8_t *) ehdr;
    Elf64_Shdr *shdr = (Elf64_Shdr *) (p + ehdr->e_shoff);
    Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
    int8_t *sh_strtab_p = p + sh_strtab->sh_offset;
    for (uint16_t i = 0; i < ehdr->e_shnum; i++) {
        print_decimal(i, 2, false);
        print_string(L": ");
        print_decimal(shdr[i].sh_name, 4, false);
        print_string(L" '");
        hex_dump(*((int64_t * )(sh_strtab_p + shdr[i].sh_name)), 16);
        print_string_n(L"'");
    }
}

Elf64_Shdr *get_elf_section(Elf64_Ehdr *ehdr, int8_t *name) {
    int8_t *p = (int8_t *) ehdr;
    Elf64_Shdr *shdr = (Elf64_Shdr *) (p + ehdr->e_shoff);
    Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
    int8_t *sh_strtab_p = p + sh_strtab->sh_offset;
    for (uint16_t i = 0; i < ehdr->e_shnum; i++)
        if (str8_cmp(sh_strtab_p + shdr[i].sh_name, name) == 0)
            return &shdr[i];
    return NULL;
}

bool is_ek(uint64_t start_address) {
    EKHeader *ek_header = (EKHeader *) start_address;
    return ek_header->magic == 0x454B || ek_header->magic == 0x4B45;
}
