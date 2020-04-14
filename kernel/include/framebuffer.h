#ifndef EGOISTICOSBETA_FRAMEBUFFER_H
#define EGOISTICOSBETA_FRAMEBUFFER_H

#include <stdint.h>
#include <bootparms.h>

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} RGB;

typedef struct {
    RGB rgb;
    uint8_t _reserved;
} PixelFormat;


void init_frame_buffer_info(FrameBuffer *_frameBuffer);

void draw_pixel(uint64_t x, uint64_t y, RGB *rgb);

void fill_screen(RGB *rgb);

#endif //EGOISTICOSBETA_FRAMEBUFFER_H
