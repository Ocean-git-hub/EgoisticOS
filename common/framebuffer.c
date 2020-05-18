#include "include/framebuffer.h"

#include <stdbool.h>

FrameBuffer frameBuffer;
RGB background_rgb;
RGB foreground_rgb = {255, 255, 255};

void init_frame_buffer(FrameBuffer *_frameBuffer) {
    frameBuffer = *_frameBuffer;
}

inline __attribute__((always_inline)) bool is_in_screen(uint64_t x, uint64_t y) {
    return x < frameBuffer.screenWidth && y < frameBuffer.screenHeight;
}

inline __attribute__((always_inline)) void draw_pixel(uint64_t x, uint64_t y, RGB *rgb) {
    ((PixelFormat *)
            (frameBuffer.frameBufferBase +
             sizeof(PixelFormat) * (frameBuffer.screenWidth * y + x)))->rgb = *rgb;
}

inline __attribute__((always_inline)) void draw_pixel_foreground(uint64_t x, uint64_t y) {
    ((PixelFormat *)
            (frameBuffer.frameBufferBase +
             sizeof(PixelFormat) * (frameBuffer.screenWidth * y + x)))->rgb = foreground_rgb;
}

inline __attribute__((always_inline)) void draw_pixel_background(uint64_t x, uint64_t y) {
    ((PixelFormat *)
            (frameBuffer.frameBufferBase +
             sizeof(PixelFormat) * (frameBuffer.screenWidth * y + x)))->rgb = background_rgb;
}

void fill_screen(RGB *rgb) {
    for (uint64_t i = 0; i < frameBuffer.screenHeight; ++i)
        for (uint64_t j = 0; j < frameBuffer.screenWidth; ++j)
            draw_pixel(j, i, rgb);
}

void clear_screen() {
    fill_screen(&background_rgb);
}

void set_foreground(RGB *rgb) {
    foreground_rgb = *rgb;
}

void set_background(RGB *rgb) {
    background_rgb = *rgb;
}

uint64_t get_screen_height() {
    return frameBuffer.screenHeight;
}

uint64_t get_screen_width() {
    return frameBuffer.screenWidth;
}

void draw_fill_box(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, RGB *rgb) {
    for (uint64_t y = y0; y <= y1; ++y)
        for (uint64_t x = x0; x <= x1; ++x)
            draw_pixel(x, y, rgb);
}
