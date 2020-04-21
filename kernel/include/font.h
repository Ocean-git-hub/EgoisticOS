#ifndef EGOISTICOS_FONT_H
#define EGOISTICOS_FONT_H

#include <stdint.h>

uint8_t font_width = 8, font_height = 16;
const uint8_t font_data[96][16] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x09, 0x12, 0x3f, 0x12, 0x12, 0x24, 0x24, 0x7e, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x10, 0x10, 0x3c, 0x52, 0x50, 0x50, 0x3c, 0x12, 0x12, 0x52, 0x3c, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x30, 0x48, 0x48, 0x30, 0x03, 0x0c, 0x30, 0x46, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x1c, 0x20, 0x20, 0x20, 0x10, 0x30, 0x49, 0x49, 0x45, 0x42, 0x26, 0x19, 0x00, 0x00},
        {0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40, 0x00, 0x00},
        {0x00, 0x00, 0x08, 0x2a, 0x1c, 0x1c, 0x2a, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00},
        {0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x54, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x30, 0x50, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x04, 0x04, 0x08, 0x10, 0x10, 0x20, 0x40, 0x7c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x78, 0x04, 0x04, 0x04, 0x04, 0x78, 0x04, 0x04, 0x04, 0x04, 0x78, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x7c, 0x04, 0x04, 0x04, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x38, 0x04, 0x04, 0x04, 0x04, 0x78, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x38, 0x44, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7c, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x18, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0c, 0x30, 0x40, 0x30, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0x06, 0x01, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3e, 0x42, 0x42, 0x4e, 0x52, 0x52, 0x52, 0x52, 0x4e, 0x40, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x78, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x40, 0x40, 0x40, 0x4e, 0x42, 0x42, 0x42, 0x3e, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x44, 0x48, 0x48, 0x50, 0x70, 0x68, 0x48, 0x44, 0x44, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x66, 0x5a, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x62, 0x52, 0x52, 0x52, 0x52, 0x4a, 0x4a, 0x4a, 0x46, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x54, 0x4c, 0x3c, 0x02, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x3c, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7e, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x5a, 0x66, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0x42, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x7e, 0x02, 0x04, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x7e, 0x00, 0x00},
        {0x00, 0x00, 0x70, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x70, 0x00, 0x00},
        {0x00, 0x00, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00, 0x00},
        {0x00, 0x00, 0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e, 0x00, 0x00},
        {0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00},
        {0x00, 0x00, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x3c, 0x44, 0x44, 0x44, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x78, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x40, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x3c, 0x44, 0x44, 0x44, 0x44, 0x44, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x7c, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x00, 0x0c, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x44, 0x44, 0x44, 0x3c, 0x04, 0x04, 0x44, 0x38},
        {0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00},
        {0x00, 0x00, 0x10, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00},
        {0x00, 0x00, 0x08, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x70, 0x00, 0x00},
        {0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x48, 0x50, 0x60, 0x50, 0x50, 0x48, 0x48, 0x44, 0x00, 0x00},
        {0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x40},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x44, 0x44, 0x44, 0x3c, 0x04, 0x04, 0x04, 0x04},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x40, 0x38, 0x04, 0x04, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x3c, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x28, 0x28, 0x28, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x54, 0x6c, 0x44, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x28, 0x10, 0x10, 0x28, 0x28, 0x44, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x28, 0x28, 0x10, 0x10, 0x10, 0x60},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x04, 0x08, 0x10, 0x10, 0x20, 0x40, 0x7c, 0x00, 0x00},
        {0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x20, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x18, 0x00, 0x00},
        {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
        {0x00, 0x00, 0x18, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x18, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

#endif //EGOISTICOS_FONT_H
