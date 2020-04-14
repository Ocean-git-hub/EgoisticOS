#include "include/efiio.h"
#include "include/efi/efidef.h"
#include "include/efisystab.h"

void print_string(CHAR16 *str) {
    system_table->ConOut->EFI_OUT_PUT_STRING(system_table->ConOut, str);
}

void print_string_n(CHAR16 *str) {
    print_string(str);
    print_string(L"\r\n");
}

void print_char(CHAR16 c) {
    CHAR16 buf[2] = {c};
    print_string(buf);
}

EFI_INPUT_KEY get_input_key() {
    EFI_INPUT_KEY key;
    uint64_t index;
    system_table->BootServices->EFI_WAIT_FOR_EVENT(1, &(system_table->ConIn->WaitForKey), &index);
    while (system_table->ConIn->EFI_INPUT_READ_KEY(system_table->ConIn, &key));
    return key;
}

uint64_t gets(CHAR16 *buf, uint64_t buf_size) {
    uint64_t i;
    for (i = 0; i < buf_size - 1;) {
        buf[i] = get_input_key().UnicodeChar;
        print_char(buf[i]);
        if (buf[i] == L'\r' || buf[i] == L'\n') {
            print_string_n(L"");
            break;
        }
        i++;
    }
    buf[i] = L'\0';

    return i;
}
