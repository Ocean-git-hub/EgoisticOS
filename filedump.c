#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void hex_dump(char *f_name) {
    FILE *fp = fopen(f_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "File open error\n");
        exit(1);
    }
    unsigned char buf;
    for (int i = 0; fread(&buf, sizeof(char), 1, fp) >= 1; ++i)
        printf("%02x %s", buf, (i + 1) % 16 == 0 ? "\n" : "");
    printf("\n");
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc == 2)
        hex_dump(argv[1]);
    else
        printf("usage: %s <file>\n", argv[0]);
}