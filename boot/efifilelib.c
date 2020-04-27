#include "include/efifilelib.h"

#include "include/efi/efistruct.h"
#include "include/efi/efiguid.h"
#include "include/efisystab.h"
#include "include/efilocatedprotocol.h"
#include "include/utils.h"

#define MAX_FILE_NAME_SIZE 255

void get_root_file_protocol(EFI_FILE_PROTOCOL **root) {
    check_error(simple_file_system_protocol->EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME(simple_file_system_protocol,
                                                                                         root), L"OpenVolume");
}

EFI_FILE_INFO *get_file_info(CHAR16 *file_name) {
    EFI_FILE_PROTOCOL *root;
    get_root_file_protocol(&root);
    EFI_FILE_PROTOCOL *file_protocol;
    check_error(root->EFI_FILE_OPEN(root, &file_protocol, file_name, EFI_FILE_MODE_READ, 0), L"read_file_to_address");
    EFI_GUID file_info_guid = EFI_FILE_INFO_ID;
    UINTN buffer_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * MAX_FILE_NAME_SIZE;
    EFI_FILE_INFO *file_info;
    system_table->BootServices->EFI_ALLOCATE_POOL(EfiLoaderData, buffer_size, (void **) &file_info);
    check_error(file_protocol->EFI_FILE_GET_INFO(file_protocol, &file_info_guid, &buffer_size, file_info),
                L"FileGetInfo");
    root->EFI_FILE_CLOSE(root);
    file_protocol->EFI_FILE_CLOSE(file_protocol);
    return file_info;
}

void read_file_to_address(uint64_t address, CHAR16 *file_name) {
    EFI_FILE_PROTOCOL *root;
    get_root_file_protocol(&root);
    EFI_FILE_PROTOCOL *file;
    check_error(root->EFI_FILE_OPEN(root, &file, file_name, EFI_FILE_MODE_READ, 0), L"read_file_to_address");
    EFI_FILE_INFO *file_info = get_file_info(file_name);
    uint64_t file_size = file_info->FileSize;
    system_table->BootServices->EFI_FREE_POOL(file_info);
    file->EFI_FILE_READ(file, &file_size, (void *) address);
    file->EFI_FILE_CLOSE(root);
}
