#include <int.h>
#include <io.h>
#include <bool.h>
#include <elf.h>
#include "include/string.h"
#include "include/menu.h"
#include "include/efi/eficonst.h"
#include "include/efi/efistruct.h"
#include "include/efi/efiguid.h"

#define BOOT_LOADER_VERSION L"0.2"

EFI_SYSTEM_TABLE *st;

#define PRINT_STRING(s) (st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, L ## s))

void print_string(CHAR16 *str) {
    st->ConOut->EFI_OUT_PUT_STRING(st->ConOut, str);
}

void hex_dump(uint64_t value, uint8_t digit) {
    uint8_t digit_tmp = digit;
    if (digit > 16)
        return;
    CHAR16 buf[17] = {0}, unit_val;
    for (int8_t i = 15; i >= 0 && digit-- != 0; i--) {
        unit_val = value & 0xfU;
        buf[(unsigned char) i] = unit_val + ((unit_val < 0xa) ? L'0' : L'A' - 0xa);
        value >>= 4U;
    }
    print_string(&buf[16 - digit_tmp]);
}

void dump_status(uint64_t status, bool linefeed) {
    PRINT_STRING("0x");
    hex_dump(status, 16);
    if (linefeed)
        PRINT_STRING("\r\n");
}

#define MAX_DECIMAL_BUFFER 30

void print_decimal(uint64_t value, bool linefeed) {
    int8_t i = 0;
    uint8_t j = 0;
    CHAR16 buf_tmp[MAX_DECIMAL_BUFFER] = {0}, buf[MAX_DECIMAL_BUFFER] = {0};
    do
        buf_tmp[(unsigned char) i++] = L'0' + value % 10;
    while ((value /= 10) > 0 && i < MAX_DECIMAL_BUFFER - 1);
    while (i >= 0)
        buf[j++] = buf_tmp[(unsigned char) --i];
    print_string(buf);
    if (linefeed)
        PRINT_STRING("\r\n");
}

void check_error(uint64_t status, CHAR16 *message) {
    if (status != EFI_SUCCESS) {
        print_string(message);
        PRINT_STRING(": ");
        dump_status(status, true);
    }
}

#define TIME_ZONE_TOKYO 9

void get_time(EFI_TIME *time, uint8_t *hour, uint8_t *minute) {
    *hour = time->Hour + TIME_ZONE_TOKYO;
    *minute = time->Minute;
    if (*hour > 24)
        *hour -= 24;
}

void get_datetime(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute) {
    EFI_TIME time;
    st->RuntimeServices->EFI_GET_TIME(&time, NULL);
    *year = time.Year;
    *month = time.Month;
    *day = time.Day;
    get_time(&time, hour, minute);
}

void print_datetime() {
    uint16_t year;
    uint8_t month, day, hour, minute;
    get_datetime(&year, &month, &day, &hour, &minute);
    print_decimal(year, false);
    PRINT_STRING("/");
    print_decimal(month, false);
    PRINT_STRING("/");
    print_decimal(day, false);
    PRINT_STRING(" ");
    print_decimal(hour, false);
    PRINT_STRING(":");
    print_decimal(minute, false);
}

void print_boot_info() {
    PRINT_STRING("Egoistic Boot Loader ver.");
    print_string(BOOT_LOADER_VERSION);
    PRINT_STRING(" [FirmwareVendor: ");
    print_string(st->FirmwareVendor);
    PRINT_STRING("(");
    switch (st->Hdr.Revision) {
        case EFI_2_70_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.7.0");
            break;
        case EFI_2_60_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.6.0");
            break;
        case EFI_2_40_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.4.0");
            break;
        case EFI_2_50_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.5.0");
            break;
        case EFI_2_31_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.3.1");
            break;
        case EFI_2_30_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.3.0");
            break;
        case EFI_2_20_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.2.0");
            break;
        case EFI_2_10_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.1.0");
            break;
        case EFI_2_00_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.2.0.0");
            break;
        case EFI_1_10_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.1.1.0");
            break;
        case EFI_1_02_SYSTEM_TABLE_REVISION:
            PRINT_STRING("Ver.1.0.2");
            break;
    }
    PRINT_STRING(")]  (Now: ");
    print_datetime();
    PRINT_STRING("(UTC+0900))\r\n");
}


