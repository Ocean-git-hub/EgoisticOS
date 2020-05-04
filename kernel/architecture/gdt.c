#include <architecture/gdt.h>

#include <stdint.h>

const uint64_t gdt[] = {
        0, 0x00af9a000000ffff, 0x00cf93000000ffff
};

void init_gdt() {
    struct __attribute__((packed)) {
        uint16_t size;
        uint64_t offset;
    } gdtr;
    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t) gdt;
    uint16_t data_segment = SEGMENT_SELECTOR_DATA;
    uint16_t code_segment = SEGMENT_SELECTOR_CODE;
    __asm__ volatile ("lgdt %0\n"
                      "mov %1, %%ax\n"
                      "mov %%ax, %%ds\n"
                      "mov %%ax, %%ss\n"
                      "pushq %2\n"
                      "leaq _label(%%rip), %%rax\n"
                      "pushq %%rax\n"
                      "lretq\n"
                      "_label:\n"
    ::"m"(gdtr), "m"(data_segment), "m"(code_segment):"%rax");
}
