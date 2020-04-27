#ifndef EGOISTICOS_INTERRUPT_H
#define EGOISTICOS_INTERRUPT_H

#include <stdint.h>

void set_interrupt_descriptor(uint8_t index, void *handler, uint8_t present);

void init_intrrupt();

#endif //EGOISTICOS_INTERRUPT_H
