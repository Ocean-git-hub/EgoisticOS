#include <architecture/x64/interrupt.h>

#include <graphics.h>
#include <architecture/x64/gdt.h>

#define MAX_NUM_IDT_DESCRIPTOR 256

#define IDT_GATE_TYPES_TASK_GATE_32 0x5U
#define IDT_GATE_TYPES_INTERRUPT_GATE_16 0x6U
#define IDT_GATE_TYPES_TRAP_GATE_16 0x7U
#define IDT_GATE_TYPES_INTERRUPT_GATE_32 0xeU
#define IDT_GATE_TYPES_TRAP_GATE_32 0xfU

IDT idt[MAX_NUM_IDT_DESCRIPTOR];

struct __attribute__((packed)) {
    uint16_t size;
    uint64_t offset;
} idtr;

void set_interrupt_descriptor(uint8_t index, void *handler, uint8_t present) {
    idt[index].offset1 = (uint64_t) handler;
    idt[index].offset2 = (uint64_t) handler >> 16U;
    idt[index].offset3 = (uint64_t) handler >> 32U;
    idt[index].selector = SEGMENT_SELECTOR_CODE;
    idt[index].ist = 0;
    idt[index].zero = 0;
    idt[index].type = IDT_GATE_TYPES_INTERRUPT_GATE_32;
    idt[index].zero2 = 0;
    idt[index].dpl = 0;
    idt[index].present = present;
    idt[index].reserved = 0;
}

void DE() {
    kernel_printf("[!] DE has occurred.\n");
    while (1);
}

void DB() {
    kernel_printf("[!] DB has occurred.\n");
    while (1);
}

void NMI() {
    kernel_printf("[!] NMI has occurred.\n");
    while (1);
}

void BP() {
    kernel_printf("[!] BP has occurred.\n");
    while (1);
}

void OF() {
    kernel_printf("[!] OF has occurred.\n");
    while (1);
}

void BRE() {
    kernel_printf("[!] BRE has occurred.\n");
    while (1);
}

void IO() {
    kernel_printf("[!] IO has occurred.\n");
    while (1);
}

void DNA() {
    kernel_printf("[!] DNA has occurred.\n");
    while (1);
}

void DF() {
    kernel_printf("[!] DF has occurred.\n");
    while (1);
}

void CSO() {
    kernel_printf("[!] CSO has occurred.\n");
    while (1);
}

void ITSS() {
    kernel_printf("[!] ITSS has occurred.\n");
    while (1);
}

void SNP() {
    kernel_printf("[!] DE has occurred.\n");
    while (1);
}

void SSF() {
    kernel_printf("[!] SSF has occurred.\n");
    while (1);
}

void GPF() {
    kernel_printf("[!] GPF has occurred.\n");
    while (1);
}

void PF() {
    kernel_printf("[!] PF has occurred.\n");
    while (1);
}

void reserved() {
    kernel_printf("[!] reserved.\n");
    while (1);
}

void x87FPE() {
    kernel_printf("[!] x87FPE has occurred.\n");
    while (1);
}

void AC() {
    kernel_printf("[!] AC has occurred.\n");
    while (1);
}

void MC() {
    kernel_printf("[!] MC has occurred.\n");
    while (1);
}

void SFPE() {
    kernel_printf("[!] SFPE has occurred.\n");
    while (1);
}

void VE() {
    kernel_printf("[!] VE has occurred.\n");
    while (1);
}

void SE() {
    kernel_printf("[!] SE has occurred.\n");
    while (1);
}

void default_handler() {
    kernel_printf("[!] An unregistered interrupt has occurred.\n");
    while (1);
}

void init_interrupt() {
    void *exceptions[] = {DE, DB, NMI, BP, OF, BRE, IO, DNA, DF, CSO, ITSS, SNP, SSF, GPF, PF, reserved, x87FPE, AC, MC,
                          SFPE, VE, reserved, SE, reserved};
    for (uint8_t i = 0; i < 24; ++i)
        set_interrupt_descriptor(i, exceptions[i], 1);
    idtr.size = sizeof(idt) - 1;
    idtr.offset = (uint64_t) idt;
    __asm__ volatile ("lidt %0\n"::"m"(idtr));
}
