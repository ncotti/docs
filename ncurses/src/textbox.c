#include "textbox.h"
#include "widget.h"
#include <ncurses.h>
#include <string.h>

widget_t *textbox_new(const char *text, bool boxed) {
    widget_t *widget;
    textbox_t *textbox;
    widget_border_t border;
    const chtype border_ch = boxed ? 'a' : ' ';

    widget = (widget_t *)malloc(sizeof(widget_t));
    textbox = (textbox_t *)malloc(sizeof(textbox_t));

    if ((widget == NULL) || (textbox == NULL)) {
        free(widget);
        free(textbox);
        return NULL;
    }

    widget->base.window = NULL;
    widget->data = (void *)textbox;
    widget->base.refresh_fn = textbox_refresh;
    widget->base.del_fn = textbox_del;
    widget->base.on_focus_fn = textbox_on_focus;
    widget->base.xpos = 0;
    widget->base.ypos = 0;
    widget->base.height = 0;
    widget->base.width = 0;
    widget->base.type = WIDGET_TEXTBOX;

    textbox_set_text(widget, text);

    memset(&border, border_ch, sizeof(widget_border_t));

    textbox_set_border(widget, boxed, border);

    return widget;
}

widget_status_t textbox_del(widget_t *widget) {
    if (widget == NULL) {
        return WIDGET_NULL;
    }

    if (widget->base.type != WIDGET_TEXTBOX) {
        return WIDGET_WRONG_TYPE;
    }

    wclear(widget->base.window);
    delwin(widget->base.window);
    free(widget->data);
    free((void *)widget);

    return WIDGET_OK;
}

void textbox_on_focus(widget_t *widget) {
    widget_border_t border;
    memset(&border, 0, sizeof(widget_border_t));

    wattron(widget->base.window, COLOR_PAIR(1));
    textbox_set_border(widget, true, border);
    mvwprintw(widget->base.window, 1, 1, "hi");
    wattroff(widget->base.window, COLOR_PAIR(1));
    wrefresh(widget->base.window);
}

void textbox_refresh(widget_t *widget, int height, int width, int ypos,
                     int xpos) {
    textbox_t *textbox = (textbox_t *)widget->data;
    widget_border_t border;

    memset(&border, 0, sizeof(widget_border_t));

    if (widget->base.height != height || widget->base.width != width ||
        widget->base.ypos != ypos || widget->base.xpos != xpos) {

        if (widget->base.window != NULL) {
            wclear(widget->base.window);
            delwin(widget->base.window);
        }
        widget->base.window = newwin(height, width, ypos, xpos);
    }

    widget->base.height = height;
    widget->base.width = width;
    widget->base.ypos = ypos;
    widget->base.xpos = xpos;

    mvwprintw(widget->base.window, 1, 1, "%s", textbox->text);
    textbox_set_border(widget, true, border);
    wnoutrefresh(widget->base.window);
}

void textbox_set_color(void) {}

void textbox_set_border(widget_t *widget, bool boxed, widget_border_t border) {
    wborder(widget->base.window, border.ls, border.rs, border.ts, border.bs,
            border.tl, border.tr, border.bl, border.br);
}

void textbox_set_text(widget_t *widget, const char *text) {
    textbox_t *textbox = (textbox_t *)(widget->data);
    strncpy(textbox->text, text, sizeof(textbox->text));
}
