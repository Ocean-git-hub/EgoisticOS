#include <keyboard.h>

#include <stdint.h>
#include <x64.h>
#include <keyboard/kbencoder.h>
#include <keyboard/kbcontroller.h>
#include <graphics.h>

#define DO_NOT_USE 0

#define ASCII_ESCAPE 0x1b
#define ASCII_BACKSPACE 0x08
#define ASCII_TAB 0x09

#define CAPS_LOCK 0
#define LEFT_CONTROL 0
#define LEFT_SHIFT 0
#define LEFT_ALT 0
#define RIGHT_SHIFT 0
#define HANKAKU_ZENKAKU 0
#define FUNCTION_1 0
#define FUNCTION_2 0
#define FUNCTION_3 0
#define FUNCTION_4 0
#define FUNCTION_5 0
#define FUNCTION_6 0
#define FUNCTION_7 0
#define FUNCTION_8 0
#define FUNCTION_9 0
#define FUNCTION_10 0
#define FUNCTION_11 0
#define FUNCTION_12 0
#define SCROLL_LOCK 0
#define NUM_LOCK 0

bool is_set_scan_code;
char current_scan_code;
KeyStatus key_status;

const char keymap_codeset1[] = {
        DO_NOT_USE, ASCII_ESCAPE, '1', '2', '3', '4', '5', '6',
        '7', '8', '9', '0', '-', '^', ASCII_BACKSPACE, ASCII_TAB,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
        'o', 'p', '@', '[', '\n', LEFT_CONTROL, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
        ':', HANKAKU_ZENKAKU, LEFT_SHIFT, ']', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', RIGHT_SHIFT, '*',
        LEFT_ALT, ' ', CAPS_LOCK, FUNCTION_1, FUNCTION_2, FUNCTION_3, FUNCTION_4, FUNCTION_5,
        FUNCTION_6, FUNCTION_7, FUNCTION_8, FUNCTION_9, FUNCTION_10, NUM_LOCK, SCROLL_LOCK, '7',
        '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, '_', 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, '\\', 0x00, 0x00
};

uint8_t get_key_data() {
    while ((read_keyboard_controller_status() & KEYBOARD_CONTROLLER_STATUS_BIT_OBF) == 0);
    return io_read_b(IO_PORT_KEYBOARD_CONTROLLER_DATA_PORT);
}

uint8_t get_scan_code() {
    uint8_t scan_code;
    while (((scan_code = get_key_data()) & KEYBOARD_CONTROLLER_STATUS_BIT_ST7) != 0);
    return scan_code;
}

void do_keyboard_interrupt() {
    if ((read_keyboard_controller_status() & KEYBOARD_CONTROLLER_STATUS_BIT_OBF) != 0) {
        uint8_t keycode = io_read_b(IO_PORT_KEYBOARD_CONTROLLER_DATA_PORT);
        if (!(keycode & KEYBOARD_CONTROLLER_DATA_BREAK))
            kernel_printf("%c", keymap_codeset1[keycode]);
//        current_scan_code = get_scan_code();
//        is_set_scan_code = true;
    }
    send_apic_eoi(IRQ_KEYBOARD);
}

void keyboard_interrupt() {
    __asm__ volatile ("push %rax\n"
                      "push %rbx\n"
                      "push %rcx\n"
                      "push %rdx\n"
                      "push %rsp\n"
                      "push %rbp\n"
                      "push %rsi\n"
                      "push %rdi\n"
                      "push %r8\n"
                      "push %r9\n"
                      "push %r10\n"
                      "push %r11\n"
                      "push %r12\n"
                      "push %r13\n"
                      "push %r14\n"
                      "push %r15\n");
    do_keyboard_interrupt();
    __asm__ volatile ("pop %r15\n"
                      "pop %r14\n"
                      "pop %r13\n"
                      "pop %r12\n"
                      "pop %r11\n"
                      "pop %r10\n"
                      "pop %r9\n"
                      "pop %r8\n"
                      "pop %rdi\n"
                      "pop %rsi\n"
                      "pop %rbp\n"
                      "pop %rsp\n"
                      "pop %rdx\n"
                      "pop %rcx\n"
                      "pop %rbx\n"
                      "pop %rax\n"
                      "add $8, %rsp\n"
                      "iretq\n");
}

void init_keyboard() {
    set_interrupt_descriptor(IRQ_KEYBOARD, keyboard_interrupt, 1);
    set_apic_irq(true, IRQ_KEYBOARD);
}

char get_char() {
    return keymap_codeset1[get_scan_code()];
}
