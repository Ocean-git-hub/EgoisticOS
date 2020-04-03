#ifndef EGOISTICOS_MENU_H
#define EGOISTICOS_MENU_H

#include <bool.h>
#include <io.h>
#include "efi/efidef.h"
#include "efi/efistruct.h"

#define MAX_MUN_MENUS 50
#define MAX_MENU_NAME_LEN 255

typedef struct {
    CHAR16 *menuName;

    void (*menuFunction)(void *);

    void *argument;
    bool isSelected;
} Menu;


typedef struct {
    uint64_t cursor;
    uint64_t numMenus;
    Menu *menus[MAX_MUN_MENUS];
} MenuList;

void menu_init(EFI_SYSTEM_TABLE *system_table);

void print_menu(MenuList *menu_list);

EFI_INPUT_KEY select_menu(MenuList *menu_list);

#endif //EGOISTICOS_MENU_H
