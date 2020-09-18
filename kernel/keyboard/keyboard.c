#include <keyboard.h>

#include <x64.h>
#include <graphics.h>

bool is_set_key;
InputKey input_key;
InputKey previous_input_key;
bool is_next_e0;

const char keymap_codeset1_printable[128] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', '@', '[', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0, 0, ']', 'z', 'x',
        'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9',
        '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char codeset1_ascii_to_shift[128] = {
        ['1']='!', ['2']='"', ['3']='#', ['4']='$', ['5']='%', ['6']='&', ['7']='\'', ['8']='(', ['9']=')', ['0']='~',
        ['-']='=', ['^']='~', ['@']='`', ['[']='{', [';']='+', [':']='*', [']']='}', [',']='<', ['.']='>', ['/']='?',
        ['\\']='_'
};

const UnprintableCode keymap_codeset1_unprintable[128] = {
        [0x0e]=BackSpace, [0x3a]=CapsLock, [0x2a]=LeftShift, [0x36]=RightShift, [0x1d]=LeftCtrl, [0x38]=LeftAlt,
        [0x45]=NumLock, [0x01]=Esc, [0x3b]=F1, [0x3c]=F2, [0x3d]=F3, [0x3e]=F4, [0x3f]=F5, [0x40]=F6, [0x41]=F7,
        [0x42]=F8, [0x43]=F9, [0x44]=F10, [0x57]=F11, [0x58]=F12, [0x46]=ScrollLock,
};

const UnprintableCode keymap_codeset1_unprintable_e0[128] = {
        [0x38]=RightAlt, [0x1d]=RightCtrl, [0x52]=Insert, [0x53]=Delete, [0x4b]=LeftArrow, [0x47]=Home, [0x4f]=End,
        [0x48]=UpArrow, [0x50]=DownArrow, [0x49]=PageUp, [0x51]=PageDown, [0x4d]=RightArrow,
};

const UnprintableCode keymap_codeset1_unprintable_break[] = {
        [0xaa]=LeftShift, [0xb6]=RightShift, [0x9d]=LeftCtrl, [0xb8]=LeftAlt
};

const UnprintableCode keymap_codeset1_unprintable_break_e0[] = {
        [0xb8]=RightAlt, [0x9d]=RightCtrl
};

uint8_t get_scan_code() {
    while (read_keyboard_controller_status().OBF == KEYBOARD_CONTROLLER_STATUS_BUFFER_EMPTY);
    return io_read_b(IO_PORT_KEYBOARD_CONTROLLER_DATA_PORT);
}

char get_char() {
    while (!is_set_key || input_key.asciiChar == 0);
    is_set_key = false;
    return input_key.asciiChar;
}

void set_key_status(UnprintableCode unprintable_code, bool is_enable) {
    switch (unprintable_code) {
        case CapsLock:
            if (input_key.keyStatus.isShift)
                input_key.keyStatus.isCapsLock = ~input_key.keyStatus.isCapsLock;
            break;
        case NumLock:
            input_key.keyStatus.isNumLock = ~input_key.keyStatus.isNumLock;
            break;
        case ScrollLock:
            input_key.keyStatus.isScrollLock = ~input_key.keyStatus.isScrollLock;
            break;
        case LeftShift:
        case RightShift:
            input_key.keyStatus.isShift = is_enable;
            break;
        case LeftCtrl:
        case RightCtrl:
            input_key.keyStatus.isControl = is_enable;
            break;
        case LeftAlt:
        case RightAlt:
            input_key.keyStatus.isAlt = is_enable;
            break;
        default:
            break;
    }
}

void analyse_key_status() {
    previous_input_key = input_key;
    uint8_t scan_code = get_scan_code();
    if ((scan_code & KEYBOARD_CONTROLLER_DATA_BREAK) == 0) {
        if (is_next_e0 && scan_code < 128) {
            input_key.asciiChar = 0;
            input_key.unprintableCode = keymap_codeset1_unprintable_e0[scan_code];
            set_key_status(input_key.unprintableCode, true);
            is_next_e0 = false;
        } else if (scan_code < 128) {
            uint8_t codeset = keymap_codeset1_printable[scan_code];
            if (codeset != 0) {
                input_key.unprintableCode = 0;
                input_key.asciiChar = codeset;
                if ((input_key.keyStatus.isShift && !input_key.keyStatus.isCapsLock) ||
                    (!input_key.keyStatus.isShift && input_key.keyStatus.isCapsLock)) {
                    if ('a' <= codeset && codeset <= 'z')
                        input_key.asciiChar += 'A' - 'a';
                    else if (codeset != ' ' && codeset != '\n' && codeset != '\t')
                        input_key.asciiChar = codeset1_ascii_to_shift[codeset];
                }
            } else {
                input_key.asciiChar = 0;
                input_key.unprintableCode = keymap_codeset1_unprintable[scan_code];
                set_key_status(input_key.unprintableCode, true);
            }
        }
    } else {
        input_key.asciiChar = 0;
        input_key.unprintableCode = 0;
        if (scan_code == 0xe0) {
            is_next_e0 = true;
        } else {
            UnprintableCode unprintable_code;
            if (is_next_e0) {
                is_next_e0 = false;
                unprintable_code = keymap_codeset1_unprintable_break_e0[scan_code];
            } else
                unprintable_code = keymap_codeset1_unprintable_break[scan_code];
            switch (unprintable_code) {
                case LeftShift:
                case RightShift:
                case LeftAlt:
                case RightAlt:
                case LeftCtrl:
                case RightCtrl:
                    set_key_status(unprintable_code, false);
                default:
                    break;
            }
        }
    }
}

void set_led() {
    set_keyboard_led((input_key.keyStatus.isCapsLock ? KEYBOARD_LED_SETTING_CAPS_LOCK : 0) |
                     (input_key.keyStatus.isNumLock ? KEYBOARD_LED_SETTING_NUM_LOCK : 0) |
                     (input_key.keyStatus.isScrollLock ? KEYBOARD_LED_SETTING_SCROLL_LOCK : 0));
}

void dump_key_status() {
    kernel_printf("key_status: isShift=%d isCtrl=%d isScrLock=%d isNumLock=%d isCapsLock=%d isAlt=%d\n"
                  "ascii: %c(0x%02x) unprintable: 0x%02x\n",
                  input_key.keyStatus.isShift, input_key.keyStatus.isControl, input_key.keyStatus.isScrollLock,
                  input_key.keyStatus.isNumLock, input_key.keyStatus.isCapsLock, input_key.keyStatus.isAlt,
                  input_key.asciiChar, input_key.asciiChar, input_key.unprintableCode);
}

void do_keyboard_interrupt() {
    analyse_key_status();
    if ((previous_input_key.keyStatus.bits & 0b111U) != (input_key.keyStatus.bits & 0b111U))
        set_led();
    is_set_key = true;
    send_apic_eoi();
}

void keyboard_interrupt() {
    enter_interrupt();
    do_keyboard_interrupt();
    leave_interrupt();
}

void init_keyboard() {
    set_interrupt_descriptor(IRQ_KEYBOARD, keyboard_interrupt, 1);
    set_apic_irq(true, IRQ_KEYBOARD);
}
