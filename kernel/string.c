#include <string.h>

#define _MAX_DECIMAL_BUFFER 20
#define _READING_ZERO_BUFFER_SIZE 3

void cat_string(char *dest, char *src) {
    for (; *dest != '\0'; dest++);
    for (; *src != '\0'; src++, dest++)
        *dest = *src;
    *dest = L'\0';
}

uint64_t str_len(char *str) {
    uint64_t len = 0;
    for (; *str != '\0'; str++)
        len++;
    return len;
}

void str_n_copy(char *dest, char *src, int64_t n) {
    while (n-- > 0 && *src != '\0')
        *dest++ = *src++;
    *dest = L'\0';
}

void str_copy(char *dest, char *src) {
    while (*src != '\0')
        *dest++ = *src++;
    *dest = L'\0';
}

int8_t str_cmp(char *s1, char *s2) {
    for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++);
    return *s1 - *s2;
}

int64_t find_string_r(char *str, char *token) {
    const int64_t token_len = str_len(token);
    int64_t token_index = token_len;
    for (int64_t i = str_len(str) - 1; i >= 0; i--) {
        if (str[i] != token[--token_index])
            token_index = token_len;
        if (token_index == 0)
            return i;
    }
    return -1;
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
    if (i == 0)
        s[j++] = '0';
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
    if (i == 0)
        s[j++] = '0';
    while (i > 0)
        s[j++] = tmp[--i];
    s[j] = '\0';
    return j;
}

void s_print_double(double value, char *s) {
    uint64_t bits = *(uint64_t *) &value;
    if (bits >> 63U) {
        *s++ = '-';
        value *= -1;
    }
    uint64_t before_decimal_point = value;
    s += s_print_unsigned_decimal(before_decimal_point, s);
    *s++ = '.';
    uint64_t after_decimal_point6 = value * 1000000 - before_decimal_point * 1000000 + 1000000;
    s_print_unsigned_decimal(after_decimal_point6, s);
    for (int i = 0; i < 7; ++i)
        s[i] = s[i + 1];
}

int64_t power(uint8_t x, uint8_t y) {
    uint8_t ret_x = 1;
    for (uint8_t i = 0; i < y; ++i)
        ret_x *= x;
    return ret_x;
}

uint64_t vs_printf(char *str, const char *format, va_list args) {
    char reading_zero_buf[_READING_ZERO_BUFFER_SIZE] = {0}, buf[_MAX_DECIMAL_BUFFER + 1], *arg_char, *_str = str;
    uint8_t reading_zero_buf_index, reading_zero;
    int8_t n_reading_zero;
    while (*format != '\0') {
        if (*format != '%') {
            *str++ = *format++;
            continue;
        } else if (format[1] == '%') {
            *str++ = *format++;
            format++;
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
                *str++ = va_arg(args, int32_t);
                break;
            case 's':
                arg_char = va_arg(args, char*);
                str_copy(str, arg_char);
                str += str_len(arg_char);
                break;
            case 'f':
                s_print_double(va_arg(args, double), buf);
                str_copy(str, buf);
                str += str_len(buf);
                break;
            default:
                switch (*format) {
                    case 'd':
                        n_reading_zero = reading_zero - s_print_decimal(va_arg(args, int32_t), buf);
                        break;
                    case 'u':
                        n_reading_zero = reading_zero - s_print_unsigned_decimal(va_arg(args, int32_t), buf);
                        break;
                    case 'x':
                        n_reading_zero = reading_zero - s_print_hex32(va_arg(args, int32_t), buf);
                        break;
                    case 'l':
                        switch (*++format) {
                            case 'd':
                                n_reading_zero = reading_zero - s_print_decimal(va_arg(args, int64_t), buf);
                                break;
                            case 'u':
                                n_reading_zero = reading_zero - s_print_unsigned_decimal(va_arg(args, int64_t), buf);
                                break;
                            case 'x':
                                n_reading_zero = reading_zero - s_print_hex64(va_arg(args, int64_t), buf);
                                break;
                            default:
                                *str++ = '%';
                                *str++ = 'l';
                                *str++ = *format++;
                                continue;
                        }
                        break;
                    default:
                        *str++ = '%';
                        *str++ = *format++;
                        continue;
                }
                for (int j = 0; j < n_reading_zero; ++j)
                    *str++ = '0';
                str_copy(str, buf);
                str += str_len(buf);
        }
        format++;
    }
    *str = 0;
    return str - _str - 1;
}

uint64_t s_printf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    uint64_t len = vs_printf(str, format, args);
    va_end(args);
    return len;
}