#ifndef EGOISTICOS_WINDOW_H
#define EGOISTICOS_WINDOW_H

#include <stdint.h>
#include "../../common/include/color.h"

#define MAX_NUM_WINDOWS 10

typedef struct Window Window;

struct Window {
    uint64_t x0, y0, x1, y1, w, h;
    RGB foreground, background;
    Window *parentWindow, *childWindows[MAX_NUM_WINDOWS];
};

void init_window();

void window_create(Window *window, uint64_t x, uint64_t y, uint64_t w, uint64_t h);

void window_create_sub_window(Window *window, Window *parent_window, uint64_t x, uint64_t y, uint64_t w, uint64_t h);

void window_set_RGB(Window *window, RGB *rgb_fore, const RGB *rgb_back);

void window_set_foreground_RGB(Window *window, const RGB *rgb);

void window_set_background_RGB(Window *window, const RGB *rgb);

uint64_t window_get_height(Window *window);

uint64_t window_get_width(Window *window);

void window_set_text(Window *window, char *string);

void window_clear(Window *window);

#endif //EGOISTICOS_WINDOW_H
