#ifndef EGOISTICOS_HPET_H
#define EGOISTICOS_HPET_H

#include <acpi.h>
#include <stdbool.h>

#define MILLI_PER_MICRO 1000
#define ONE_PER_MICRO 1000000

void init_hpet(SDTHeader *hpet);

void dump_hpet_info();

bool set_timer(uint8_t timer_no, uint64_t micro_second, bool periodic_mode, uint8_t *irq_no, void *handler);

#endif //EGOISTICOS_HPET_H
