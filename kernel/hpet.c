#include <hpet.h>

#include <acpi.h>
#include <graphics.h>
#include <stdbool.h>
#include <architecture/apic.h>
#include <architecture/interrupt.h>
#include <architecture/io.h>

#define HPET_REGISTER_OFFSET_GENERAL_CAPABILITIES_ID 0
#define HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION 0x10
#define HPET_REGISTER_OFFSET_GENERAL_INTERRUPT_STATUS 0x20
#define HPET_REGISTER_OFFSET_MAIN_COUNTER_VALUE 0xf0
#define HPET_REGISTER_OFFSET_TIMER_CONFIGURATION_CAPABILITY_BASE 0x100
#define HPET_REGISTER_OFFSET_TIMER_COMPARATOR_VALUE_BASE 0x108
#define HPET_REGISTER_OFFSET_TIMER_FSB_INTERRUPT_ROUTE_BASE 0x110
#define MICRO_PER_FEMTO 1000000000

typedef struct __attribute__((packed)) {
    uint8_t addressSpaceID;
    uint8_t registerBitWidth;
    uint8_t registerBitOffset;
    uint8_t reserved;
    uint64_t address;
} ACPIAddressFormat;

typedef struct __attribute__((packed)) {
    SDTHeader header;
    uint32_t eventTimerBlockID;
    ACPIAddressFormat ACPIAddressFormat;
    uint8_t HPETNumber;
    uint16_t mainCounterMinimumClockTickInPeriodicMode;
    uint8_t pageProtectionAndOEMAttribute;
} HPETDT;

typedef union __attribute__((packed)) {
    uint64_t bits;
    struct {
        uint8_t revID;
        uint8_t numTimCap: 5;
        uint8_t countSizeCap: 1;
        uint8_t reserved: 1;
        uint8_t legRtCap: 1;
        uint16_t vendorID;
        uint32_t counterClkPeriod;
    };
} GeneralCapabilitiesIDRegister;

typedef union {
    uint64_t bits;
    struct {
        uint64_t enableCnf: 1;
        uint64_t legRtCnf: 1;
        uint64_t reserved: 6;
        uint64_t reservedForNonOS: 8;
        uint64_t reserved2: 48;
    };
} GeneralConfigurationRegister;

typedef union {
    uint64_t mainCounterVal;
} MainCounterRegister;

typedef union {
    uint64_t bits;
    struct {
        uint16_t reserved: 1;
        uint16_t intTypeCnf: 1;
        uint16_t intEnbCnf: 1;
        uint16_t typeCnf: 1;
        uint16_t perIntCap: 1;
        uint16_t sizeCap: 1;
        uint16_t valSetCnf: 1;
        uint16_t reserved2: 1;
        uint16_t mode32BitCnf: 1;
        uint16_t intRouteCnf: 5;
        uint16_t FSBEnCnf: 1;
        uint16_t FSBIntDelCap: 1;
        uint16_t reserved3;
        uint32_t intRouteCap;
    };
} TimerNConfigurationCapabilityRegister;

typedef union {
    uint64_t comparatorVal;
} TimerNComparatorRegister;

HPETDT *hpetdt;
uint64_t hpet_register_base;
uint32_t main_counter_tick_period_by_femto;
uint8_t num_timers;
bool is_hpet_counter_size_64bit;
bool is_use_non_periodic;

void (*do_non_periodic_timer_handler)();

uint64_t read_hpet_register(uint32_t offset) {
    return *(uint64_t *) (hpet_register_base + offset);
}

void write_hpet_register(uint32_t offset, uint64_t value) {
    *(uint64_t *) (hpet_register_base + offset) = value;
}

uint64_t read_hpet_timer_n(uint8_t timer_n, uint32_t offset) {
    return read_hpet_register(offset + 20 * timer_n);
}

void write_hpet_timer_n(uint8_t timer_n, uint32_t offset, uint64_t value) {
    write_hpet_register(offset + 20 * timer_n, value);
}

void disable_timer() {
    GeneralConfigurationRegister gcr =
            (GeneralConfigurationRegister) read_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION);
    gcr.enableCnf = 0;
    write_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION, gcr.bits);
}

void enable_timer() {
    GeneralConfigurationRegister gcr =
            (GeneralConfigurationRegister) read_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION);
    gcr.enableCnf = 1;
    write_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION, gcr.bits);
}

