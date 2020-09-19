#include <window.h>

#include "../common/include/framebuffer.h"
#include <graphics.h>
#include <font.h>

Window root_window;

void init_window() {
    window_create(&root_window, 0, 0, get_screen_width(), get_screen_height());
    root_window.parentWindow = 0;
}

void window_create(Window *window, uint64_t x, uint64_t y, uint64_t w, uint64_t h) {
    window->x0 = x;
    window->y0 = y;
    window->w = w;
    window->h = h;
    window->x1 = x + w - 1;
    window->y1 = y + h - 1;
    window->background.blue = window->background.green = window->background.red = 255;
    window->foreground.blue = window->foreground.green = window->foreground.red = 0;
    window->parentWindow = &root_window;
    for (uint64_t i = 0; i < MAX_NUM_WINDOWS; ++i)
        window->childWindows[i] = 0;
}

void window_create_sub_window(Window *window, Window *parent_window, uint64_t x, uint64_t y, uint64_t w, uint64_t h) {
    window_create(window, x, y, w, h);
    window->parentWindow = parent_window;
}

void window_set_RGB(Window *window, RGB *rgb_fore, const RGB *rgb_back) {
    window->foreground = *rgb_fore;
    window->background = *rgb_back;
}

void window_set_foreground_RGB(Window *window, const RGB *rgb) {
    window->foreground = *rgb;
}

void window_set_background_RGB(Window *window, const RGB *rgb) {
    window->background = *rgb;
}

uint64_t window_get_height(Window *window) {
    return window->h;
}

uint64_t window_get_width(Window *window) {
    return window->w;
}

void window_get_absolutely_position(Window *window, uint64_t *x0, uint64_t *x1, uint64_t *y0, uint64_t *y1) {
    *x0 = window->x0, *y0 = window->y0, *x1 = window->x1, *y1 = window->y1;
    Window *window_p = window->parentWindow;
    while (window_p->parentWindow != 0) {
        *x0 += window_p->x0, *y0 += window_p->y0, *x1 += window_p->x0, *y1 += window_p->y0;
        window_p = window_p->parentWindow;
    }
}

void window_set_text(Window *window, char *string) {
    window_clear(window);
    uint64_t x0, _x0, y0, x1, y1;
    window_get_absolutely_position(window, &x0, &x1, &y0, &y1);
    _x0 = x0;
    while (*string != '\0') {
        print_char_coordinate_RGB(*string++, x0, y0, &window->foreground);
        x0 += FONT_WIDTH;
        if (x0 + FONT_WIDTH > x1) {
            x0 = _x0;
            y0 += FONT_HEIGHT;
            if (y0 + FONT_HEIGHT > y1)
                return;
        }
    }
}

void window_clear(Window *window) {
    uint64_t x0, y0, x1, y1;
    window_get_absolutely_position(window, &x0, &x1, &y0, &y1);
    draw_fill_box_RGB(x0, y0, x1, y1, &window->background);
}
