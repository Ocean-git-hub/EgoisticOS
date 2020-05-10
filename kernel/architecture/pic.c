#include <architecture/pic.h>

#include <stdbool.h>

#include <architecture/ioport.h>
#include <architecture/io.h>

#include <graphics.h>

#define INTERRUPT_NUMBER_MASTER_BASE 0x20
#define INTERRUPT_NUMBER_SLAVE_BASE 0x28
#define PIC_ICW3_SLAVE_ID_IRQ2 0b010
#define PIC_OCW2_NORMAL_EOI_COMMAND 0b011

typedef union {
    uint8_t bits;
    struct {
        uint8_t IC4: 1;
        uint8_t SNGL: 1;
        uint8_t ADI: 1;
        uint8_t LTIIM: 1;
        uint8_t one: 1;
        uint8_t IVA: 3;
    } ICW1;
    union {
        uint8_t icw2;
        struct {
            uint8_t IVAMCS: 3;
            uint8_t IVAx86: 5;
        };
    } ICW2;
    union {
        struct {
            uint8_t S0: 1;
            uint8_t S1: 1;
            uint8_t S2: 1;
            uint8_t S3: 1;
            uint8_t S4: 1;
            uint8_t S5: 1;
            uint8_t S6: 1;
            uint8_t S7: 1;
        } master;
        struct {
            uint8_t ID: 2;
            uint8_t zero: 6;
        } slave;
    } ICW3;
    struct {
        uint8_t isx86: 1;
        uint8_t AEOI: 1;
        uint8_t MS: 1;
        uint8_t BUF: 1;
        uint8_t SFNM: 1;
        uint8_t zero: 3;
    } ICW4;
} ICW;

typedef union {
    uint8_t bits;
    union {
        struct {
            uint8_t IRQ0: 1;
            uint8_t IRQ1: 1;
            uint8_t IRQ2: 1;
            uint8_t IRQ3: 1;
            uint8_t IRQ4: 1;
            uint8_t IRQ5: 1;
            uint8_t IRQ6: 1;
            uint8_t IRQ7: 1;
        } master;
        struct {
            uint8_t IRQ8: 1;
            uint8_t IRQ9: 1;
            uint8_t IRQ10: 1;
            uint8_t IRQ11: 1;
            uint8_t IRQ12: 1;
            uint8_t IRQ13: 1;
            uint8_t IRQ14: 1;
            uint8_t IRQ15: 1;
        } slave;
    } OCW1;
    struct {
        uint8_t IRN: 3;
        uint8_t zero: 2;
        uint8_t command: 3;
    } OCW2;
} OCW;

void mask_all_pic_irq(){
    OCW ocw;
    ocw.bits = PIC_INTERRUPT_MASK_REGISTER_IRQ_ALL_MASK;
    io_write_b(IO_PORT_PIC_MASTER_INTERRUPT_MASK_REGISTER, ocw.bits);
    io_write_b(IO_PORT_PIC_SLAVE_INTERRUPT_MASK_REGISTER, ocw.bits);
}

void init_pic() {
    ICW icw = {};
    icw.ICW1.IC4 = 1;
    icw.ICW1.one = 1;
    io_write_b(IO_PORT_PIC_MASTER_COMMAND, icw.bits);
    io_write_b(IO_PORT_PIC_SLAVE_COMMAND, icw.bits);

    icw.bits = 0;
    icw.ICW2.icw2 = INTERRUPT_NUMBER_MASTER_BASE;
    io_write_b(IO_PORT_PIC_MASTER_DATA, icw.bits);
    icw.ICW2.icw2 = INTERRUPT_NUMBER_SLAVE_BASE;
    io_write_b(IO_PORT_PIC_SLAVE_DATA, icw.bits);

    icw.bits = 0;
    icw.ICW3.master.S2 = 1;
    io_write_b(IO_PORT_PIC_MASTER_DATA, icw.bits);
    icw.bits = 0;
    icw.ICW3.slave.ID = PIC_ICW3_SLAVE_ID_IRQ2;
    io_write_b(IO_PORT_PIC_SLAVE_DATA, icw.bits);

    icw.bits = 0;
    icw.ICW4.isx86 = 1;
    io_write_b(IO_PORT_PIC_MASTER_DATA, icw.bits);
    io_write_b(IO_PORT_PIC_SLAVE_DATA, icw.bits);

    mask_all_pic_irq();
}

void set_pic_irq(bool is_enable, uint8_t irq_no) {
    irq_no -= INTERRUPT_NUMBER_MASTER_BASE;
    if (irq_no < 8) {
        OCW current_mask = (OCW) io_read_b(IO_PORT_PIC_MASTER_INTERRUPT_MASK_REGISTER);
        OCW mask = current_mask;
        if (is_enable)
            mask.bits &= ~(1U << irq_no);
        else
            mask.bits |= 1U << irq_no;
        if (mask.bits == current_mask.bits)
            return;
        io_write_b(IO_PORT_PIC_MASTER_INTERRUPT_MASK_REGISTER, mask.bits);
    } else if (irq_no < 16) {
        OCW current_mask = (OCW) io_read_b(IO_PORT_PIC_SLAVE_INTERRUPT_MASK_REGISTER);
        OCW mask = current_mask;
        if (is_enable)
            mask.bits &= ~(1U << irq_no);
        else
            mask.bits |= 1U << irq_no;
        if (mask.bits == current_mask.bits)
            return;
        io_write_b(IO_PORT_PIC_SLAVE_INTERRUPT_MASK_REGISTER, mask.bits);
    }
}

void send_pic_eoi(uint8_t irq_no) {
    irq_no -= INTERRUPT_NUMBER_MASTER_BASE;
    OCW ocw2 = {};
    ocw2.OCW2.IRN = irq_no;
    ocw2.OCW2.command = PIC_OCW2_NORMAL_EOI_COMMAND;
    if (irq_no < 8)
        io_write_b(IO_PORT_PIC_MASTER_COMMAND, ocw2.bits);
    else if (irq_no < 16)
        io_write_b(IO_PORT_PIC_SLAVE_COMMAND, ocw2.bits);
}
