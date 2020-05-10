#include "../common/include/framebuffer.h"
#include "../common/include/bootparms.h"
#include <stdint.h>
#include <stdbool.h>
#include <graphics.h>
#include <x64.h>
#include <keyboard.h>
#include <acpi.h>
#include <hpet.h>
#include <timer.h>
#include <datetime.h>

_Noreturn void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();

    kernel_print_string_n("Egoistic OS Kernel ver.0.1");
    kernel_printf("Total memory: %uMB\n", bootParameter->memoryMap.totalMemory / 1024 / 1024);
    kernel_printf("Frame buffer base address: 0x%016lx\n", (uint64_t) bootParameter->frameBuffer.frameBufferBase);
    kernel_printf("ACPI address: 0x%016lx\n", (uint64_t) bootParameter->acpi);

    init_gdt();
    init_interrupt();
    init_acpi(bootParameter->acpi);
    dump_acpi_info();
    init_apic(get_sdth("APIC"));
    init_hpet(get_sdth("HPET"));
    dump_hpet_info();
    init_timer();
    init_datetime();
    init_keyboard();
    enable_cpu_interrupt();

    while (true)
        halt();
}
