#ifndef EGOISTICOS_STRING_H
#define EGOISTICOS_STRING_H

#include <stdint.h>

void cat_string(char *dest, char *src);

uint64_t str_len(char *str);

void str_n_copy(char *dest, char *src, int64_t n);

void str_copy(char *dest, char *src);

int8_t str_cmp(char *s1, char *s2);

int64_t find_string_r(char *str, char *token);

#endif //EGOISTICOS_STRING_H
