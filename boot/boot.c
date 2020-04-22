#include <stdbool.h>
#include <bootparms.h>
#include <framebuffer.h>

#include "include/efi.h"
#include "include/efisystab.h"
#include "include/efilocatedprotocol.h"
#include "include/efiio.h"
#include "include/efifilelib.h"
#include "include/executablelib.h"
#include "include/string.h"
#include "include/utils.h"
#include "include/ekf.h"

void get_memory_map(MemoryMap *memory_map) {
    memory_map->memoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR *memoryDescriptor = NULL;
    uint32_t descriptorVersion;
    EFI_STATUS status = system_table->BootServices->EFI_GET_MEMORY_MAP(&memory_map->memoryMapSize,
                                                                       memoryDescriptor,
                                                                       &memory_map->mapKey,
                                                                       &memory_map->descriptorSize,
                                                                       &descriptorVersion);
    while (status == EFI_BUFFER_TOO_SMALL || memoryDescriptor == NULL) {
        if (memoryDescriptor != NULL)
            system_table->BootServices->EFI_FREE_POOL(memoryDescriptor);
        system_table->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, memory_map->memoryMapSize,
                                                      (void **) &memoryDescriptor);
        status = system_table->BootServices->EFI_GET_MEMORY_MAP(&memory_map->memoryMapSize,
                                                                memoryDescriptor,
                                                                &memory_map->mapKey,
                                                                &memory_map->descriptorSize,
                                                                &descriptorVersion);
    }
    memory_map->memoryDescriptorBase = (MemoryDescriptor *) memoryDescriptor;
}

void set_boot_parameters(BootParameter *boot_parameter) {
    boot_parameter->frameBuffer.frameBufferBase = graphics_output_protocol->Mode->FrameBufferBase;
    boot_parameter->frameBuffer.frameBufferSize = graphics_output_protocol->Mode->FrameBufferSize;
    boot_parameter->frameBuffer.screenHeight = graphics_output_protocol->Mode->Info->VerticalResolution;
    boot_parameter->frameBuffer.screenWidth = graphics_output_protocol->Mode->Info->HorizontalResolution;
    EFI_GUID acpi_guid = EFI_ACPI_20_TABLE_GUID;
    boot_parameter->acpi = get_configuration_table(&acpi_guid);
    if (boot_parameter->acpi == NULL)
        print_string_n(L"[!] Couldn't find ACPI table.");
}

void exit_boot_services(MemoryMap *memory_map, void *image_handle) {
    EFI_STATUS status;
    do {
        get_memory_map(memory_map);
        status = system_table->BootServices->EFI_EXIT_BOOT_SERVICES(image_handle, memory_map->mapKey);
    } while (status != EFI_SUCCESS);
}

void show_memory_map() {
    MemoryMap memory_map;
    get_memory_map(&memory_map);
    MemoryDescriptor *memory_descriptor = memory_map.memoryDescriptorBase;
    for (uint32_t i = 0; i < memory_map.memoryMapSize / memory_map.descriptorSize; ++i) {
        print_decimal(i, 2, false);
        print_string(L": physicalStart: 0x");
        hex_dump(memory_descriptor->physicalStart, 16);
        print_string(L" numberOfPages: ");
        print_decimal(memory_descriptor->numberOfPages, 8, false);
        print_string(L" type: ");
        print_decimal(memory_descriptor->type, 2, false);
        print_string_n(L"");
        memory_descriptor = (MemoryDescriptor *) ((uint64_t) memory_descriptor + memory_map.descriptorSize);
        get_input_key();
    }
}

uint64_t get_total_memory_by_byte(MemoryMap *memory_map) {
    uint64_t total_memory = 0;
    MemoryDescriptor *memory_descriptor = memory_map->memoryDescriptorBase;
    for (uint32_t i = 0; i < memory_map->memoryMapSize / memory_map->descriptorSize; ++i) {
        total_memory += memory_descriptor->numberOfPages * 4 * 1024;
        memory_descriptor = (MemoryDescriptor *) ((uint64_t) memory_descriptor + memory_map->descriptorSize);
    }
    return total_memory;
}

