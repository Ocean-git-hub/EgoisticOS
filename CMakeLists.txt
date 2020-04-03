cmake_minimum_required(VERSION 3.15)
project(EgoisticOS C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

if (${CMAKE_C_COMPILER} STREQUAL /usr/bin/x86_64-w64-mingw32-gcc)
    set(CMAKE_C_FLAGS "-Wall -Wextra -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -Wno-missing-field-initializers")
    set(CMAKE_EXE_LINKER_FLAGS "-e efi_main ")
    add_executable(Boot boot/boot.c boot/menu.c boot/string.c boot/include include)
else ()
    set(CMAKE_C_FLAGS "-Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -Wno-shift-count-overflow -Wno-missing-field-initializers -fPIE")
    set(CMAKE_EXE_LINKER_FLAGS "-e start_kernel -s")
    add_executable(Kernel kernel/kernel.c include)
endif ()