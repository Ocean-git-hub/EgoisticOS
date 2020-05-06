#ifndef EGOISTICOS_KBCONTROLLER_H
#define EGOISTICOS_KBCONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_CONTROLLER_DATA_BREAK 0x80U

#define KEYBOARD_CONTROLLER_STATUS_BUFFER_EMPTY 0
#define KEYBOARD_CONTROLLER_STATUS_BUFFER_EXIST 1

#define KEYBOARD_CONTROLLER_COMMAND_SELF_TEST 0xaa

typedef union {
    uint8_t bits;
    struct {
        uint8_t OBF: 1;
        uint8_t IBF: 1;
        uint8_t F0: 1;
        uint8_t F1: 1;
        uint8_t ST4: 1;
        uint8_t ST5: 1;
        uint8_t ST6: 1;
        uint8_t ST7: 1;
    };
} KeyboardControllerStatus;

KeyboardControllerStatus read_keyboard_controller_status();

void write_keyboard_controller_command(uint8_t command);

bool keyboard_self_test();

#endif //EGOISTICOS_KBCONTROLLER_H
