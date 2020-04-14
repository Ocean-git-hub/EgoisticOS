#ifndef EGOISTICOS_EFISTRUCT_H
#define EGOISTICOS_EFISTRUCT_H

#include "efidef.h"
#include "eficonst.h"

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
} EFI_GUID;

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
    UINT16 ScanCode;
    CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct {
    UINT16 Year;
    UINT8 Month;
    UINT8 Day;
    UINT8 Hour;
    UINT8 Minute;
    UINT8 Second;
    UINT8 Pad1;
    UINT32 Nanosecond;
    INT16 TimeZone;
    UINT8 Daylight;
    UINT8 Pad2;
} EFI_TIME;

typedef struct {
    UINT32 Resolution;
    UINT32 Accuracy;
    BOOLEAN SetsToZero;
} EFI_TIME_CAPABILITIES;

typedef struct {
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef struct {
    EFI_GUID CapsuleGuid;
    UINT32 HeaderSize;
    UINT32 Flags;
    UINT32 CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef EFI_VOID(EFIAPI *EFI_EVENT_NOTIFY)(
        IN EFI_EVENT Event,
        IN EFI_VOID *Context
);

typedef enum {
    TimerCancel,
    TimerPeriodic,
    TimerRelative
} EFI_TIMER_DELAY;

typedef enum {
    EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef struct _EFI_DEVICE_PATH_PROTOCOL {
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef struct {
    EFI_HANDLE AgentHandle;
    EFI_HANDLE ControllerHandle;
    UINT32 Attributes;
    UINT32 OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef struct {
    EFI_TABLE_HEADER Hdr;
    CHAR16 *FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;

    struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
        EFI_STATUS (EFIAPI *EFI_INPUT_RESET)(
                IN struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
                IN BOOLEAN ExtendedVerification
        );

        EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY)(
                IN struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
                OUT EFI_INPUT_KEY *Key
        );

        EFI_EVENT WaitForKey;
    } *ConIn;

    EFI_HANDLE ConsoleOutHandle;

    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
        EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN BOOLEAN ExtendedVerification
        );

        EFI_STATUS (EFIAPI *EFI_OUT_PUT_STRING)(
                IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                IN CHAR16 *String
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

    EFI_HANDLE StandardErrorHandle;
    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;

    struct EFI_RUNTIME_SERVICES {
        EFI_TABLE_HEADER Hdr;

        EFI_STATUS (EFIAPI *EFI_GET_TIME)(
                OUT EFI_TIME *Time,
                OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
        );

        EFI_STATUS (EFIAPI *SET_TIME)(
                IN EFI_TIME *Time
        );

        EFI_STATUS (EFIAPI *GET_WAKEUP_TIME)(
                OUT BOOLEAN *Enabled,
                OUT BOOLEAN *Pending,
                OUT EFI_TIME *Time
        );

        EFI_STATUS (EFIAPI *SET_WAKEUP_TIME)(
                IN BOOLEAN Enable,
                IN EFI_TIME *Time OPTIONAL
        );

        EFI_STATUS (EFIAPI *SET_VIRTUAL_ADDRESS_MAP)(
                IN UINTN MemoryMapSize,
                IN UINTN DescriptorSize,
                IN UINT32 DescriptorVersion,
                IN EFI_MEMORY_DESCRIPTOR *VirtualMap
        );

        EFI_STATUS (EFIAPI *CONVERT_POINTER)(
                IN UINTN DebugDisposition,
                IN EFI_VOID **Address
        );

        EFI_STATUS (EFIAPI *GET_VARIABLE)(
                IN CHAR16 *VariableName,
                IN EFI_GUID *VendorGuid,
                OUT UINT32 *Attributes OPTIONAL,
                IN OUT UINTN *DataSize,
                OUT EFI_VOID *Data OPTIONAL
        );

        EFI_STATUS (EFIAPI *GET_NEXT_VARIABLE_NAME)(
                IN OUT UINTN *VariableNameSize,
                IN OUT CHAR16 *VariableName,
                IN OUT EFI_GUID *VendorGuid
        );


        EFI_STATUS (EFIAPI *GetNextVariableName)(
                IN OUT UINTN *VariableNameSize,
                IN OUT CHAR16 *VariableName,
                IN OUT EFI_GUID *VendorGuid
        );


        EFI_STATUS (EFIAPI *GET_NEXT_HIGH_MONOTONIC_COUNT)(
                OUT UINT32 *HighCount
        );

        EFI_VOID (EFIAPI *EFI_RESET_SYSTEM)(
                IN EFI_RESET_TYPE ResetType,
                IN EFI_STATUS ResetStatus,
                IN UINTN DataSize,
                IN EFI_VOID *ResetData OPTIONAL
        );

        EFI_STATUS (EFIAPI *UPDATE_CAPSULE)(
                IN EFI_CAPSULE_HEADER **CapsuleHeaderArray,
                IN UINTN CapsuleCount,
                IN EFI_PHYSICAL_ADDRESS ScatterGatherList OPTIONAL
        );

        EFI_STATUS (EFIAPI *QUERY_CAPSULE_CAPABILITIES)(
                IN EFI_CAPSULE_HEADER **CapsuleHeaderArray,
                IN UINTN CapsuleCount,
                OUT UINT64 *MaximumCapsuleSize,
                OUT EFI_RESET_TYPE *ResetType
        );

        EFI_STATUS (EFIAPI *QUERY_VARIABLE_INFO)(
                IN UINT32 Attributes,
                OUT UINT64 *MaximumVariableStorageSize,
                OUT UINT64 *RemainingVariableStorageSize,
                OUT UINT64 *MaximumVariableSize
        );
    } *RuntimeServices;

    struct EFI_BOOT_SERVICES {
        EFI_TABLE_HEADER Hdr;

        EFI_TPL (EFIAPI *EFI_RAISE_TPL)(
                IN EFI_TPL NewTpl
        );

        EFI_VOID (EFIAPI *EFI_RESTORE_TPL)(
                IN EFI_TPL OldTpl
        );

        EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES)(
                IN EFI_ALLOCATE_TYPE Type,
                IN EFI_MEMORY_TYPE MemoryType,
                IN UINTN Pages,
                IN OUT EFI_PHYSICAL_ADDRESS *Memory
        );

        EFI_STATUS (EFIAPI *EFI_FREE_PAGES)(
                IN EFI_PHYSICAL_ADDRESS Memory,
                IN UINTN Pages
        );

        EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP)(
                IN OUT UINTN *MemoryMapSize,
                IN OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
                OUT UINTN *MapKey,
                OUT UINTN *DescriptorSize,
                OUT UINT32 *DescriptorVersion
        );

        EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL)(
                IN EFI_MEMORY_TYPE PoolType,
                IN UINTN Size,
                OUT EFI_VOID **Buffer
        );

        EFI_STATUS (EFIAPI *EFI_FREE_POOL)(
                IN EFI_VOID *Buffer
        );

        EFI_STATUS (EFIAPI *EFI_CREATE_EVENT)(
                IN UINT32 Type,
                IN EFI_TPL NotifyTpl,
                IN EFI_EVENT_NOTIFY NotifyFunction, OPTIONAL
                IN EFI_VOID *NotifyContext, OPTIONAL
                OUT EFI_EVENT *Event
        );

        EFI_STATUS (EFIAPI *EFI_SET_TIMER)(
                IN EFI_EVENT Event,
                IN EFI_TIMER_DELAY Type,
                IN UINT64 TriggerTime
        );

        EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT)(
                IN UINTN NumberOfEvents,
                IN EFI_EVENT *Event,
                OUT UINTN *Index
        );

        EFI_STATUS (EFIAPI *EFI_SIGNAL_EVENT)(
                IN EFI_EVENT Event
        );

        EFI_STATUS (EFIAPI *EFI_CLOSE_EVENT)(
                IN EFI_EVENT Event
        );

        EFI_STATUS (EFIAPI *EFI_CHECK_EVENT)(
                IN EFI_EVENT Event
        );

        EFI_STATUS (EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(
                IN OUT EFI_HANDLE *Handle,
                IN EFI_GUID *Protocol,
                IN EFI_INTERFACE_TYPE InterfaceType,
                IN EFI_VOID *Interface
        );

        EFI_STATUS (EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE)(
                IN EFI_HANDLE Handle,
                IN EFI_GUID *Protocol,
                IN EFI_VOID *OldInterface,
                IN EFI_VOID *NewInterface
        );

        EFI_STATUS (EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE)(
                IN EFI_HANDLE Handle,
                IN EFI_GUID *Protocol,
                IN EFI_VOID *Interface
        );

        EFI_STATUS (EFIAPI *EFI_HANDLE_PROTOCOL)(
                IN EFI_HANDLE Handle,
                IN EFI_GUID *Protocol,
                OUT EFI_VOID **Interface
        );

        EFI_VOID *Reserved;

        EFI_STATUS (EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY)(
                IN EFI_GUID *Protocol,
                IN EFI_EVENT Event,
                OUT EFI_VOID **Registration
        );

        EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE)(
                IN EFI_LOCATE_SEARCH_TYPE SearchType,
                IN EFI_GUID *Protocol OPTIONAL,
                IN EFI_VOID *SearchKey OPTIONAL,
                IN OUT UINTN *BufferSize,
                OUT EFI_HANDLE *Buffer
        );

        EFI_STATUS (EFIAPI *EFI_LOCATE_DEVICE_PATH)(
                IN EFI_GUID *Protocol,
                IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
                OUT EFI_HANDLE *Device
        );

        EFI_STATUS (EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE)(
                IN EFI_GUID *Guid,
                IN EFI_VOID *Table
        );

        EFI_STATUS (EFIAPI *EFI_IMAGE_LOAD)(
                IN BOOLEAN BootPolicy,
                IN EFI_HANDLE ParentImageHandle,
                IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
                IN EFI_VOID *SourceBuffer OPTIONAL,
                IN UINTN SourceSize,
                OUT EFI_HANDLE *ImageHandle
        );

        EFI_STATUS (EFIAPI *EFI_IMAGE_START)(
                IN EFI_HANDLE ImageHandle,
                OUT UINTN *ExitDataSize,
                OUT CHAR16 **ExitData OPTIONAL
        );

        EFI_STATUS (EFIAPI *EFI_EXIT)(
                IN EFI_HANDLE ImageHandle,
                IN EFI_STATUS ExitStatus,
                IN UINTN ExitDataSize,
                IN CHAR16 *ExitData OPTIONAL
        );

        EFI_STATUS (EFIAPI *EFI_IMAGE_UNLOAD)(
                IN EFI_HANDLE ImageHandle
        );

        EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES)(
                IN EFI_HANDLE ImageHandle,
                IN UINTN MapKey
        );

        EFI_STATUS (EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT)(
                OUT UINT64 *Count
        );

        EFI_STATUS (EFIAPI *EFI_STALL)(
                IN UINTN Microseconds
        );

        EFI_STATUS (EFIAPI *EFI_SET_WATCHDOG_TIMER)(
                IN UINTN Timeout,
                IN UINT64 WatchdogCode,
                IN UINTN DataSize,
                IN CHAR16 *WatchdogData OPTIONAL
        );

        EFI_STATUS (EFIAPI *EFI_CONNECT_CONTROLLER)(
                IN EFI_HANDLE ControllerHandle,
                IN EFI_HANDLE *DriverImageHandle OPTIONAL,
                IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL,
                IN BOOLEAN Recursive
        );

        EFI_STATUS (EFIAPI *EFI_DISCONNECT_CONTROLLER)(
                IN EFI_HANDLE ControllerHandle,
                IN EFI_HANDLE DriverImageHandle OPTIONAL,
                IN EFI_HANDLE ChildHandle
                OPTIONAL
        );

        EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL)(
                IN EFI_HANDLE Handle,
                IN EFI_GUID *Protocol,
                OUT EFI_VOID **Interface OPTIONAL,
                IN EFI_HANDLE AgentHandle,
                IN EFI_HANDLE ControllerHandle,
                IN UINT32 Attributes
        );

        EFI_STATUS (EFIAPI *EFI_CLOSE_PROTOCOL)(
                IN EFI_HANDLE Handle,
                IN EFI_GUID *Protocol,
                IN EFI_HANDLE AgentHandle,
                IN EFI_HANDLE ControllerHandle
        );

        EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION)(
                IN EFI_HANDLE Handle,
                IN EFI_GUID *Protocol,
                OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
                OUT UINTN *EntryCount
        );

        EFI_STATUS (EFIAPI *EFI_PROTOCOLS_PER_HANDLE)(
                IN EFI_HANDLE Handle,
                OUT EFI_GUID ***ProtocolBuffer,
                OUT UINTN *ProtocolBufferCount
        );

        EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(
                IN EFI_LOCATE_SEARCH_TYPE SearchType,
                IN EFI_GUID *Protocol OPTIONAL,
                IN EFI_VOID *SearchKey OPTIONAL,
                IN OUT UINTN *NoHandles,
                OUT EFI_HANDLE **Buffer
        );

        EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL)(
                IN EFI_GUID *Protocol,
                IN EFI_VOID *Registration   OPTIONAL,
                OUT EFI_VOID **Interface
        );

        EFI_STATUS (EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
                IN OUT EFI_HANDLE *Handle,
                ...
        );

        EFI_STATUS (EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
                IN EFI_HANDLE Handle,
                ...
        );

        EFI_STATUS (EFIAPI *EFI_CALCULATE_CRC32)(
                IN EFI_VOID *Data,
                IN UINTN
                DataSize,
                OUT UINT32 *Crc32
        );

        EFI_VOID (EFIAPI *EFI_COPY_MEM)(
                IN EFI_VOID *Destination,
                IN EFI_VOID *Source,
                IN UINTN Length
        );

        EFI_VOID (EFIAPI *EFI_SET_MEM)(
                IN EFI_VOID *Buffer,
                IN UINTN Size,
                IN UINT8 Value
        );

        EFI_STATUS (EFIAPI *EFI_CREATE_EVENT_EX)(
                IN UINT32 Type,
                IN EFI_TPL NotifyTpl,
                IN EFI_EVENT_NOTIFY NotifyFunction OPTIONAL,
                IN CONST EFI_VOID *NotifyContext OPTIONAL,
                IN CONST EFI_GUID *EventGroup OPTIONAL,
                OUT EFI_EVENT *Event
        );
    } *BootServices;

    UINTN NumberOfTableEntries;
    struct EFI_CONFIGURATION_TABLE {
        EFI_GUID VendorGuid;
        EFI_VOID *VendorTable;
    } *ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct _EFI_DEVICE_PATH_TO_TEXT_PROTOCOL {
    CHAR16 *(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_NODE)(
            IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode,
            IN BOOLEAN DisplayOnly,
            IN BOOLEAN AllowShortcuts
    );

    CHAR16 *(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_PATH)(
            IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
            IN BOOLEAN DisplayOnly,
            IN BOOLEAN AllowShortcuts
    );

} EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