void boot_menu(MenuList *menu_list) {
    while (true) {
        st->ConOut->EFI_TEXT_CLEAR_SCREEN(st->ConOut);
        print_boot_info();
        CHAR16 arrow_up[2] = {ARROW_UP};
        CHAR16 arrow_down[2] = {ARROW_DOWN};
        PRINT_STRING("\r\n////////////////////Select Boot Menu///////////////////\r\n");
        PRINT_STRING("[Esc=Exit Boot Menu ");
        print_string(arrow_up);
        print_string(arrow_down);
        PRINT_STRING("=Move Cursor Enter=Select Entry]\r\n\r\n");
        if (select_menu(menu_list).ScanCode == EFI_SCAN_CODE_ESCAPE)
            return;
    }
}

#define MAX_NAME_SIZE 255

EFI_FILE_INFO *get_file_info(EFI_FILE_PROTOCOL *file_protocol) {
    EFI_GUID file_info_guid = EFI_FILE_INFO_ID;
    UINTN buffer_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * MAX_NAME_SIZE;
    EFI_FILE_INFO *file_info;
    st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, buffer_size,
                                        (void **) &file_info);
    file_protocol->EFI_FILE_GET_INFO(file_protocol, &file_info_guid, &buffer_size, file_info);
    return file_info;
}

bool is_directory(EFI_FILE_PROTOCOL *root, CHAR16 *name) {
    EFI_FILE_PROTOCOL *file_protocol;
    EFI_STATUS status = root->EFI_FILE_OPEN(root, &file_protocol, name, EFI_FILE_MODE_READ, 0);
    if (status != EFI_SUCCESS) {
        dump_status(status, true);
        return 0;
    }
    EFI_FILE_INFO *file_info = get_file_info(file_protocol);
    bool is_directory = file_info->Attribute == EFI_FILE_DIRECTORY;
    st->BootServices->EFI_FREE_POOL(file_info);
    return is_directory;
}

#define MAX_NAME_LEN 255

uint64_t get_file_list(EFI_FILE_PROTOCOL *root, CHAR16 *directory_name, EFI_FILE_INFO *file_buffer[],
                       uint64_t file_buffer_size) {
    uint64_t buffer_size, index = 0;
    if (is_directory(root, directory_name) == true) {
        EFI_FILE_PROTOCOL *directory;
        EFI_STATUS status = root->EFI_FILE_OPEN(root, &directory, directory_name, EFI_FILE_MODE_READ, 0);
        if (status != EFI_SUCCESS) {
            dump_status(status, true);
            return 0;
        }
        while (true) {
            uint8_t *buffer;
            st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, MAX_NAME_LEN, (void **) &buffer);
            buffer_size = MAX_NAME_LEN;
            directory->EFI_FILE_READ(directory, &buffer_size, buffer);
            if (buffer_size == 0 || index == file_buffer_size)
                break;
            file_buffer[index++] = (EFI_FILE_INFO *) buffer;
        }
    }
    return index;
}

#define FILE_BUFFER_SIZE  50

uint64_t
get_file_name_list(EFI_FILE_PROTOCOL *root, CHAR16 *directory_name, CHAR16 **buffer[]) {
    EFI_FILE_INFO *file_info_buffer[FILE_BUFFER_SIZE];
    uint64_t list_size = get_file_list(root, directory_name, file_info_buffer, FILE_BUFFER_SIZE);
    for (uint64_t i = 0; i < list_size; ++i) {
        CHAR16 *file_name;
        int64_t file_name_len = str_len(file_info_buffer[i]->FileName);
        st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, sizeof(CHAR16) * (file_name_len + 1),
                                            (void **) &file_name);
        str_n_copy(file_name, file_info_buffer[i]->FileName, file_name_len);
        (*buffer)[i] = file_name;
    }
    for (uint64_t i = 0; i < list_size; ++i)
        st->BootServices->EFI_FREE_POOL(file_info_buffer[i]);
    return list_size;
}

