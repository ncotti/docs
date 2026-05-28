#include "textbox.h"
#include "color.h"
#include "widget.h"
#include <ncurses.h>
#include <string.h>

static void alignment_to_position(widget_t *widget) {
    textbox_t *textbox = (textbox_t *)widget->data;
    int ypos, xpos;
    int8_t ypos_increment;

    // Vertical alignment
    switch (textbox->alignment) {
    case TEXT_ALIGN_TOP_LEFT:
    case TEXT_ALIGN_TOP_CENTER:
    case TEXT_ALIGN_TOP_RIGHT: {
        ypos = 1;
        ypos_increment = +1;
        break;
    }

    case TEXT_ALIGN_MIDDLE_LEFT:
    case TEXT_ALIGN_MIDDLE_CENTER:
    case TEXT_ALIGN_MIDDLE_RIGHT: {
        ypos = widget->base.height / 2;
        ypos_increment = +1;
        break;
    }

    case TEXT_ALIGN_BOTTOM_LEFT:
    case TEXT_ALIGN_BOTTOM_CENTER:
    case TEXT_ALIGN_BOTTOM_RIGHT: {
        ypos = widget->base.height - 2;
        ypos_increment = -1;
        break;
    }
    }

    // Minus two because of the borders.
    const uint16_t max_width = widget->base.width - 2;

    char line[1024] = "";
    char *copy = strdup(textbox->text);

    char *word = strtok(copy, " ");
    while (word) {
        // Add words to the current line while its width does not surpasses the
        // window's width.
        if (strlen(line) + strlen(word) < max_width) {
            // Add space between words
            if (strlen(line) > 0) {
                strcat(line, " ");
            }
            strcat(line, word);
        } else {
            // Print and erase line

            // Horizontal alignment
            switch (textbox->alignment) {
            case TEXT_ALIGN_TOP_LEFT:
            case TEXT_ALIGN_MIDDLE_LEFT:
            case TEXT_ALIGN_BOTTOM_LEFT: {
                xpos = 1;
                break;
            }

            case TEXT_ALIGN_TOP_CENTER:
            case TEXT_ALIGN_MIDDLE_CENTER:
            case TEXT_ALIGN_BOTTOM_CENTER: {
                xpos = (widget->base.width - 2 - strlen(line)) / 2;
                break;
            }

            case TEXT_ALIGN_TOP_RIGHT:
            case TEXT_ALIGN_MIDDLE_RIGHT:
            case TEXT_ALIGN_BOTTOM_RIGHT: {
                xpos = widget->base.width - strlen(line) - 1;
                break;
            }
            }

            wattron(widget->base.window, COLOR_PAIR(textbox->text_color));
            mvwprintw(widget->base.window, ypos, xpos, "%s", line);
            ypos += ypos_increment;
            wattroff(widget->base.window, COLOR_PAIR(textbox->text_color));
            line[0] = '\0';
            strcat(line, word);
        }

        word = strtok(NULL, " ");
    }

    if (strlen(line) > 0) {
        // Print and erase line

        // Horizontal alignment
        switch (textbox->alignment) {
        case TEXT_ALIGN_TOP_LEFT:
        case TEXT_ALIGN_MIDDLE_LEFT:
        case TEXT_ALIGN_BOTTOM_LEFT: {
            xpos = 1;
            break;
        }

        case TEXT_ALIGN_TOP_CENTER:
        case TEXT_ALIGN_MIDDLE_CENTER:
        case TEXT_ALIGN_BOTTOM_CENTER: {
            xpos = (widget->base.width - 2 - strlen(line)) / 2;
            break;
        }

        case TEXT_ALIGN_TOP_RIGHT:
        case TEXT_ALIGN_MIDDLE_RIGHT:
        case TEXT_ALIGN_BOTTOM_RIGHT: {
            xpos = widget->base.width - strlen(line) - 1;
            break;
        }
        }

        wattron(widget->base.window, COLOR_PAIR(textbox->text_color));
        mvwprintw(widget->base.window, ypos, xpos, "%s", line);
        ypos += ypos_increment;
        wattroff(widget->base.window, COLOR_PAIR(textbox->text_color));
    }

    free(copy);
}

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

    textbox->text_color = COLOR_WHITE_BLACK;
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
    pos_t text_pos;

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

    // TODO werase or wclear ?
    werase(widget->base.window);

    alignment_to_position(widget);

    textbox_set_border(widget, true, border);
    wnoutrefresh(widget->base.window);
}

void textbox_set_color(widget_t *widget, color_t color) {
    textbox_t *textbox = (textbox_t *)widget->data;
    textbox->text_color = color;
    textbox_refresh(widget, widget->base.height, widget->base.width,
                    widget->base.ypos, widget->base.xpos);
}

void textbox_set_alignment(widget_t *widget, text_align_t alignment) {

    textbox_t *textbox = (textbox_t *)widget->data;

    textbox->alignment = alignment;
}

void textbox_set_border(widget_t *widget, bool boxed, widget_border_t border) {
    wborder(widget->base.window, border.ls, border.rs, border.ts, border.bs,
            border.tl, border.tr, border.bl, border.br);
}

void textbox_set_text(widget_t *widget, const char *text) {
    textbox_t *textbox = (textbox_t *)(widget->data);
    strncpy(textbox->text, text, sizeof(textbox->text));
}
