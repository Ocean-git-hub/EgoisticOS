#ifndef EGOISTICOS_STRING_H
#define EGOISTICOS_STRING_H

#include "efi/efidef.h"

void cat_string(CHAR16 *dest, CHAR16 *src);

uint64_t str_len(CHAR16 *str);

void str_n_copy(CHAR16 *dest, CHAR16 *src, int64_t n);

int8_t str_cmp(CHAR16 *s1, CHAR16 *s2);

int64_t find_string_r(CHAR16 *str, CHAR16 *token);

#endif //EGOISTICOS_STRING_H
