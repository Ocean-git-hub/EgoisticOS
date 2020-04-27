include_directories(../include)

add_executable(Kernel include kernel.c ../common/framebuffer.c graphics.c x64.c keyboard.c kbencoder.c kbcontroller.c interrupt.c pic.c handler.c)
target_link_options(Kernel PUBLIC -T ../../kernel/kernel.ld -e start_kernel -Wall -Wextra -Wno-missing-field-initializers -nostdlib -shared)
