#ifndef EGOISTICOS_IO_H
#define EGOISTICOS_IO_H

#include <stdint.h>
#include <architecture/ioport.h>

uint8_t io_read_b(uint16_t address);

void io_write_b(uint16_t address, uint8_t data);

#endif //EGOISTICOS_IO_H
