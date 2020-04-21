cmake_minimum_required(VERSION 3.15)
project(EgoisticOS)

set(CMAKE_CXX_STANDARD 99)

include_directories(include)

set(MINGW32_64_GCC /usr/bin/x86_64-w64-mingw32-gcc)
set(NATIVE_GCC /usr/bin/gcc)
set(CMAKE_C_COMPILER ${MINGW32_64_GCC})
set(CMAKE_C_COMPILER ${NATIVE_GCC})

if (${CMAKE_C_COMPILER} STREQUAL ${MINGW32_64_GCC})
    add_executable(BootLoader boot/include boot/boot.c boot/efisystab.c boot/efiio.c boot/string.c boot/utils.c boot/efifilelib.c boot/executablelib.c boot/efilocatedprotocol.c common/framebuffer.c)
    target_link_options(BootLoader PUBLIC -e efi_main -Wall -Wextra -nostdlib -Wl,--subsystem,10 -Wno-missing-field-initializers)
endif ()

if (${CMAKE_C_COMPILER} STREQUAL ${NATIVE_GCC})
    add_executable(Kernel kernel/include kernel/kernel.c kernel/graphics.c common/framebuffer.c)
    target_link_options(Kernel PUBLIC -T ../kernel/kernel.ld -e start_kernel -Wall -Wextra -Wno-missing-field-initializers -nostdlib)
endif ()
