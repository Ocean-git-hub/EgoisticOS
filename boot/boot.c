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
    CHAR16 buf[20];
    for (uint32_t i = 0; i < memory_map.memoryMapSize / memory_map.descriptorSize; ++i) {
        print_decimal(i, 2, false);
        print_string(L": physical: 0x");
        hex_dump(memory_descriptor->physicalStart, 16);
        print_string(L" pages: ");
        print_decimal(memory_descriptor->numberOfPages, 8, false);
        print_string(L" type: ");
        print_decimal(memory_descriptor->type, 2, false);
        print_string(L" ");
        get_memory_type_name(memory_descriptor->type, buf);
        print_string_n(buf);

        memory_descriptor = (MemoryDescriptor *) ((uint64_t) memory_descriptor + memory_map.descriptorSize);
        get_input_key();
    }
    system_table->BootServices->EFI_FREE_POOL(memory_map.memoryDescriptorBase);
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

uint64_t get_free_page_address(uint64_t size, MemoryMap *memory_map) {
    MemoryDescriptor *memory_descriptor = memory_map->memoryDescriptorBase;
    for (uint32_t i = 0; i < memory_map->memoryMapSize / memory_map->descriptorSize; ++i) {
        if (memory_descriptor->type == EfiConventionalMemory && memory_descriptor->numberOfPages * 4 * 1024 > size)
            return memory_descriptor->physicalStart;
        memory_descriptor = (MemoryDescriptor *) ((uint64_t) memory_descriptor + memory_map->descriptorSize);
    }
    return 0;
}

void unsupported(){
    print_string_n(L"Sorry, this is an unsupported file.");
    print_string_n(L"Please press any key to shutdown...");
    get_input_key();
    shutdown();
}

EFI_STATUS efi_main(void *image_handle, EFI_SYSTEM_TABLE *_system_table) {
    init_systab(_system_table);
    init_located_protocol();

    system_table->ConOut->EFI_TEXT_SET_MODE(system_table->ConOut, get_largest_screen_mode());
    system_table->ConOut->EFI_TEXT_CLEAR_SCREEN(system_table->ConOut);
    print_boot_info();
    print_string_n(L"");

    CHAR16 *kernel_name = L"kernel";
    uint64_t stack_size = 1024 * 1024 * 8;
    EFI_FILE_INFO *kernel_info = get_file_info(kernel_name);
    uint64_t kernel_size = kernel_info->FileSize;
    system_table->BootServices->EFI_FREE_POOL(kernel_info);
    print_string(L"Kernel size: ");
    print_decimal(kernel_size / 1024, 2, false);
    print_string_n(L"KB");

    MemoryMap memory_map;
    get_memory_map(&memory_map);
    uint64_t kernel_address = get_free_page_address(kernel_size + stack_size, &memory_map);
    if (kernel_address == 0)
        unsupported();
    system_table->BootServices->EFI_FREE_POOL(memory_map.memoryDescriptorBase);
    read_file_to_address(kernel_address, kernel_name);

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
        uint64_t stack_base = kernel_address + stack_size;
        __asm__ volatile ("mov %0, %%rdi\n"
                          "mov %1, %%rsp\n"
                          "jmp *%2\n"
        ::"m"(kernel_arg1), "m"(stack_base), "m"(kernel_start));
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
    } else
        unsupported();
    return EFI_SUCCESS;
}
