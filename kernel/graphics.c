#include <graphics.h>
#include <stdarg.h>
#include <font.h>
#include <string.h>

uint64_t cursor_x = FONT_WIDTH, cursor_y = FONT_HEIGHT;

void kernel_print_char(char c) {
    if (c == '\n') {
        cursor_x = FONT_WIDTH;
        cursor_y += FONT_HEIGHT;
        return;
    }
    if (c == '\t') {
        for (uint8_t i = 0; i < 4; ++i)
            kernel_print_char(' ');
        return;
    }
    if (cursor_x + FONT_WIDTH >= get_screen_width()) {
        cursor_x = FONT_WIDTH;
        cursor_y += FONT_HEIGHT;
    }
    if (cursor_y + FONT_HEIGHT >= get_screen_height()) {
        cursor_x = FONT_WIDTH;
        cursor_y = FONT_HEIGHT;
        clear_screen();
    }
    const uint8_t *font = font_data[c - 0x20];
    for (uint8_t y = 0; y < FONT_HEIGHT; ++y) {
        uint8_t font_w = font[y];
        for (int8_t x = FONT_WIDTH; x >= 0; x--) {
            uint8_t pixel = (font_w >> (x - 1)) & 0b1U;
            if (pixel == 1)
                draw_pixel_foreground(FONT_WIDTH - x + cursor_x, y + cursor_y);
        }
    }
    cursor_x += FONT_WIDTH;
}

void kernel_print_string(const char *str) {
    while (*str != '\0')
        kernel_print_char(*str++);
}

void kernel_print_string_n(const char *str) {
    while (*str != '\0')
        kernel_print_char(*str++);
    kernel_print_char('\n');
}

void kernel_printf(const char *format, ...) {
    char buf[1024];
    va_list args;
    va_start(args, format);
    vs_printf(buf, format, args);
    va_end(args);
    kernel_print_string(buf);
}

void print_char_coordinate(char c, uint64_t start_x, uint64_t start_y) {
    const uint8_t *font = font_data[c - 0x20];
    for (uint8_t y = 0; y < FONT_HEIGHT; ++y) {
        uint8_t font_w = font[y];
        for (int8_t x = FONT_WIDTH; x >= 0; x--) {
            uint8_t pixel = (font_w >> (x - 1U)) & 0b1U;
            if (pixel == 1)
                draw_pixel_foreground(FONT_WIDTH - x + start_x, y + start_y);
        }
    }
}

void print_char_coordinate_RGB(char c, uint64_t start_x, uint64_t start_y, const RGB *rgb) {
    const uint8_t *font = font_data[c - 0x20];
    for (uint8_t y = 0; y < FONT_HEIGHT; ++y) {
        uint8_t font_w = font[y];
        for (int8_t x = FONT_WIDTH; x >= 0; x--) {
            uint8_t pixel = (font_w >> (x - 1U)) & 0b1U;
            if (pixel == 1)
                draw_pixel_RGB(FONT_WIDTH - x + start_x, y + start_y, rgb);
        }
    }
}

void print_string_coordinate(const char *str, uint64_t x, uint64_t y) {
    for (; *str != '\0'; x += FONT_WIDTH)
        print_char_coordinate(*str++, x, y);
}

void print_string_coordinate_RGB(const char *str, uint64_t x, uint64_t y, const RGB *rgb) {
    for (; *str != '\0'; x += FONT_WIDTH)
        print_char_coordinate_RGB(*str++, x, y, rgb);
}

void printf_coordinate(uint64_t x, uint64_t y, const char *format, ...) {
    char buf[1024];
    va_list args;
    va_start(args, format);
    vs_printf(buf, format, args);
    va_end(args);
    print_string_coordinate(buf, x, y);
}

void printf_coordinate_RGB(uint64_t x, uint64_t y, const RGB *rgb, const char *format, ...) {
    char buf[1024];
    va_list args;
    va_start(args, format);
    vs_printf(buf, format, args);
    va_end(args);
    print_string_coordinate_RGB(buf, x, y, rgb);
}
