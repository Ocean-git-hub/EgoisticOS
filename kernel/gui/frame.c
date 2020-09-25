#include <gui/frame.h>

#define TITLE_BAR_H 20
#define TITLE_TEXT_OFFSET 15

void frame_create(Frame *frame, char *title, uint64_t x, uint64_t y, uint64_t w, uint64_t h) {
    if (h < TITLE_BAR_H)
        h = TITLE_BAR_H;
    window_create(&frame->mainWindow, x, y, w, h);
    window_create_sub_window(&frame->titleWindow, &frame->mainWindow, 0, 0, w, TITLE_BAR_H);
    window_create_sub_window(&frame->subMainWindow, &frame->mainWindow, 0, TITLE_BAR_H, w, h - TITLE_BAR_H);
    window_set_RGB(&frame->titleWindow, &rgb_white, &rgb_blue);
    frame->title = title;
}

void frame_show(Frame *frame) {
    window_clear(&frame->mainWindow);
    window_clear(&frame->titleWindow);
    window_set_text_position(&frame->titleWindow, frame->title, TITLE_TEXT_OFFSET, 0);
    window_clear(&frame->subMainWindow);
}
