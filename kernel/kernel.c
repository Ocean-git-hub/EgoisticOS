#include "../common/include/framebuffer.h"
#include "../common/include/bootparms.h"
#include <stdbool.h>
#include <x64.h>
#include <keyboard.h>
#include <acpi.h>
#include <timer.h>
#include <datetime.h>
#include <memory.h>
#include <shell.h>
#include <power.h>
#include <desktop.h>

_Noreturn void start_kernel(BootParameter *bootParameter) {
    /*
     *       ~~~ Function Prologue ~~~
     *                            (rsp%16=0)
     *     push   %rbp            (rsp%16=8)
     *     mov    %rsp,%rbp
     *     sub    $--,%rsp        ($--%16=0;rsp%16=8)
     *     mov    %rdi,-0x8(%rbp)
     */
    __asm__ volatile ("sub $8, %rsp"); // make rsp 16-byte alignment

    init_frame_buffer(&bootParameter->frameBuffer);
    init_gdt();
    init_interrupt();
    init_acpi(bootParameter->acpi);
    init_apic(get_sdth("APIC"));
    init_timer();
    init_datetime();
    init_keyboard();
    enable_cpu_interrupt();
    init_memory(&bootParameter->memoryMap, bootParameter->kernelEndAddress);
    init_power(bootParameter->reset_system);
    init_desktop();

    shell();

    while (true)
        halt();
}
