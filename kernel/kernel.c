#include "../common/include/framebuffer.h"
#include "../common/include/bootparms.h"
#include <stdint.h>
#include <stdbool.h>
#include <graphics.h>
#include <x64.h>
#include <keyboard.h>
#include <acpi.h>
#include <hpet.h>
#include <datetime.h>

_Noreturn void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();

    init_gdt();
    init_interrupt();
    init_acpi(bootParameter->acpi);
    init_apic(get_sdth("APIC"));
    init_hpet(get_sdth("HPET"));
    dump_hpet_info();
    bootParameter->time.nanosecond = 0;
    init_datetime(*(Time_t *) &bootParameter->time);
    init_keyboard();
    enable_cpu_interrupt();

    kernel_print_string_n("Egoistic OS Kernel ver.0.1");
    kernel_printf("Total memory: %uMB\n", bootParameter->memoryMap.totalMemory / 1024 / 1024);
    kernel_printf("Frame buffer base address: 0x%016lx\n", (uint64_t) bootParameter->frameBuffer.frameBufferBase);
    kernel_printf("ACPI address: 0x%016lx\n", (uint64_t) bootParameter->acpi);
    dump_acpi_info();
    dump_hpet_info();

    while (true)
        halt();
}
