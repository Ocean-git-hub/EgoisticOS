#include "include/framebuffer.h"

#include <stdbool.h>

FrameBuffer frameBuffer;

void init_frame_buffer_info(FrameBuffer *_frameBuffer) {
    frameBuffer = *_frameBuffer;
}

bool is_in_screen(uint64_t x, uint64_t y) {
    return 0 <= x && x < frameBuffer.screenWidth && 0 <= y && y < frameBuffer.screenHeight;
}

void draw_pixel(uint64_t x, uint64_t y, RGB *rgb) {
    PixelFormat *pixelFormat = (PixelFormat *)
            (frameBuffer.frameBufferBase + sizeof(PixelFormat) * (frameBuffer.screenWidth * y + x));
    pixelFormat->rgb = *rgb;
}

void fill_screen(RGB *rgb) {
    for (uint64_t i = 0; i < frameBuffer.screenHeight; ++i)
        for (uint64_t j = 0; j < frameBuffer.screenWidth; ++j)
            draw_pixel(j, i, rgb);
}
