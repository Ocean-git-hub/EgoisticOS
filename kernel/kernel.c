#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <bootparms.h>
#include <framebuffer.h>

#include "include/graphics.h"

_Noreturn void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();

    kernel_print_string_n("\nEgoistic OS Kernel ver.0.1\n");
    kernel_printf("total memory: %uMB\n", bootParameter->memoryMap.totalMemory / 1024 / 1024);
    kernel_printf("Frame buffer base address: 0x%016lx\n", (uint64_t) bootParameter->frameBuffer.frameBufferBase);
    kernel_printf("ACPI address: 0x%016lx\n", (uint64_t) bootParameter->acpi);

    while (true)
            __asm__ volatile ("hlt\n");
}
