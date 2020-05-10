#include <graphics.h>
#include <stdint.h>
#include <stdarg.h>
#include <font.h>
#include "../common/include/framebuffer.h"

#define MAX_DECIMAL_BUFFER 20

uint64_t cursor_x = font_width, cursor_y = font_height;

void kernel_print_char(char c) {
    if (c == '\n') {
        cursor_x = font_width;
        cursor_y += font_height;
        return;
    }
    if (c == '\t') {
        for (uint8_t i = 0; i < 4; ++i)
            kernel_print_char(' ');
        return;
    }
    if (cursor_x + font_width >= get_screen_width()) {
        cursor_x = font_width;
        cursor_y += font_height;
    }
    if (cursor_y + font_height >= get_screen_height()) {
        cursor_x = font_width;
        cursor_y = font_height;
        clear_screen();
    }
    const uint8_t *font = font_data[c - 0x20];
    for (uint8_t y = 0; y < font_height; ++y) {
        uint8_t font_w = font[y];
        for (int8_t x = font_width; x >= 0; x--) {
            uint8_t pixel = (font_w >> (x - 1)) & 0b1U;
            if (pixel == 1)
                draw_pixel_foreground(font_width - x + cursor_x, y + cursor_y);
        }
    }
    cursor_x += font_width;
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

uint8_t s_print_unsigned_decimal(uint64_t value, char *s) {
    uint8_t digit = 1;
    uint64_t value_tmp = value;
    while ((value_tmp /= 10) != 0)
        digit++;
    s[digit] = '\0';
    for (int16_t i = digit - 1; i >= 0; i--) {
        s[i] = '0' + value % 10;
        value /= 10;
    }
    return digit;
}

uint8_t s_print_decimal(uint64_t value, char *s) {
    char is_signed = value >> 63U;
    if (is_signed) {
        *s++ = '-';
        value *= -1;
    }
    return s_print_unsigned_decimal(value, s) + is_signed;
}

int8_t s_print_hex64(uint64_t value, char *s) {
    char tmp[17] = {0}, unit_val;
    int8_t i;
    uint8_t j = 0;
    for (i = 0; i < 16 && value > 0; i++) {
        unit_val = value & 0xfU;
        tmp[i] = unit_val + ((unit_val < 0xa) ? '0' : 'A' - 0xa);
        value >>= 4U;
    }
    while (i > 0)
        s[j++] = tmp[--i];
    s[j] = '\0';
    return j;
}

int8_t s_print_hex32(uint32_t value, char *s) {
    char tmp[9] = {0}, unit_val;
    int8_t i;
    uint8_t j = 0;
    for (i = 0; i < 8 && value > 0; i++) {
        unit_val = value & 0xfU;
        tmp[i] = unit_val + ((unit_val < 0xa) ? '0' : 'A' - 0xa);
        value >>= 4U;
    }
    while (i > 0)
        s[j++] = tmp[--i];
    s[j] = '\0';
    return j;
}

int64_t power(uint8_t x, uint8_t y) {
    uint8_t ret_x = 1;
    for (uint8_t i = 0; i < y; ++i)
        ret_x *= x;
    return ret_x;
}

#define _READING_ZERO_BUFFER_SIZE 3

void kernel_printf(const char *format, ...) {
    va_list args;
    char reading_zero_buf[_READING_ZERO_BUFFER_SIZE] = {0}, buf[MAX_DECIMAL_BUFFER + 1];
    uint8_t reading_zero_buf_index, reading_zero;
    int8_t n_reading_zero;
    va_start(args, format);
    while (*format != '\0') {
        if (*format != '%') {
            kernel_print_char(*format++);
            continue;
        }
        format++;
        reading_zero = reading_zero_buf_index = 0;
        if (*format == '0') {
            format++;
            while ('0' <= *format && *format <= '9')
                if (reading_zero_buf_index < _READING_ZERO_BUFFER_SIZE)
                    reading_zero_buf[reading_zero_buf_index++] = *format++;
            for (uint8_t i = 0; i < reading_zero_buf_index; ++i)
                reading_zero += (reading_zero_buf[reading_zero_buf_index - i - 1] - '0') * power(10, i);
        }
        switch (*format) {
            case 'c':
                kernel_print_char(va_arg(args, int32_t));
                break;
            case 's':
                kernel_print_string(va_arg(args, const char*));
                break;
            case 'd':
                n_reading_zero = reading_zero - s_print_decimal(va_arg(args, int32_t), buf);
                for (int j = 0; j < n_reading_zero; ++j)
                    kernel_print_char('0');
                kernel_print_string(buf);
                break;
            case 'u':
                n_reading_zero = reading_zero - s_print_unsigned_decimal(va_arg(args, int32_t), buf);
                for (int j = 0; j < n_reading_zero; ++j)
                    kernel_print_char('0');
                kernel_print_string(buf);
                break;
            case 'x':
                n_reading_zero = reading_zero - s_print_hex32(va_arg(args, int32_t), buf);
                for (int j = 0; j < n_reading_zero; ++j)
                    kernel_print_char('0');
                kernel_print_string(buf);
                break;
            case 'l':
                switch (*++format) {
                    case 'd':
                        n_reading_zero = reading_zero - s_print_decimal(va_arg(args, int64_t), buf);
                        for (int j = 0; j < n_reading_zero; ++j)
                            kernel_print_char('0');
                        kernel_print_string(buf);
                        break;
                    case 'u':
                        n_reading_zero = reading_zero - s_print_unsigned_decimal(va_arg(args, int64_t), buf);
                        for (int j = 0; j < n_reading_zero; ++j)
                            kernel_print_char('0');
                        kernel_print_string(buf);
                        break;
                    case 'x':
                        n_reading_zero = reading_zero - s_print_hex64(va_arg(args, int64_t), buf);
                        for (int j = 0; j < n_reading_zero; ++j)
                            kernel_print_char('0');
                        kernel_print_string(buf);
                        break;
                    default:
                        kernel_print_char(*format);
                }
                break;
            default:
                kernel_print_char(*format);
        }
        format++;
    }
    va_end(args);
}

void print_char_coordinate(char c, uint64_t start_x, uint64_t start_y) {
    const uint8_t *font = font_data[c - 0x20];
    for (uint8_t y = 0; y < font_height; ++y) {
        uint8_t font_w = font[y];
        for (int8_t x = font_width; x >= 0; x--) {
            uint8_t pixel = (font_w >> (x - 1U)) & 0b1U;
            if (pixel == 1)
                draw_pixel_foreground(font_width - x + start_x, y + start_y);
            else
                draw_pixel_background(font_width - x + start_x, y + start_y);
        }
    }
}

void print_string_coordinate(const char *str, uint64_t *x, uint64_t *y) {
    while (*str != '\0') {
        print_char_coordinate(*str++, *x, *y);
        *x += font_width;
    }
}

uint64_t printf_c_x, printf_c_y;

void printf_coordinate(uint64_t x, uint64_t y, const char *format, ...) {
    va_list args;
    char reading_zero_buf[_READING_ZERO_BUFFER_SIZE] = {0}, buf[MAX_DECIMAL_BUFFER + 1];
    uint8_t reading_zero_buf_index, reading_zero;
    int8_t n_reading_zero;
    va_start(args, format);
    while (*format != '\0') {
        if (*format != '%') {
            print_char_coordinate(*format++, x, y);
            x += font_width;
            continue;
        }
        format++;
        reading_zero = reading_zero_buf_index = 0;
        if (*format == '0') {
            format++;
            while ('0' <= *format && *format <= '9')
                if (reading_zero_buf_index < _READING_ZERO_BUFFER_SIZE)
                    reading_zero_buf[reading_zero_buf_index++] = *format++;
            for (uint8_t i = 0; i < reading_zero_buf_index; ++i)
                reading_zero += (reading_zero_buf[reading_zero_buf_index - i - 1] - '0') * power(10, i);
        }
        switch (*format) {
            case 'c':
                print_char_coordinate(va_arg(args, int32_t), x, y);
                x += font_width;
                break;
            case 's':
                print_string_coordinate(va_arg(args, const char*), &x, &y);
                break;
            case 'd':
                n_reading_zero = reading_zero - s_print_decimal(va_arg(args, int32_t), buf);
                for (int j = 0; j < n_reading_zero; ++j) {
                    print_char_coordinate('0', x, y);
                    x += font_width;
                }
                print_string_coordinate(buf, &x, &y);
                break;
            case 'u':
                n_reading_zero = reading_zero - s_print_unsigned_decimal(va_arg(args, int32_t), buf);
                for (int j = 0; j < n_reading_zero; ++j) {
                    print_char_coordinate('0', x, y);
                    x += font_width;
                }
                print_string_coordinate(buf, &x, &y);
                break;
            case 'x':
                n_reading_zero = reading_zero - s_print_hex32(va_arg(args, int32_t), buf);
                for (int j = 0; j < n_reading_zero; ++j) {
                    print_char_coordinate('0', x, y);
                    x += font_width;
                }
                print_string_coordinate(buf, &x, &y);
                break;
            case 'l':
                switch (*++format) {
                    case 'd':
                        n_reading_zero = reading_zero - s_print_decimal(va_arg(args, int64_t), buf);
                        for (int j = 0; j < n_reading_zero; ++j) {
                            print_char_coordinate('0', x, y);
                            x += font_width;
                        }
                        print_string_coordinate(buf, &x, &y);
                        break;
                    case 'u':
                        n_reading_zero = reading_zero - s_print_unsigned_decimal(va_arg(args, int64_t), buf);
                        for (int j = 0; j < n_reading_zero; ++j) {
                            print_char_coordinate('0', x, y);
                            x += font_width;
                        }
                        print_string_coordinate(buf, &x, &y);
                        break;
                    case 'x':
                        n_reading_zero = reading_zero - s_print_hex64(va_arg(args, int64_t), buf);
                        for (int j = 0; j < n_reading_zero; ++j) {
                            print_char_coordinate('0', x, y);
                            x += font_width;
                        }
                        print_string_coordinate(buf, &x, &y);
                        break;
                    default:
                        kernel_print_char(*format);
                }
                break;
            default:
                kernel_print_char(*format);
        }
        format++;
    }
    va_end(args);
}