void init_hpet(SDTHeader *hpet) {
    io_write_b(0x43, 0b00110010); // Set 8254 PIT to one-shot mode
    hpetdt = (HPETDT *) hpet;
    hpet_register_base = hpetdt->ACPIAddressFormat.address;
    GeneralConfigurationRegister gcr =
            (GeneralConfigurationRegister) read_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION);
    gcr.enableCnf = 0;
    gcr.legRtCnf = 0;
    write_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION, gcr.bits);
    write_hpet_register(HPET_REGISTER_OFFSET_MAIN_COUNTER_VALUE, 0);
    GeneralCapabilitiesIDRegister gcir =
            (GeneralCapabilitiesIDRegister) read_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CAPABILITIES_ID);
    main_counter_tick_period_by_femto = gcir.counterClkPeriod;
    is_hpet_counter_size_64bit = gcir.countSizeCap;
    num_timers = gcir.numTimCap;
}

void non_periodic_timer_interrupt() {
    enter_interrupt();
    do_non_periodic_timer_handler();
    is_use_non_periodic = false;
    leave_interrupt();
}

bool set_timer(uint8_t timer_no, uint64_t micro_second, bool periodic_mode, uint8_t *_irq_no, void *handler) {
    if (timer_no > num_timers)
        return false;
    TimerNConfigurationCapabilityRegister tnccr =
            (TimerNConfigurationCapabilityRegister)
                    read_hpet_timer_n(timer_no, HPET_REGISTER_OFFSET_TIMER_CONFIGURATION_CAPABILITY_BASE);
    uint8_t irq_no = 0;
    uint32_t int_route_cap_tmp = tnccr.intRouteCap;
    while ((int_route_cap_tmp & 1U) == 0) {
        irq_no++;
        int_route_cap_tmp >>= 1U;
    }
    tnccr.intTypeCnf = 0;
    tnccr.intEnbCnf = 1;
    tnccr.typeCnf = periodic_mode;
    tnccr.intRouteCnf = irq_no;
    tnccr.FSBEnCnf = 0;
    *_irq_no = irq_no = irq_no + INTERRUPT_NUMBER_BASE;
    if (periodic_mode) {
        if (!tnccr.perIntCap || is_use_non_periodic)
            return false;
        disable_timer();
        write_hpet_timer_n(timer_no, HPET_REGISTER_OFFSET_TIMER_CONFIGURATION_CAPABILITY_BASE, tnccr.bits);
        write_hpet_register(HPET_REGISTER_OFFSET_MAIN_COUNTER_VALUE, 0);
        tnccr = (TimerNConfigurationCapabilityRegister)
                read_hpet_timer_n(timer_no, HPET_REGISTER_OFFSET_TIMER_CONFIGURATION_CAPABILITY_BASE);
        tnccr.valSetCnf = 1;
        write_hpet_timer_n(timer_no, HPET_REGISTER_OFFSET_TIMER_CONFIGURATION_CAPABILITY_BASE, tnccr.bits);
        set_interrupt_descriptor(irq_no, handler, 1);
        set_apic_irq(true, irq_no);
        uint64_t femto_second = micro_second * MICRO_PER_FEMTO;
        write_hpet_timer_n(timer_no, HPET_REGISTER_OFFSET_TIMER_COMPARATOR_VALUE_BASE,
                           femto_second / main_counter_tick_period_by_femto);
        enable_timer();
    } else {
        is_use_non_periodic = true;
        do_non_periodic_timer_handler = handler;
        set_interrupt_descriptor(irq_no, non_periodic_timer_interrupt, 1);
        set_apic_irq(true, irq_no);

    }
    return true;
}

void dump_gcir() {
    GeneralCapabilitiesIDRegister gcir =
            (GeneralCapabilitiesIDRegister) read_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CAPABILITIES_ID);
    kernel_printf("REV_ID: %d,NUM_TIM_CAP: %d,COUNT_SIZE_CAP: %d,LEG_RT_CAP: %d,"
                  "VENDOR_ID: %d,COUNTER_CLK_PERIOD: %d", gcir.revID, gcir.numTimCap, gcir.countSizeCap, gcir.legRtCap,
                  gcir.vendorID, gcir.counterClkPeriod);
}

void dump_gcr() {
    GeneralConfigurationRegister gcr =
            (GeneralConfigurationRegister) read_hpet_register(HPET_REGISTER_OFFSET_GENERAL_CONFIGURATION);
    kernel_printf("ENABLE_CNF: %d,LEG_RT_CNF: %d", gcr.enableCnf, gcr.legRtCnf);
}

void dump_mcr() {
    MainCounterRegister mcr = (MainCounterRegister) read_hpet_register(HPET_REGISTER_OFFSET_MAIN_COUNTER_VALUE);
    kernel_printf("MAIN_COUNTER_VAL: %lu", mcr.mainCounterVal);
}

void dump_hpet_info() {
    kernel_printf("HPET Info: [");
    dump_gcir();
    kernel_print_char(',');
    dump_gcr();
    kernel_print_char(',');
    dump_mcr();
    kernel_printf("]\n");
}
