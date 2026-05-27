#ifndef BUTTON_H_
#define BUTTON_H_

#include "widget.h"
#include <ncurses.h>

typedef struct button_t {
    uint32_t presses;
    WINDOW *window;
} button_t;

widget_t *button_new(void);
widget_status_t button_del(widget_t *widget);
void button_refresh(widget_t *widget, int height, int width, int ypos,
                    int xpos);

#endif // BUTTON_H_