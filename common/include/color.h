#ifndef EGOISTICOS_COLOR_H
#define EGOISTICOS_COLOR_H

#include <stdint.h>

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} RGB;

extern const RGB rbg_black;
extern const RGB rgb_white;
extern const RGB rgb_green;
extern const RGB rgb_red;
extern const RGB rgb_blue;
extern const RGB rgb_ceramica_blu;
extern const RGB rgb_siro;
extern const RGB rgb_bianco_perla;

#endif //EGOISTICOS_COLOR_H
