include_directories(include)

add_executable(Kernel kernel.c ../common/framebuffer.c graphics.c keyboard/keyboard.c keyboard/kbencoder.c
        keyboard/kbcontroller.c architecture/x64/interrupt.c architecture/x64/pic.c acpi.c architecture/x64/apic.c
        architecture/x64/msr.c architecture/x64/gdt.c architecture/x64/cpu.c architecture/x64/io.c hpet.c datetime.c timer.c rtc.c
        memory/memory.c memory/physicalmemory.c memory/virtualmemory.c shell.c string.c architecture/x64/crs.c)
target_compile_options(Kernel PUBLIC -mno-red-zone -Wall -Wextra -Wno-missing-field-initializers)
target_link_options(Kernel PUBLIC -T ../../kernel/kernel.ld -nostdlib -fno-common)
