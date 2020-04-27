#ifndef EGOISTICOS_EKF_H
#define EGOISTICOS_EKF_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t magic;
    void *bssStart;
    uint64_t bssSize;
    void *text;
} EKHeader;

#endif //EGOISTICOS_EKF_H
