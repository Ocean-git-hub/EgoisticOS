#include "include/menu.h"

EFI_SYSTEM_TABLE *st;

void menu_init(EFI_SYSTEM_TABLE *system_table) {
    st = system_table;
}

void print_menu(MenuList *menu_list) {
    for (uint64_t i = 0; i < menu_list->numMenus; ++i) {
        if (i == menu_list->cursor)
            st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, L"* ");
        else
            st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, L"  ");
        st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, menu_list->menus[i]->menuName);
        st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, L"\r\n");
    }
}

EFI_INPUT_KEY select_menu(MenuList *menu_list) {
    EFI_INPUT_KEY key;
    uint64_t index;
    print_menu(menu_list);
    st->BootServices->EFI_WAIT_FOR_EVENT(1, &st->ConIn->WaitForKey, &index);
    st->ConIn->EFI_INPUT_READ_KEY(st->ConIn, &key);
    switch (key.ScanCode) {
        case EFI_SCAN_CODE_UP:
            menu_list->cursor = ((menu_list->cursor == 0) ? menu_list->numMenus : menu_list->cursor) - 1;
            break;
        case EFI_SCAN_CODE_DOWN:
            ++menu_list->cursor;
            menu_list->cursor %= menu_list->numMenus;
            break;
        default:
            if (key.UnicodeChar == '\r') {
                if (menu_list->menus[menu_list->cursor]->menuFunction != NULL)
                    menu_list->menus[menu_list->cursor]->menuFunction(menu_list->menus[menu_list->cursor]->argument);
                for (uint64_t i = 0; i < menu_list->numMenus; ++i)
                    menu_list->menus[i]->isSelected = false;
                menu_list->menus[menu_list->cursor]->isSelected = true;
                menu_list->cursor = 0;
                break;
            }
    }
    return key;
}
