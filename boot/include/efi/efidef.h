#ifndef EGOISTICOS_EFIDEF_H
#define EGOISTICOS_EFIDEF_H

#include <int.h>

#define EFIAPI
#define OUT
#define IN
#define OPTIONAL
#define CONST const

typedef uint8_t BOOLEAN;
typedef int64_t INTN;
typedef uint64_t UINTN;
typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;
typedef int8_t CHAR8;
typedef uint16_t CHAR16;
typedef void VOID;
typedef UINTN EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void *EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN EFI_TPL;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

#endif //EGOISTICOS_EFIDEF_H
