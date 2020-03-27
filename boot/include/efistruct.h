#ifndef EGOISTICOS_EFISTRUCT_H
#define EGOISTICOS_EFISTRUCT_H

#include "efidef.h"
#include "eficonst.h"

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
//    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
//    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode;
//    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt;
    UINT64 _buf[3];
    struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
        UINT32 MaxMode;
        UINT32 Mode;
        struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
            UINT32 Version;
            UINT32 HorizontalResolution;
            UINT32 VerticalResolution;
            enum EFI_GRAPHICS_PIXEL_FORMAT {
                PixelRedGreenBlueReserved8BitPerColor,
                PixelBlueGreenRedReserved8BitPerColor,
                PixelBitMask,
                PixelBltOnly,
                PixelFormatMax
            } PixelFormat;
            struct EFI_PIXEL_BITMASK {
                UINT32 RedMask;
                UINT32 GreenMask;
                UINT32 BlueMask;
                UINT32 ReservedMask;
            } PixelInformation;
            UINT32 PixelsPerScanLine;
        } *Info;
        UINTN SizeOfInfo;
        EFI_PHYSICAL_ADDRESS FrameBufferBase;
        UINTN FrameBufferSize;
    } *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct {
    uint8_t _buf[60];

    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
        EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN BOOLEAN ExtendedVerification
        );

        EFI_STATUS (EFIAPI *OUT_PUT_STRING)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN UINT16 *String

        );

        EFI_STATUS (EFIAPI *EFI_TEXT_TEST_STRING)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN CHAR16 *String
        );

        EFI_STATUS (EFIAPI *EFI_TEXT_QUERY_MODE)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN UINTN ModeNumber,
                OUT UINTN *Columns,
                OUT UINTN *Rows
        );

        EFI_STATUS (EFIAPI *EFI_TEXT_SET_MODE)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN UINTN ModeNumber
        );

        EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN UINTN Attribute
        );

        EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
        );

        EFI_STATUS (EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN UINTN Column,
                IN UINTN Row
        );

        EFI_STATUS (EFIAPI *EFI_TEXT_ENABLE_CURSOR)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN BOOLEAN Visible
        );

        struct SIMPLE_TEXT_OUTPUT_MODE {
            INT32 MaxMode;
            INT32 Mode;
            INT32 Attribute;
            INT32 CursorColumn;
            INT32 CursorRow;
            BOOLEAN CursorVisible;
        } *Mode;
    } *ConOut;
} EFI_SYSTEM_TABLE;

#endif //EGOISTICOS_EFISTRUCT_H
