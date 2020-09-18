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

void draw_pixel_RGB(uint64_t x, uint64_t y, RGB *rgb);

void draw_pixel(uint64_t x, uint64_t y, uint8_t red, uint8_t green, uint8_t blue);

void draw_pixel_foreground(uint64_t x, uint64_t y);

void draw_pixel_background(uint64_t x, uint64_t y);

void fill_screen_RGB(RGB *rgb);

void fill_screen(uint8_t red, uint8_t green, uint8_t blue);

void clear_screen();

void set_foreground_RGB(RGB *rgb);

void set_foreground(uint8_t red, uint8_t green, uint8_t blue);

void set_background_RGB(RGB *rgb);

void set_background(uint8_t red, uint8_t green, uint8_t blue);

uint64_t get_screen_height();

uint64_t get_screen_width();

bool is_in_screen(uint64_t x, uint64_t y);

void draw_fill_box_RGB(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, RGB *rgb);

void draw_fill_box(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint8_t red, uint8_t green, uint8_t blue);

#endif //EGOISTICOS_FRAMEBUFFER_H
