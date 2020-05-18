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
#include <memory.h>
#include <shell.h>

_Noreturn void start_kernel(BootParameter *bootParameter) {
    /*
     *       ~~~ Function Prologue ~~~
     *                            (rsp%16=0)
     *     push   %rbp            (rsp%16=8)
     *     mov    %rsp,%rbp
     *     sub    $--,%rsp        ($--%16=0;rsp%16=8)
     *     mov    %rdi,-0x8(%rbp)
     */
    __asm__ volatile ("sub $8, %rsp\n"); // make rsp 16-byte alignment

    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();

    init_gdt();
    init_interrupt();
    init_acpi(bootParameter->acpi);
    init_apic(get_sdth("APIC"));
    init_timer();
    init_datetime();
    init_keyboard();
    enable_cpu_interrupt();
    init_memory(&bootParameter->memoryMap, bootParameter->kernelEndAddress);

    kernel_print_string_n("Egoistic OS Kernel ver.0.1");
    kernel_printf("Frame buffer base address: 0x%016lx\n", bootParameter->frameBuffer.frameBufferBase);
    kernel_printf("ACPI address: 0x%016lx\n", bootParameter->acpi);
    kernel_printf("ControlRegisters 0234: 0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
                  read_cr(CR_0).bits, read_cr(CR_2).bits,
                  read_cr(CR_3).bits, read_cr(CR_4).bits);
    print_physical_memory_usage();
    dump_hpet_info();
    dump_acpi_info();

    uint32_t eax, ebx, ecx, edx;
    read_cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
    kernel_printf("a:%08x,b:%08x,c:%08x,d:%08x\n", eax, ebx, ecx, edx);
    shell();

    while (true)
        halt();
}
