#include "include/string.h"
#include "include/efi/efidef.h"

void cat_string(CHAR16 *dest, CHAR16 *src) {
    for (; *dest != '\0'; dest++);
    for (; *src != '\0'; src++, dest++)
        *dest = *src;
    *dest = L'\0';
}

uint64_t str_len(CHAR16 *str) {
    uint64_t len = 0;
    for (; *str != '\0'; str++)
        len++;
    return len;
}

void str_n_copy(CHAR16 *dest, CHAR16 *src, int64_t n) {
    while (n-- > 0 || *src == '\0')
        *dest++ = *src++;
    *dest = L'\0';
}

int8_t str16_cmp(CHAR16 *s1, CHAR16 *s2) {
    for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++);
    return *s1 - *s2;
}

int8_t str8_cmp(int8_t *s1, int8_t *s2) {
    for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++);
    return *s1 - *s2;
}

int64_t find_string_r(CHAR16 *str, CHAR16 *token) {
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
