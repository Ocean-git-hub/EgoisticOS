#ifndef EGOISTICOS_KEYBOARD_H
#define EGOISTICOS_KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>
#include <keyboard/kbencoder.h>
#include <keyboard/kbcontroller.h>

typedef union {
    uint8_t bits;
    struct {
        uint8_t isScrollLock: 1;
        uint8_t isNumLock: 1;
        uint8_t isCapsLock: 1;
        uint8_t isShift: 1;
        uint8_t isControl: 1;
        uint8_t isAlt: 1;
    };
} KeyStatus;

typedef enum {
    BackSpace = 1,
    Delete,
    CapsLock,
    NumLock,
    ScrollLock,
    LeftShift,
    RightShift,
    LeftCtrl,
    RightCtrl,
    LeftAlt,
    RightAlt,
    UpArrow,
    DownArrow,
    RightArrow,
    LeftArrow,
    Home,
    End,
    Insert,
    PageUp,
    PageDown,
    PrintScreen,
    Pause,
    Esc,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12
} UnprintableCode;

typedef struct {
    KeyStatus keyStatus;
    UnprintableCode unprintableCode;
    char asciiChar;
} InputKey;

void do_keyboard_interrupt();

uint8_t get_scan_code();

char get_char();

void init_keyboard();

#endif //EGOISTICOS_KEYBOARD_H
