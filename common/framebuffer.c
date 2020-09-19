#include "include/framebuffer.h"

FrameBuffer frameBuffer;
RGB background_rgb;
RGB foreground_rgb = {255, 255, 255};

void init_frame_buffer(FrameBuffer *_frameBuffer) {
    frameBuffer = *_frameBuffer;
}

inline __attribute__((always_inline)) bool is_in_screen(uint64_t x, uint64_t y) {
    return x < frameBuffer.screenWidth && y < frameBuffer.screenHeight;
}

inline __attribute__((always_inline)) void draw_pixel_RGB(uint64_t x, uint64_t y, const RGB *rgb) {
    ((PixelFormat *)
            (frameBuffer.frameBufferBase + sizeof(PixelFormat) * (frameBuffer.screenWidth * y + x)))->rgb = *rgb;
}

inline __attribute__((always_inline))
void draw_pixel(uint64_t x, uint64_t y, uint8_t red, uint8_t green, uint8_t blue) {
    RGB rgb = {blue, green, red};
    draw_pixel_RGB(x, y, &rgb);
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

void fill_screen_RGB(const RGB *rgb) {
    for (uint64_t i = 0; i < frameBuffer.screenHeight; ++i)
        for (uint64_t j = 0; j < frameBuffer.screenWidth; ++j)
            draw_pixel_RGB(j, i, rgb);
}

void fill_screen(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint64_t i = 0; i < frameBuffer.screenHeight; ++i)
        for (uint64_t j = 0; j < frameBuffer.screenWidth; ++j)
            draw_pixel(j, i, red, green, blue);
}

void clear_screen() {
    fill_screen_RGB(&background_rgb);
}

void set_foreground_RGB(const RGB *rgb) {
    foreground_rgb = *rgb;
}

void set_foreground(uint8_t red, uint8_t green, uint8_t blue) {
    foreground_rgb.red = red;
    foreground_rgb.green = green;
    foreground_rgb.blue = blue;
}

void set_background_RGB(const RGB *rgb) {
    background_rgb = *rgb;
}

void set_background(uint8_t red, uint8_t green, uint8_t blue) {
    background_rgb.red = red;
    background_rgb.green = green;
    background_rgb.blue = blue;
}

void draw_fill_box_RGB(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, const RGB *rgb) {
    for (uint64_t y = y0; y <= y1; ++y)
        for (uint64_t x = x0; x <= x1; ++x)
            draw_pixel_RGB(x, y, rgb);
}

void draw_fill_box(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint8_t red, uint8_t green, uint8_t blue) {
    for (uint64_t y = y0; y <= y1; ++y)
        for (uint64_t x = x0; x <= x1; ++x)
            draw_pixel(x, y, red, green, blue);
}

uint64_t get_screen_height() {
    return frameBuffer.screenHeight;
}

uint64_t get_screen_width() {
    return frameBuffer.screenWidth;
}
