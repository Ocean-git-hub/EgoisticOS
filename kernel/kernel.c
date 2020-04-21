#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <bootparms.h>
#include <framebuffer.h>

#include "include/graphics.h"

_Noreturn void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();
    kernel_print_string_n("Start kernel.");
    kernel_printf("ACPI Address: 0x%016lx\n", (uint64_t) bootParameter->acpi);
    kernel_printf("%c %s %d %u %x %ld %lu %lx %08x\n",
                  'A', "hello", -10, 10, 0xf7, -105151511549078510, 105151511549078510, 0xabcdefabcdef1234, 0xff);

    while (true)
            __asm__ volatile ("hlt\n");
}