typedef struct {
    EFI_EVENT Event;
    EFI_STATUS Status;
    UINTN BufferSize;
    EFI_VOID *Buffer;
} EFI_FILE_IO_TOKEN;

typedef struct _EFI_FILE_PROTOCOL {
    UINT64 Revision;

    EFI_STATUS (EFIAPI *EFI_FILE_OPEN)(
            IN struct _EFI_FILE_PROTOCOL *This,
            OUT struct _EFI_FILE_PROTOCOL **NewHandle,
            IN CHAR16 *FileName,
            IN UINT64 OpenMode,
            IN UINT64 Attributes
    );

    EFI_STATUS (EFIAPI *EFI_FILE_CLOSE)(
            IN struct _EFI_FILE_PROTOCOL *This
    );

    EFI_STATUS (EFIAPI *EFI_FILE_DELETE)(
            IN struct _EFI_FILE_PROTOCOL *This
    );

    EFI_STATUS (EFIAPI *EFI_FILE_READ)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN OUT UINTN *BufferSize,
            OUT EFI_VOID *Buffer
    );

    EFI_STATUS (EFIAPI *EFI_FILE_WRITE)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN OUT UINTN *BufferSize,
            IN EFI_VOID *Buffer
    );

    EFI_STATUS (EFIAPI *EFI_FILE_GET_POSITION)(
            IN struct _EFI_FILE_PROTOCOL *This,
            OUT UINT64 *Position
    );

    EFI_STATUS (EFIAPI *EFI_FILE_SET_POSITION)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN UINT64 Position
    );

    EFI_STATUS (EFIAPI *EFI_FILE_GET_INFO)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN EFI_GUID *InformationType,
            IN OUT UINTN *BufferSize,
            OUT EFI_VOID *Buffer
    );

    EFI_STATUS (EFIAPI *EFI_FILE_SET_INFO)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN EFI_GUID *InformationType,
            IN UINTN BufferSize,
            IN EFI_VOID *Buffer
    );

    EFI_STATUS (EFIAPI *EFI_FILE_FLUSH)(
            IN struct _EFI_FILE_PROTOCOL *This
    );

    EFI_STATUS (EFIAPI *EFI_FILE_OPEN_EX)(
            IN struct _EFI_FILE_PROTOCOL *This,
            OUT struct _EFI_FILE_PROTOCOL **NewHandle,
            IN CHAR16 *FileName,
            IN UINT64 OpenMode,
            IN UINT64 Attributes,
            IN OUT EFI_FILE_IO_TOKEN *Token
    );

    EFI_STATUS (EFIAPI *EFI_FILE_READ_EX)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN OUT EFI_FILE_IO_TOKEN *Token
    );

    EFI_STATUS (EFIAPI *EFI_FILE_WRITE_EX)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN OUT EFI_FILE_IO_TOKEN *Token
    );

    EFI_STATUS (EFIAPI *EFI_FILE_FLUSH_EX)(
            IN struct _EFI_FILE_PROTOCOL *This,
            IN OUT EFI_FILE_IO_TOKEN *Token
    );

} EFI_FILE_PROTOCOL;

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    UINT64 Revision;

    EFI_STATUS (EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(
            IN struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
            OUT EFI_FILE_PROTOCOL **Root
    );

} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef struct {
    UINT64 Size;
    UINT64 FileSize;
    UINT64 PhysicalSize;
    EFI_TIME CreateTime;
    EFI_TIME LastAccessTime;
    EFI_TIME ModificationTime;
    UINT64 Attribute;
    CHAR16 FileName[];
} EFI_FILE_INFO;

#endif //EGOISTICOS_EFISTRUCT_H
