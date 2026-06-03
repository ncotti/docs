/***[Includes]****************************************************************/

/***[Static variables]********************************************************/

/***[Static functions prototypes]*********************************************/

/***[Static functions]********************************************************/

/***[Public functions]********************************************************/

/***[Includes]****************************************************************/
#include "button.h"
#include "widget.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

/***[Static variables]********************************************************/

/***[Static functions prototypes]*********************************************/

static void button_on_resize(widget_t *widget, dim_t dim, pos_t pos);
static bool button_on_refresh(widget_t *widget);
static bool button_on_focus(widget_t *widget, int key);
static void button_on_lose_focus(widget_t *widget);

/***[Static functions]********************************************************/

static void button_on_resize(widget_t *widget, dim_t dim, pos_t pos) {
    wresize(widget->base.window, dim.height, dim.width);
    mvwin(widget->base.window, pos.y, pos.x);
    widget->base.dirty = true;
}

static bool button_on_refresh(widget_t *widget) {
    button_t *button = (button_t *)widget->data;
    if (!widget->base.dirty) {
        return false;
    }

    // TODO Be smarter about it
    wclear(widget->base.window);

    wattron(widget->base.window, COLOR_PAIR(button->border_color));
    wborder(widget->base.window, 0, 0, 0, 0, 0, 0, 0, 0);
    wattroff(widget->base.window, COLOR_PAIR(button->border_color));

    wnoutrefresh(widget->base.window);
    widget->base.dirty = false;
    return true;
}

// TODO is the layout responsability to scan for buttons and see if the buttons
// have been pressed with their respective keys
static bool button_on_focus(widget_t *widget, int key) {
    button_t *button = widget->data;
    bool key_was_consumed = false;

    switch (key) {
    case KEY_ENTER:
    case ' ': {
        key_was_consumed = true;
        widget->base.dirty = true;
        button->on_press_fn(button->params);
        break;
    }

    default: {
        break;
    }
    }

    button_set_border_color(widget, COLOR_RED_BLACK);
    return key_was_consumed;
}

static void button_on_lose_focus(widget_t *widget) {
    button_t *button = widget->data;
    button_set_border_color(widget, button->stored_border_color);
}

/***[Public functions]********************************************************/
widget_t *button_new(button_on_press_fn_t on_press_fn, void *params,
                     const char *text, int key, color_t color) {
    widget_t *widget = (widget_t *)malloc(sizeof(widget_t));
    button_t *button = (button_t *)malloc(sizeof(button_t));

    if ((widget == NULL) || (button == NULL)) {
        free(widget);
        free(button);
        return NULL;
    }

    widget_init(widget, (void *)button, WIDGET_BUTTON, newwin(1, 1, 1, 1),
                button_on_refresh, button_del, button_on_focus,
                button_on_lose_focus, button_on_resize);

    button->color = color;
    button->border_color = COLOR_WHITE_BLACK;
    button->stored_border_color = COLOR_WHITE_BLACK;
    button->key = key;
    button->on_press_fn = on_press_fn;
    button->params = params;

    strncpy(button->text, text, sizeof(button->text) - 1);
    button->text[sizeof(button->text) - 1] = '\0';

    return widget;
}

widget_status_t button_del(widget_t *widget) {

    button_t *button = NULL;
    widget_status_t ret = widget_cast(widget, (void **)&button, WIDGET_BUTTON);

    if (ret != WIDGET_OK) {
        return ret;
    }

    wclear(widget->base.window);
    delwin(widget->base.window);
    free((void *)button);
    free((void *)widget);

    return WIDGET_OK;
}

widget_status_t button_set_border_color(widget_t *widget, color_t color) {
    button_t *button = NULL;
    widget_status_t ret = widget_cast(widget, (void **)&button, WIDGET_BUTTON);

    if (ret != WIDGET_OK) {
        return ret;
    }

    if (button->border_color != color) {
        button->stored_border_color = button->border_color;
        button->border_color = color;
        widget->base.dirty = true;
    }
    return WIDGET_OK;
}