cmake_minimum_required(VERSION 3.15)
project(EgoisticOS)

set(CMAKE_CXX_STANDARD 99)

set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)

include_directories(include)

add_executable(BootLoader boot/include boot/boot.c boot/efisystab.c boot/efiio.c boot/string.c boot/utils.c boot/efifilelib.c boot/executablelib.c boot/efilocatedprotocol.c)
target_link_options(BootLoader PUBLIC -e efi_main -Wall -Wextra -nostdlib -Wl,--subsystem,10 -Wno-missing-field-initializers)

add_executable(Kernel kernel/include kernel/kernel.c kernel/framebuffer.c)
target_link_options(Kernel PUBLIC -e start_kernel -Wall -Wextra -nostdlib -fno-builtin -fno-common -Wno-missing-field-initializers -fPIE -Wl,-pie)