#include <keyboard/kbcontroller.h>

#include <stdint.h>
#include <stdbool.h>
#include <x64.h>
#include <keyboard/kbencoder.h>

#define KEYBOARD_CONTROLLER_SELF_TEST_OK 0x55

KeyboardControllerStatus read_keyboard_controller_status() {
    return (KeyboardControllerStatus) io_read_b(IO_PORT_KEYBOARD_CONTROLLER_STATUS_REGISTER);
}

void write_keyboard_controller_command(uint8_t command) {
    while ((read_keyboard_controller_status().IBF) == KEYBOARD_CONTROLLER_STATUS_BUFFER_EXIST);
    io_write_b(IO_PORT_KEYBOARD_CONTROLLER_COMMAND_REGISTER, command);
}

bool keyboard_self_test() {
    write_keyboard_controller_command(KEYBOARD_CONTROLLER_COMMAND_SELF_TEST);
    while ((read_keyboard_controller_status().OBF) == KEYBOARD_CONTROLLER_STATUS_BUFFER_EMPTY);
    return read_keyboard_encoder_buffer() == KEYBOARD_CONTROLLER_SELF_TEST_OK;
}
