#include "../common/include/framebuffer.h"
#include "../common/include/bootparms.h"
#include <stdint.h>
#include <stdbool.h>
#include <graphics.h>
#include <x64.h>
#include <keyboard.h>
#include <acpi.h>

_Noreturn void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();

    kernel_print_string_n("Egoistic OS Kernel ver.0.1");
    kernel_printf("Total memory: %uMB\n", bootParameter->memoryMap.totalMemory / 1024 / 1024);
    kernel_printf("Frame buffer base address: 0x%016lx\n", (uint64_t) bootParameter->frameBuffer.frameBufferBase);
    kernel_printf("ACPI address: 0x%016lx\n", (uint64_t) bootParameter->acpi);
    //kernel_printf("Keyboard self test: %s\n", keyboard_self_test() == true ? "Success" : "Failed");
    init_gdt();
    init_interrupt();
    init_acpi(bootParameter->acpi);
    SDTHeader *apic_header = get_sdth("APIC");
    init_apic(apic_header);
    init_keyboard();
    enable_cpu_interrupt();

    while (true)
        kernel_printf("%c", get_char());
}
