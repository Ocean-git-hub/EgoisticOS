#ifndef EGOISTICOS_DESKTOP_H
#define EGOISTICOS_DESKTOP_H

#include <window.h>

typedef struct WindowHandler WindowHandler;

struct WindowHandler {
    Window window;
};

void init_desktop();

#endif //EGOISTICOS_DESKTOP_H
