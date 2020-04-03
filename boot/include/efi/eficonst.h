#ifndef EGOISTICOS_EFICONST_H
#define EGOISTICOS_EFICONST_H

#include "efidef.h"

//*******************************************************
// Event Types
//*******************************************************
#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_NOTIFY_SIGNAL 0x00000100
#define EVT_NOTIFY_WAIT 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

//*******************************************************
// UNICODE DRAWING CHARACTERS
//*******************************************************
#define BOXDRAW_HORIZONTAL 0x2500
#define BOXDRAW_VERTICAL 0x2502
#define BOXDRAW_DOWN_RIGHT 0x250c
#define BOXDRAW_DOWN_LEFT 0x2510
#define BOXDRAW_UP_RIGHT 0x2514
#define BOXDRAW_UP_LEFT 0x2518
#define BOXDRAW_VERTICAL_RIGHT 0x251c
#define BOXDRAW_VERTICAL_LEFT 0x2524
#define BOXDRAW_DOWN_HORIZONTAL 0x252c
#define BOXDRAW_UP_HORIZONTAL 0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL 0x253c

#define BOXDRAW_DOUBLE_HORIZONTAL 0x2550
#define BOXDRAW_DOUBLE_VERTICAL 0x2551
#define BOXDRAW_DOWN_RIGHT_DOUBLE 0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT 0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT 0x2554
#define BOXDRAW_DOWN_LEFT_DOUBLE 0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT 0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT 0x2557

#define BOXDRAW_UP_RIGHT_DOUBLE 0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT 0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT 0x255a

#define BOXDRAW_VERTICAL_RIGHT_DOUBLE 0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT 0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT 0x2560

#define BOXDRAW_VERTICAL_LEFT_DOUBLE 0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT 0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT 0x2563

#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE 0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL 0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL 0x2566

#define BOXDRAW_UP_HORIZONTAL_DOUBLE 0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL 0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL 0x2569

#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE 0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL 0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL 0x256c

//*******************************************************
// EFI Required Block Elements Code Chart
//*******************************************************
#define BLOCKELEMENT_FULL_BLOCK 0x2588
#define BLOCKELEMENT_LIGHT_SHADE 0x2591

//*******************************************************
// EFI Required Geometric Shapes Code Chart
//*******************************************************
#define GEOMETRICSHAPE_UP_TRIANGLE 0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE 0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE 0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE 0x25c4

//*******************************************************
// EFI Required Arrow shapes
//*******************************************************
#define ARROW_UP 0x2191
#define ARROW_DOWN 0x2193

//*******************************************************
// EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL Attributes
//*******************************************************
#define EFI_BLACK 0x00
#define EFI_BLUE 0x01
#define EFI_GREEN 0x02
#define EFI_CYAN 0x03
#define EFI_RED 0x04
#define EFI_MAGENTA 0x05
#define EFI_BROWN 0x06
#define EFI_LIGHTGRAY 0x07
#define EFI_BRIGHT 0x08
#define EFI_DARKGRAY (EFI_BLACK | EFI_BRIGHT)
#define EFI_LIGHTBLUE 0x09
#define EFI_LIGHTGREEN 0x0A
#define EFI_LIGHTCYAN 0x0B
#define EFI_LIGHTRED 0x0C
#define EFI_LIGHTMAGENTA 0x0D
#define EFI_YELLOW 0x0E
#define EFI_WHITE 0x0F
#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_BLUE 0x10
#define EFI_BACKGROUND_GREEN 0x20
#define EFI_BACKGROUND_CYAN 0x30
#define EFI_BACKGROUND_RED 0x40
#define EFI_BACKGROUND_MAGENTA 0x50
#define EFI_BACKGROUND_BROWN 0x60
#define EFI_BACKGROUND_LIGHTGRAY 0x70

//*******************************************************
//EFI_STATUS Success Codes (High Bit Clear)
//*******************************************************
#define EFI_SUCCESS 0

//*******************************************************
//EFI_STATUS Error Codes (High Bit Set)
//*******************************************************
#define EFI_ERROR 0x8000000000000000U