EFI_STATUS efi_main(void *image_handle, EFI_SYSTEM_TABLE *_system_table) {
    init_systab(_system_table);
    init_located_protocol();

    system_table->ConOut->EFI_TEXT_SET_MODE(system_table->ConOut, get_largest_screen_mode());
    system_table->ConOut->EFI_TEXT_CLEAR_SCREEN(system_table->ConOut);
    print_boot_info();
    print_string_n(L"");

    show_memory_map();

    uint64_t kernel_address = 0x100000;
    read_file_to_address(kernel_address, L"kernel");

    BootParameter bootParameter;
    set_boot_parameters(&bootParameter);
    if (is_ek(kernel_address)) {
        print_string_n(L"This is an egoistic kernel file.");
        EKHeader *ek_header = (EKHeader *) kernel_address;
        system_table->BootServices->EFI_SET_MEM((void *) kernel_address + ek_header->bssStart, ek_header->bssSize, 0);
        exit_boot_services(&bootParameter.memoryMap, image_handle);
        bootParameter.memoryMap.totalMemory = get_total_memory_by_byte(&bootParameter.memoryMap);
        uint64_t kernel_arg1 = (uint64_t) &bootParameter;
        uint64_t kernel_start = (uint64_t) &ek_header->text;
        uint64_t stack_base = kernel_address + 1024 * 1024 * 8;
        __asm__ volatile ("mov %0, %%rdi\n"
                          "mov %1, %%rsp\n"
                          "jmp *%2\n"::"m"(kernel_arg1), "m"(stack_base), "m"(kernel_start));
        /*} else if (is_elf(kernel_address)) {
            print_string_n(L"This is an elf file.");

            Elf64_Shdr *bss_section = get_elf_section((Elf64_Ehdr *) kernel_address, ".bss");
            if (bss_section == NULL) {
                print_string_n(L"[!] Couldn't find .bss section.");
            } else {
                print_string_n(L"[*] Find bss");
                print_string(L".bss sh_size: ");
                print_decimal(bss_section->sh_size, 3, true);
                print_string(L".bss sh_offset: ");
                hex_dump(bss_section->sh_offset, 16);
                system_table->BootServices->EFI_SET_MEM((void *) (kernel_address + bss_section->sh_offset),
                                                        bss_section->sh_size, 0);
            }

            Elf64_Shdr *init_section = get_elf_section((Elf64_Ehdr *) kernel_address, ".init");
            if (init_section == NULL) {
                print_string_n(L"\r\n[!] Couldn't find .init section.");
            } else {
                ((void (*)()) (kernel_address + init_section->sh_offset))();
            }

            while (true);
            exit_boot_services(&bootParameter.memoryMap, image_handle);
            uint64_t kernel_arg1 = (uint64_t) &bootParameter;
            uint64_t kernel_start = get_text_start_for_elf((Elf64_Ehdr *) kernel_address);
            uint64_t stack_base = kernel_address + 1024 * 1024;
            __asm__ volatile ("mov %0, %%rdi\n"
                              "mov %1, %%rsp\n"
                              "jmp *%2\n"::"m"(kernel_arg1), "m"(stack_base), "m"(kernel_start));
        } else if (is_pe64(kernel_address)) {
            print_string_n(L"This is a pe32+ file.");
            uint64_t kernel_start = get_text_start_for_pe64((IMAGE_DOS_HEADER *) kernel_address);
            exit_boot_services(&bootParameter.memoryMap, image_handle);
            ((void (*)(BootParameter *)) kernel_start)(&bootParameter);*/
    } else {
        print_string_n(L"Sorry, this is an unsupported file.");
        print_string_n(L"Please press any key to shutdown...");
        get_input_key();
        shutdown();
    }
    return EFI_SUCCESS;
}
