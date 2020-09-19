#include <power.h>
#include <stdint.h>

void (*reset_system)();

void init_power(void (*_reset_system)()) {
    reset_system = _reset_system;
}

void reset_system_power(RESET_TYPE type) {
    __asm__ volatile ("mov $0, %%edx\n"
                      "mov $0, %%r8d\n"
                      "mov $0, %%r9d\n"::"c"(type));
    reset_system();
}
