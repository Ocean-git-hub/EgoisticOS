#include <stdbool.h>
#include "../common/include/bootparms.h"

#include "include/efi.h"
#include "include/efisystab.h"
#include "include/efilocatedprotocol.h"
#include "include/efiio.h"
#include "include/efifilelib.h"
#include "include/executablelib.h"
#include "include/string.h"
#include "include/utils.h"
#include "include/ekf.h"

void get_memory_map(MemoryMap *memory_map);

void set_boot_parameters(BootParameter *boot_parameter);

void exit_boot_services(MemoryMap *memory_map, void *image_handle);

uint64_t get_total_memory_pages(MemoryMap *memory_map);

uint64_t get_free_page_address(uint64_t size, MemoryMap *memory_map);

void unsupported();

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
    print_string(L"[*] Kernel size: ");
    print_decimal(kernel_size, 10, false);
    print_string_n(L"byte");

    MemoryMap memory_map;
    get_memory_map(&memory_map);
    uint64_t kernel_address = get_free_page_address(kernel_size + stack_size, &memory_map);
    if (kernel_address == 0)
        unsupported();
    uint64_t kernel_file_end_address = kernel_address + kernel_size;
    print_string(L"[*] There are free pages to load kernel\n\r[*] Kernel Start: 0x");
    hex_dump(kernel_address, 16);
    print_string_n(L"");

    system_table->BootServices->EFI_ALLOCATE_PAGES(AllocateAddress, EfiLoaderCode,
                                                   (kernel_size + stack_size + 4095) / 4096, &kernel_address);
    system_table->BootServices->EFI_FREE_POOL(memory_map.memoryDescriptorBase);
    read_file_to_address(kernel_address, kernel_name);
    print_string_n(L"[*] Loading kernel successfully");

    BootParameter boot_parameter;
    set_boot_parameters(&boot_parameter);
    boot_parameter.kernelEndAddress = kernel_file_end_address;
    if (is_ek(kernel_address)) {
        print_string_n(L"[*] This file is an egoistic kernel file.");
        EKHeader *ek_header = (EKHeader *) kernel_address;
        system_table->BootServices->EFI_SET_MEM(kernel_address + ek_header->bssStart, ek_header->bssSize, 0);
        uint64_t kernel_arg1 = (uint64_t) &boot_parameter;
        uint64_t kernel_start = (uint64_t) &ek_header->text;
        uint64_t stack_base = (kernel_address + stack_size) / 16 * 16;
        print_string(L"[*] Kernel arg1: 0x");
        hex_dump(kernel_arg1, 16);
        print_string(L"\r\n[*] Stack base: 0x");
        hex_dump(stack_base, 16);

        exit_boot_services(&boot_parameter.memoryMap, image_handle);
        boot_parameter.memoryMap.totalMemory = get_total_memory_pages(&boot_parameter.memoryMap);
        __asm__ volatile ("mov %0, %%rdi\n"
                          "mov %1, %%rsp\n" // stack_base was set 16-byte alignment
                          "jmp *%2\n"
        ::"m"(kernel_arg1), "m"(stack_base), "m"(kernel_start));
    } else
        unsupported();
    return EFI_SUCCESS;
}

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

uint64_t get_total_memory_pages(MemoryMap *memory_map) {
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

void unsupported() {
    print_string_n(L"[!] Sorry, this is an unsupported file.");
    print_string_n(L"    Please press any key to shutdown...");
    get_input_key();
    shutdown();
}