void explore_file() {
    EFI_GUID simple_file_p = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *simple_file_system_protocol;
    EFI_FILE_PROTOCOL *root;
    st->BootServices->EFI_LOCATE_PROTOCOL(&simple_file_p, NULL, (void **) &simple_file_system_protocol);
    simple_file_system_protocol->EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME(simple_file_system_protocol, &root);

    CHAR16 **file_names = NULL;
    CHAR16 *current_directory = {L"."};
    uint64_t list_size = 0;
    MenuList file_menu_list = {0};
    uint64_t directory_len = 0;
    while (true) {
        st->ConOut->EFI_TEXT_CLEAR_SCREEN(st->ConOut);
        CHAR16 arrow_up[2] = {ARROW_UP};
        CHAR16 arrow_down[2] = {ARROW_DOWN};
        PRINT_STRING("\r\n////////////////////Select Boot File///////////////////\r\n");
        PRINT_STRING("[Esc=Exit Explore ");
        print_string(arrow_up);
        print_string(arrow_down);
        PRINT_STRING("=Move Cursor Enter=Select Entry]\r\n\r\n");
        for (uint64_t j = 0; j < list_size; ++j)
            if (file_menu_list.menus[j]->isSelected) {
                CHAR16 *current_directory_tmp = current_directory;
                if (str_cmp(file_menu_list.menus[j]->menuName, L"..") == 0) {
                    directory_len = str_len(current_directory);
                    st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, sizeof(CHAR16) * (directory_len + 1),
                                                        (void **) &current_directory);
                    current_directory[0] = '\0';
                    current_directory_tmp[find_string_r(current_directory_tmp, L"\\")] = '\0';
                    cat_string(current_directory, current_directory_tmp);
                    st->BootServices->EFI_FREE_POOL(current_directory_tmp);
                } else if (str_cmp(file_menu_list.menus[j]->menuName, L".") != 0) {
                    directory_len = str_len(current_directory) +
                                    str_len(file_menu_list.menus[j]->menuName) + str_len(L"\\");
                    st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, sizeof(CHAR16) * (directory_len + 1),
                                                        (void **) &current_directory);
                    current_directory[0] = '\0';
                    cat_string(current_directory, current_directory_tmp);
                    cat_string(current_directory, L"\\");
                    cat_string(current_directory, file_menu_list.menus[j]->menuName);
                    st->BootServices->EFI_FREE_POOL(current_directory_tmp);
                }
                break;
            }
        if (is_directory(root, current_directory) == false) {
            EFI_FILE_PROTOCOL *file;
            root->EFI_FILE_OPEN(root, &file, current_directory, EFI_FILE_MODE_READ, 0);
            EFI_FILE_INFO *file_info = get_file_info(file);
            uint64_t file_size = file_info->FileSize;
            uint64_t page_size = file_size;
            st->BootServices->EFI_FREE_POOL(file_info);
            uint64_t memory_address = 0x100000;
            st->BootServices->EFI_ALLOCATE_PAGES(AllocateAddress, EfiLoaderCode, (file_size + 4095) / 4096,
                                                 &memory_address);
            file->EFI_FILE_READ(file, &page_size, (void *) memory_address);
            Elf64_Ehdr *ehdr = (Elf64_Ehdr *) memory_address;
            bool is_efi_header = ehdr->e_ident[0] == 0x7f && ehdr->e_ident[1] == 'E' && ehdr->e_ident[2] == 'L' &&
                                 ehdr->e_ident[3] == 'F';
            if (is_efi_header) {

            } else {
                PRINT_STRING("[!] This file is not an elf file.\r\n\r\n");
                for (uint64_t i = 0; i < file_size; ++i) {
                    hex_dump(((uint8_t *) memory_address)[i], 2);
                    if ((i + 1) % 16 == 0)
                        PRINT_STRING("\r\n");
                    else
                        PRINT_STRING(" ");
                }
                CHAR16 s[2] = {0};
                PRINT_STRING("\r\n");
                for (uint64_t i = 0; i < file_size; ++i) {
                    s[0] = ((uint8_t *) memory_address)[i];
                    if (st->ConOut->EFI_TEXT_TEST_STRING(st->ConOut, s) != EFI_SUCCESS)
                        s[0] = L'.';
                    if (s[0] == '\r' || s[0] == '\n')
                        PRINT_STRING("\r\n");
                    else
                        print_string(s);
                }
            }
        }
        for (uint64_t i = 0; i < list_size; ++i)
            st->BootServices->EFI_FREE_POOL(file_names[i]);
        list_size = get_file_name_list(root, current_directory, &file_names);
        file_menu_list.numMenus = list_size;
        Menu menus[MAX_MUN_MENUS] = {0};
        for (uint64_t i = 0; i < list_size; i++) {
            menus[i].menuName = file_names[i];
            file_menu_list.menus[i] = &menus[i];
        }
        if (select_menu(&file_menu_list).ScanCode == EFI_SCAN_CODE_ESCAPE)
            return;
    }
}

