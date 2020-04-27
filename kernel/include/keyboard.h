#ifndef EGOISTICOS_KEYBOARD_H
#define EGOISTICOS_KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "kbencoder.h"
#include "kbcontroller.h"

typedef struct {
    bool isScrollLock;
    bool isNumLock;
    bool isCapsLock;
    bool isShift;
    bool isControl;
    bool isAlt;
} KeyStatus;

void do_keyboard_interrupt();

uint8_t get_scan_code();

char get_char();

void init_keyboard();

#endif //EGOISTICOS_KEYBOARD_H
