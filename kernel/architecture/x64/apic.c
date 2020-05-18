#include <architecture/x64/apic.h>

#include <stdbool.h>
#include <architecture/x64/msr.h>
#include <architecture/x64/pic.h>
#include <graphics.h>

#define CPUID_EAX1_X2APIC_SUPPORT_BIT 0x200000U //bit 21

#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LOCAL_APIC_ID 0x20
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LOCAL_APIC_VERSION 0x30
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_TASK_PRIORITY 0x80
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_ARBITRATION_PRIORITY 0x90
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_PROCESSOR_PRIORITY 0xa0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_EOI 0xb0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_REMOTE_READ 0xc0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LOGICAL_DESTINATION 0xd0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_DESTINATION_FORMAT 0xe0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_SPURIOUS_INTERRUPT_VECTOR 0xf0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_IN_SERVICE_BASE 0x100
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_TRIGGER_MODE_BASE 0x180
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_INTERRUPT_REQUEST_BASE 0x200
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_ERROR_STATUS 0x280
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_CORRECTED_MACHINE_CHECK_INTERRUPT 0x2f0
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_INTERRUPT_COMMAND_BASE 0x300
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_TIMER 0x320
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_THERMAL_SENSOR 0x330
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_PERFORMANCE_MONITORING_COUNTERS 0x340
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_LINT0 0x350
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_LINT1 0x360
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_LVT_ERROR 0x370
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_INITIAL_COUNT 0x380
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_CURRENT_COUNT 0x390
#define LOCAL_APIC_REGISTER_ADDRESS_MAP_DIVIDE_CONFIGURATION 0x3e0

#define IO_APIC_DIRECT_REGISTERS_ADDRESS_INDEX_OFFSET 0
#define IO_APIC_DIRECT_REGISTERS_ADDRESS_DATA_OFFSET 0x10
#define IO_APIC_DIRECT_REGISTERS_ADDRESS_EOI_OFFSET 0x40

#define IO_APIC_INDIRECT_REGISTERS_INDEX_ID 0
#define IO_APIC_INDIRECT_REGISTERS_INDEX_VERSION 0x1
#define IO_APIC_INDIRECT_REGISTERS_INDEX_REDIRECTION_TABLE_BASE 0x10

MADT *madt;
uint32_t local_apic_base;
uint32_t io_apic_address;

typedef enum {
    ProcessorLocalAPIC,
    IOAPIC,
    InterruptSourceOverride,
    NonMaskableInterruptSource,
    LocalAPICNMI,
    LocalAPICAddressOverride,
    IOSAPIC,
    LocalSAPIC,
    PlatformInterruptSources,
    ProcessorLocalx2APIC,
    Localx2APICNMI,
    GICCPUInterface,
    GICDistributor,
    GICMSIFrame,
    GICRedistributor,
    GICInterruptTranslationService,
    Reserved
} InterruptControllerStructureTypes;

typedef struct {
    uint8_t type;
    uint8_t length;
} InterruptControllerStructureHeader;

typedef struct {
    InterruptControllerStructureHeader header;
    uint8_t ioAPICID;
    uint8_t reserved;
    uint32_t ioAPICAddress;
    uint32_t globalSystemInterruptBase;
} IOAPICStructure;

typedef union __attribute__((packed)) {
    uint64_t bits;
    struct {
        uint8_t vector;
        uint8_t deliveryMode: 3;
        uint8_t destinationMode: 1;
        uint8_t deliveryStatus: 1;
        uint8_t interruptInputPinPolarity: 1;
        uint8_t remoteIRRl: 1;
        uint8_t triggerMode: 1;
        uint32_t mask: 1;
        uint32_t reserved: 31;
        uint8_t extendedDestinationID;
        union {
            uint8_t destination;
            union {
                uint8_t physical: 4;
                uint8_t logical;
            } Destination;
        };
    };
} RedirectionTableRegisterLayout;

bool is_support_x2apic() {
    uint32_t ecx, edx;
    __asm__ volatile ("mov $1, %%eax\n"
                      "cpuid\n":"=c"(ecx), "=d"(edx));
    return ecx & CPUID_EAX1_X2APIC_SUPPORT_BIT;
}

uint32_t read_io_apic(uint8_t index) {
    *(uint32_t *) (uint64_t) (io_apic_address + IO_APIC_DIRECT_REGISTERS_ADDRESS_INDEX_OFFSET) = index;
    return *(uint32_t *) (uint64_t) (io_apic_address + IO_APIC_DIRECT_REGISTERS_ADDRESS_DATA_OFFSET);
}

void write_io_apic(uint8_t index, uint32_t data) {
    *(uint32_t *) (uint64_t) (io_apic_address + IO_APIC_DIRECT_REGISTERS_ADDRESS_INDEX_OFFSET) = index;
    *(uint32_t *) (uint64_t) (io_apic_address + IO_APIC_DIRECT_REGISTERS_ADDRESS_DATA_OFFSET) = data;
}

void
set_redirection_table_register(uint8_t irq_no, uint8_t vector, uint8_t trigger, uint8_t mask, uint8_t destination) {
    RedirectionTableRegisterLayout data = {};
    data.vector = vector;
    data.triggerMode = trigger;
    data.mask = mask;
    data.Destination.physical = destination;
    write_io_apic(IO_APIC_INDIRECT_REGISTERS_INDEX_REDIRECTION_TABLE_BASE + irq_no * 2, data.bits);
    write_io_apic(IO_APIC_INDIRECT_REGISTERS_INDEX_REDIRECTION_TABLE_BASE + irq_no * 2 + 1, data.bits >> 32U);
}

void set_apic_irq(bool is_enable, uint8_t irq_no) {
    uint8_t vector = irq_no;
    irq_no -= INTERRUPT_NUMBER_BASE;
    if (is_enable)
        set_redirection_table_register(irq_no, vector, 0, 0, 0);
    else
        set_redirection_table_register(irq_no, vector, 0, 1, 0);
}

void send_apic_eoi() {
    *(uint32_t *) (uint64_t) (local_apic_base + LOCAL_APIC_REGISTER_ADDRESS_MAP_EOI) = 0;
}

void init_apic(SDTHeader *apic_header) {
    madt = (MADT *) apic_header;
    if (madt->Flags.PCAT_COMPAT == 1)
        mask_all_pic_irq();
    MSR msr_apic = read_msr(MSR_REGISTER_ADDRESS_IA32_APIC_BASE);
    if (msr_apic.IA32APICBaseMSRLayout.xAPICGlobalEnable == 0) {
        msr_apic.IA32APICBaseMSRLayout.xAPICGlobalEnable = 1;
        write_msr(MSR_REGISTER_ADDRESS_IA32_APIC_BASE, msr_apic);
    }
    local_apic_base = msr_apic.IA32APICBaseMSRLayout.APICBaseAddress << 12U;
    uint8_t *madt_p = (uint8_t *) madt;
    InterruptControllerStructureHeader *header;
    for (uint64_t i = sizeof(MADT); i < madt->header.length; i += header->length) {
        header = (InterruptControllerStructureHeader *) &madt_p[i];
        if (header->type == IOAPIC) {
            io_apic_address = ((IOAPICStructure *) header)->ioAPICAddress;
            break;
        }
    }
}