void shutdown() {
    st->RuntimeServices->EFI_RESET_SYSTEM(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

void option() {

}

typedef struct {
    EFI_MEMORY_DESCRIPTOR *memoryMapDescriptor;
    uint64_t memoryMapSize, mapKey, descriptorSize;
    uint32_t descriptorVersion;
} MemoryMap;

void get_memory_map(MemoryMap *memory_map) {
    memory_map->memoryMapSize = 0;
    memory_map->memoryMapDescriptor = NULL;
    EFI_STATUS status = st->BootServices->EFI_GET_MEMORY_MAP(&memory_map->memoryMapSize,
                                                             memory_map->memoryMapDescriptor,
                                                             &memory_map->mapKey, &memory_map->descriptorSize,
                                                             &memory_map->descriptorVersion);
    do
        while (status == EFI_BUFFER_TOO_SMALL) {
            if (memory_map->memoryMapDescriptor != 0)
                st->BootServices->EFI_FREE_POOL(memory_map->memoryMapDescriptor);
            st->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, memory_map->memoryMapSize,
                                                (void **) &memory_map->memoryMapDescriptor);
            status = st->BootServices->EFI_GET_MEMORY_MAP(&memory_map->memoryMapSize, memory_map->memoryMapDescriptor,
                                                          &memory_map->mapKey,
                                                          &memory_map->descriptorSize,
                                                          &memory_map->descriptorVersion);
        }
    while (status != EFI_SUCCESS);

}

void exit_boot_services(void *image_handle) {
    MemoryMap memory_map;
    EFI_STATUS status;
    do {
        get_memory_map(&memory_map);
        status = st->BootServices->EFI_EXIT_BOOT_SERVICES(image_handle, memory_map.mapKey);
    } while (status != EFI_SUCCESS);
}

void set_boot_menu(MenuList *menu_list) {
    uint8_t num_menus = 3;
    static Menu explore_file_menu = {L"ExploreFile", explore_file},
            option_menu = {L"Option", option},
            shutdown_menu = {L"Shutdown", shutdown};
    Menu *menus[] = {&explore_file_menu, &option_menu, &shutdown_menu};
    menu_list->cursor = 0;
    for (int i = 0; i < num_menus; ++i)
        menu_list->menus[i] = menus[i];
    menu_list->numMenus = num_menus;
}

EFI_STATUS efi_main(void *image_handle, EFI_SYSTEM_TABLE *system_table) {
    st = system_table;
    st->BootServices->EFI_SET_WATCHDOG_TIMER(0, 0, 0, NULL);


    MenuList menu_list;
    menu_init(system_table);
    set_boot_menu(&menu_list);
    boot_menu(&menu_list);
    return EFI_SUCCESS;
}