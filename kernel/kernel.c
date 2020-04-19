#include <stdint.h>
#include <bootparms.h>
#include <stdbool.h>

#include "framebuffer.h"
#include "include/font.h"

void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer(&bootParameter->frameBuffer);
    clear_screen();
    RGB green = {0, 255};
    RGB red = {0, 0, 255};
    RGB blue = {255};
    draw_fill_box(20, 20, 120, 100, &green);
    draw_fill_box(60, 60, 160, 140, &red);
    draw_fill_box(100, 100, 200, 180, &blue);

    uint64_t cursor_x = 0, cursor_y = 0;
    for (int k = 0; k < 96; ++k) {
        unsigned char *font = font_data[k];
        for (uint8_t y = 0; y < font_height; ++y) {
            unsigned char font_w = font[y];
            for (uint8_t x = 0; x < font_width; ++x) {
                unsigned char pixel = font_w & (0b1u << (font_width - x - 1u));
                if (pixel != 0)
                    draw_pixel_foreground(x + cursor_x, y + cursor_y);
            }
        }
        cursor_x += 8;
    }

    while (true)
            __asm__ volatile ("hlt\n");
}
