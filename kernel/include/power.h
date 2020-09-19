#ifndef EGOISTICOS_POWER_H
#define EGOISTICOS_POWER_H

typedef enum {
    ResetCold,
    ResetWarm,
    ResetShutdown,
    ResetPlatformSpecific
} RESET_TYPE;

void init_power(void (*_reset_system)());

void reset_system_power(RESET_TYPE type);

#endif //EGOISTICOS_POWER_H
