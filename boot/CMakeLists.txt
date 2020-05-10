include_directories(include)

add_executable(BootLoader boot.c efisystab.c efiio.c string.c utils.c efifilelib.c executablelib.c efilocatedprotocol.c ../common/framebuffer.c)
target_link_options(BootLoader PUBLIC -e efi_main -Wall -Wextra -nostdlib -Wl,--subsystem,10 -Wno-missing-field-initializers)
