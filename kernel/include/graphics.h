#ifndef EGOISTICOS_GRAPHICS_H
#define EGOISTICOS_GRAPHICS_H

#include <stdint.h>
#include "../../common/include/framebuffer.h"

void kernel_print_char(char c);

void kernel_print_string_n(const char *str);

void kernel_printf(const char *format, ...);

void s_print_double(double value, char *s);

void printf_coordinate(uint64_t x, uint64_t y, const char *format, ...);

#endif //EGOISTICOS_GRAPHICS_H
