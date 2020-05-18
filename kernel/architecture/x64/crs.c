#include <architecture/x64/crs.h>

#include <stdint.h>

CR read_cr(ControlRegisterType control_register_type) {
    CR cr = {};
    switch (control_register_type) {
        case CR_0:
            __asm__ volatile ("mov %%cr0, %0":"=r"(cr));
            break;
        case CR_2:
            __asm__ volatile ("mov %%cr2, %0":"=r"(cr));
            break;
        case CR_3:
            __asm__ volatile ("mov %%cr3, %0":"=r"(cr));
            break;
        case CR_4:
            __asm__ volatile ("mov %%cr4, %0":"=r"(cr));
            break;
        default:
            break;
    }
    return cr;
}

void write_cr(ControlRegisterType control_register_type, CR value) {
    switch (control_register_type) {
        case CR_0:
            __asm__ volatile ("mov %0, %%cr0"::"r"(value));
            break;
        case CR_2:
            __asm__ volatile ("mov %0, %%cr2"::"r"(value));
            break;
        case CR_3:
            __asm__ volatile ("mov %0, %%cr3"::"r"(value));
            break;
        case CR_4:
            __asm__ volatile ("mov %0, %%cr4"::"r"(value));
            break;
        default:
            break;
    }
}
