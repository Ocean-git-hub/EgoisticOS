#include <desktop.h>

#include "../common/include/framebuffer.h"
#include <timer.h>
#include <datetime.h>
#include <window.h>
#include <string.h>

#define UNDER_PANEL_H 30
#define CLOCK_PANEL_OFFSET (8 * 19)
#define CLOCK_PANEL_W (CLOCK_PANEL_OFFSET + 6)

Window desktop_window, under_panel_window, clock_panel_window;

void display_datetime() {
    DateTime time = get_datetime();
    char string[30];
    s_printf(string, "%04u/%02u/%02u %02u:%02u:%02u", time.year, time.month, time.day, time.hour, time.minute,
             time.second);
    window_set_text(&clock_panel_window, string);
}

void init_desktop() {
    init_window();
    window_create(&desktop_window, 0, 0, get_screen_width(), get_screen_height());
    window_set_background_RGB(&desktop_window, &rgb_ceramica_blu);
    window_clear(&desktop_window);

    window_create_sub_window(&under_panel_window, &desktop_window, 0,
                             window_get_height(&desktop_window) - UNDER_PANEL_H, window_get_width(&desktop_window),
                             UNDER_PANEL_H);
    window_set_background_RGB(&under_panel_window, &rgb_siro);
    window_clear(&under_panel_window);

    window_create_sub_window(&clock_panel_window, &under_panel_window,
                             window_get_width(&desktop_window) - CLOCK_PANEL_W, 0, CLOCK_PANEL_W, UNDER_PANEL_H);
    window_set_background_RGB(&clock_panel_window, &rgb_bianco_perla);
    display_datetime();
    add_timer_observer(Second, display_datetime);
}