#define EFI_LOAD_ERROR (EFI_ERROR | 1U)
#define EFI_INVALID_PARAMETER (EFI_ERROR | 2U)
#define EFI_UNSUPPORTED (EFI_ERROR | 3U)
#define EFI_BAD_BUFFER_SIZE (EFI_ERROR | 4U)
#define EFI_BUFFER_TOO_SMALL (EFI_ERROR | 5U)
#define EFI_NOT_READY (EFI_ERROR | 6U)
#define EFI_DEVICE_ERROR (EFI_ERROR | 7U)
#define EFI_WRITE_PROTECTED (EFI_ERROR | 8U)
#define EFI_OUT_OF_RESOURCES (EFI_ERROR | 9U)
#define EFI_VOLUME_CORRUPTED (EFI_ERROR | 10U)
#define EFI_VOLUME_FULL (EFI_ERROR | 11U)
#define EFI_NO_MEDIA (EFI_ERROR | 12U)
#define EFI_MEDIA_CHANGED (EFI_ERROR | 13U)
#define EFI_NOT_FOUND (EFI_ERROR | 14U)
#define EFI_ACCESS_DENIED (EFI_ERROR | 15U)
#define EFI_NO_RESPONSE (EFI_ERROR | 16U)
#define EFI_NO_MAPPING (EFI_ERROR | 17U)
#define EFI_TIMEOUT (EFI_ERROR | 18U)
#define EFI_NOT_STARTED (EFI_ERROR | 19U)
#define EFI_ALREADY_STARTED (EFI_ERROR | 20U)
#define EFI_ABORTED (EFI_ERROR | 21U)
#define EFI_ICMP_ERROR (EFI_ERROR | 22U)
#define EFI_TFTP_ERROR (EFI_ERROR | 23U)
#define EFI_PROTOCOL_ERROR (EFI_ERROR | 24U)
#define EFI_INCOMPATIBLE_VERSION (EFI_ERROR | 25U)
#define EFI_SECURITY_VIOLATION (EFI_ERROR | 26U)
#define EFI_CRC_ERROR (EFI_ERROR | 27U)
#define EFI_END_OF_MEDIA (EFI_ERROR | 28U)
#define EFI_END_OF_FILE (EFI_ERROR | 31U)
#define EFI_INVALID_LANGUAGE (EFI_ERROR | 32U)
#define EFI_COMPROMISED_DATA (EFI_ERROR | 33U)
#define EFI_IP_ADDRESS_CONFLICT (EFI_ERROR | 34U)
#define EFI_HTTP_ERROR (EFI_ERROR | 35U)

//*******************************************************
//EFI Scan Codes
//*******************************************************
#define EFI_SCAN_CODE_UP 0x01
#define EFI_SCAN_CODE_DOWN 0x02
#define EFI_SCAN_CODE_PAGE_UP 0x09
#define EFI_SCAN_CODE_PAGE_DOWN 0x0a
#define EFI_SCAN_CODE_ESCAPE 0x17


//*******************************************************
// Open Modes
//*******************************************************
#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

//*******************************************************
// File Attribute Bits
//*******************************************************
#define EFI_FILE_READ_ONLY 0x0000000000000001
#define EFI_FILE_HIDDEN 0x0000000000000002
#define EFI_FILE_SYSTEM 0x0000000000000004
#define EFI_FILE_RESERVED 0x0000000000000008
#define EFI_FILE_DIRECTORY 0x0000000000000010
#define EFI_FILE_ARCHIVE 0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

//*******************************************************
// System Table Revision
//*******************************************************
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2U<<16U) | (70U))
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2U<<16U) | (60U))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2U<<16U) | (40U))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2U<<16U) | (50U))
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2U<<16U) | (31U))
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2U<<16U) | (30U))
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2U<<16U) | (20U))
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2U<<16U) | (10U))
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2U<<16U) | (00U))
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1U<<16U) | (10U))
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1U<<16U) | (02U))

//*******************************************************
// Memory Attribute Definitions
//*******************************************************
#define EFI_MEMORY_UC 0x0000000000000001
#define EFI_MEMORY_WC 0x0000000000000002
#define EFI_MEMORY_WT 0x0000000000000004
#define EFI_MEMORY_WB 0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010
#define EFI_MEMORY_WP 0x0000000000001000
#define EFI_MEMORY_RP 0x0000000000002000
#define EFI_MEMORY_XP 0x0000000000004000
#define EFI_MEMORY_NV 0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO 0x0000000000020000
#define EFI_MEMORY_RUNTIME 0x8000000000000000

#endif //EGOISTICOS_EFICONST_H
