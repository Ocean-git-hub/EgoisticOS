#ifndef EGOISTICOS_FRAMEBUFFER_H
#define EGOISTICOS_FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} RGB;

typedef struct {
    RGB rgb;
    uint8_t _reserved;
} PixelFormat;

typedef struct {
    uint64_t frameBufferBase, frameBufferSize, screenHeight, screenWidth;
} FrameBuffer;

void init_frame_buffer(FrameBuffer *_frameBuffer);

void draw_pixel(uint64_t x, uint64_t y, RGB *rgb);

void draw_pixel_foreground(uint64_t x, uint64_t y);

void draw_pixel_background(uint64_t x, uint64_t y);

void fill_screen(RGB *rgb);

void clear_screen();

void set_foreground(RGB *rgb);

void set_background(RGB *rgb);

uint64_t get_screen_height();

uint64_t get_screen_width();

bool is_in_screen(uint64_t x, uint64_t y);

void draw_fill_box(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, RGB *rgb);

#endif //EGOISTICOS_FRAMEBUFFER_H
