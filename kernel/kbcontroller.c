#include "include/kbcontroller.h"

#include <stdint.h>
#include <stdbool.h>

#include "include/x64.h"
#include "include/ioport.h"
#include "include/kbencoder.h"

#define KEYBOARD_CONTROLLER_SELF_TEST_OK 0x55

uint8_t read_keyboard_controller_status() {
    return io_read_b(IO_PORT_KEYBOARD_CONTROLLER_STATUS_REGISTER);
}

void write_keyboard_controller_command(uint8_t command) {
    while ((read_keyboard_controller_status() & KEYBOARD_CONTROLLER_STATUS_BIT_IBF) !=
           KEYBOARD_CONTROLLER_STATUS_BUFFER_EMPTY);
    io_write_b(IO_PORT_KEYBOARD_CONTROLLER_COMMAND_REGISTER, command);
}

bool keyboard_self_test() {
    write_keyboard_controller_command(KEYBOARD_CONTROLLER_COMMAND_SELF_TEST);
    while ((read_keyboard_controller_status() & KEYBOARD_CONTROLLER_STATUS_BIT_OBF) ==
           KEYBOARD_CONTROLLER_STATUS_BUFFER_EMPTY);
    return read_keyboard_encoder_buffer() == KEYBOARD_CONTROLLER_SELF_TEST_OK;
}
