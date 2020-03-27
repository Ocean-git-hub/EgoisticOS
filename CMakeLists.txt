cmake_minimum_required(VERSION 3.15)
project(EgoisticOS C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_link_options(-e efi_main -Wall -Wextra -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10)

add_executable(EgoisticOSBoot boot/boot.c include boot)
add_executable(EgoisticOSKernel kernel/kernel.c include)