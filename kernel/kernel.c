#include <stdint.h>
#include <bootparms.h>
#include <stdbool.h>

#include "include/framebuffer.h"

void start_kernel(BootParameter *bootParameter) {
    init_frame_buffer_info(&bootParameter->frameBuffer);
    RGB rgb = {255,255,255};
    fill_screen(&rgb);

    while (true);
}
