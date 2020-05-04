include_directories(include)

add_executable(Kernel kernel.c ../common/framebuffer.c graphics.c keyboard/keyboard.c keyboard/kbencoder.c
        keyboard/kbcontroller.c architecture/interrupt.c architecture/pic.c acpi.c architecture/apic.c
        architecture/msr.c architecture/gdt.c architecture/cpu.c architecture/io.c)
target_link_options(Kernel PUBLIC -T ../../kernel/kernel.ld -e start_kernel
        -Wall -Wextra -Wno-missing-field-initializers -nostdlib -fno-common)
