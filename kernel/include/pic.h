#ifndef EGOISTICOS_PIC_H
#define EGOISTICOS_PIC_H

#include <stdint.h>
#include <stdbool.h>

#define PIC_IRQ_KEYBOARD 0x21
#define PIC_INTERRUPT_MASK_REGISTER_IRQ_ALL_MASK 0xff

void init_pic();

void set_pic_irq(bool is_enable, uint8_t irq_no);

void send_pic_eoi(uint8_t irq_no);

#endif //EGOISTICOS_PIC_H
