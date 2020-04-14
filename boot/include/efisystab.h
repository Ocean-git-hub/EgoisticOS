#ifndef EGOISTICOS_EFISYSTAB_H
#define EGOISTICOS_EFISYSTAB_H

#include "efi/efistruct.h"

extern EFI_SYSTEM_TABLE *system_table;

void init_systab(EFI_SYSTEM_TABLE *_system_table);

#endif //EGOISTICOS_EFISYSTAB_H
