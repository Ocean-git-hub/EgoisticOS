#ifndef EGOISTICOS_FRAME_H
#define EGOISTICOS_FRAME_H

#include <gui/window.h>

typedef struct {
    Window mainWindow;
    Window titleWindow;
    Window subMainWindow;
    char *title;
} Frame;

void frame_create(Frame *frame, char *title, uint64_t x, uint64_t y, uint64_t w, uint64_t h);

void frame_show(Frame *frame);

#endif //EGOISTICOS_FRAME_H
