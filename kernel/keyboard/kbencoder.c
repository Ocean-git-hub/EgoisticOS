#include <keyboard/kbencoder.h>

#include <stdint.h>
#include <stdbool.h>
#include <x64.h>
#include <keyboard/kbcontroller.h>

uint8_t read_keyboard_encoder_buffer() {
    return io_read_b(IO_PORT_KEYBOARD_CONTROLLER_DATA_PORT);
}

void write_keyboard_encoder_command(uint8_t command) {
    while ((read_keyboard_controller_status().IBF) != KEYBOARD_CONTROLLER_STATUS_BUFFER_EMPTY);
    io_write_b(IO_PORT_KEYBOARD_CONTROLLER_DATA_PORT, command);
}

uint8_t set_keyboard_led(uint8_t led) {
    if ((KEYBOARD_LED_SETTING_SCROLL_LOCK | KEYBOARD_LED_SETTING_NUM_LOCK | KEYBOARD_LED_SETTING_CAPS_LOCK) < led)
        false;
    write_keyboard_encoder_command(KEYBOARD_ENCODER_COMMAND_LED_SETTING);
    write_keyboard_encoder_command(led);
    return read_keyboard_encoder_buffer();
}
