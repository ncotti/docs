#include "button.h"
#include "widget.h"
#include <stdlib.h>

widget_t *button_new(void) {
    button_t *button;
    widget_t *widget;

    widget = (widget_t *)malloc(sizeof(widget_t));
    button = (button_t *)malloc(sizeof(button_t));

    if ((widget == NULL) || (button == NULL)) {
        free(widget);
        free(button);
        return NULL;
    }

    button->presses = 0;

    widget->base.window = NULL;
    widget->data = (void *)button;
    widget->base.refresh_fn = button_refresh;
    widget->base.del_fn = button_del;
    widget->base.xpos = 0;
    widget->base.ypos = 0;
    widget->base.height = 0;
    widget->base.width = 0;
    widget->base.type = WIDGET_BUTTON;

    return widget;
}

widget_status_t button_del(widget_t *widget) {
    if (widget == NULL) {
        return WIDGET_NULL;
    }

    if (widget->base.type != WIDGET_BUTTON) {
        return WIDGET_WRONG_TYPE;
    }

    wclear(widget->base.window);
    delwin(widget->base.window);
    free(widget->data);
    free((void *)widget);

    return WIDGET_OK;
}
void button_refresh(widget_t *widget, int height, int width, int ypos,
                    int xpos) {}