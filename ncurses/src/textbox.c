/***[Includes]****************************************************************/
#include "textbox.h"
#include "color.h"
#include "widget.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/***[Static variables]********************************************************/

/***[Static functions prototypes]*********************************************/

/***[Static functions]********************************************************/

static void calculate_rows_and_words(widget_t *widget);

static void alignment_to_position(widget_t *widget) {
    textbox_t *textbox = (textbox_t *)widget->data;
    int ypos = 0;
    int xpos = 0;

    // Minus two because of the borders.
    const int max_width = getmaxx(widget->base.window) - 2;
    const int max_height = getmaxy(widget->base.window) - 2;

    // Vertical alignment
    switch (textbox->alignment) {
    case TEXT_ALIGN_TOP_LEFT:
    case TEXT_ALIGN_TOP_CENTER:
    case TEXT_ALIGN_TOP_RIGHT: {
        ypos = 1;
        break;
    }

    case TEXT_ALIGN_MIDDLE_LEFT:
    case TEXT_ALIGN_MIDDLE_CENTER:
    case TEXT_ALIGN_MIDDLE_RIGHT: {
        ypos = (getmaxy(widget->base.window) - textbox->text_rows) / 2;
        break;
    }

    case TEXT_ALIGN_BOTTOM_LEFT:
    case TEXT_ALIGN_BOTTOM_CENTER:
    case TEXT_ALIGN_BOTTOM_RIGHT: {
        ypos = getmaxy(widget->base.window) - 1 - textbox->text_rows;
        break;
    }
    }

    ypos = ypos + textbox->top_displayed_text_row;

    textbox->text_rows = 0;

    char line[MAX_CHARS_IN_LINE] = "";
    char *copy = strdup(textbox->text);

    char *word = strtok(copy, " ");
    while (word) {
        // Add words to the current line while its width does not surpasses the
        // window's width.
        if (strlen(line) + strlen(word) < max_width) {
            // Add space between words
            if (strlen(line) > 0) {
                strncat(line, " ", 2);
            }
            strncat(line, word, strlen(word) + 1);
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
                xpos = ((max_width - (int)strlen(line)) / 2) + 1;
                break;
            }

            case TEXT_ALIGN_TOP_RIGHT:
            case TEXT_ALIGN_MIDDLE_RIGHT:
            case TEXT_ALIGN_BOTTOM_RIGHT: {
                xpos = getmaxx(widget->base.window) - (int)strlen(line) - 1;
                break;
            }
            }

            wattron(widget->base.window, COLOR_PAIR(textbox->text_color));
            mvwprintw(widget->base.window, ypos, xpos, "%s", line);
            wattroff(widget->base.window, COLOR_PAIR(textbox->text_color));
            ypos++;
            textbox->text_rows++;
            line[0] = '\0';
            strncat(line, word, sizeof(word) + 1);
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
            xpos = ((max_width - (int)strlen(line)) / 2) + 1;
            break;
        }

        case TEXT_ALIGN_TOP_RIGHT:
        case TEXT_ALIGN_MIDDLE_RIGHT:
        case TEXT_ALIGN_BOTTOM_RIGHT: {
            xpos = getmaxx(widget->base.window) - (int)strlen(line) - 1;
            break;
        }
        }

        wattron(widget->base.window, COLOR_PAIR(textbox->text_color));
        mvwprintw(widget->base.window, ypos, xpos, "%s", line);
        ypos++;
        textbox->text_rows++;
        wattroff(widget->base.window, COLOR_PAIR(textbox->text_color));
    }

    if (textbox->text_rows > max_height) {
        wattron(widget->base.window, COLOR_PAIR(textbox->text_color));
        mvwprintw(widget->base.window, 0, 0, "%c%c%c", ACS_UARROW, ACS_UARROW,
                  ACS_UARROW);
        mvwprintw(widget->base.window, getmaxy(widget->base.window), 0,
                  "%c%c%c", ACS_DARROW, ACS_DARROW, ACS_DARROW);
        wattroff(widget->base.window, COLOR_PAIR(textbox->text_color));
    }

    free(copy);
}

static void calculate_rows_and_words(widget_t *widget) {
    textbox_t *textbox = (textbox_t *)widget->data;

    // Minus two because of the borders.
    const uint16_t max_width = getmaxy(widget->base.window) - 2;
    uint16_t chars_in_line = 0;
    uint16_t chars_in_word = 0;

    textbox->text_rows = 0;
    textbox->text_words = 0;

    // The white-space is counted as character that prefixes words.
    for (uint16_t i = 0; i < (textbox->text[i] != (char)'\0'); i++) {
        chars_in_word++;
        if ((textbox->text[i] == ' ') || (textbox->text[i + 1] == '\0')) {
            textbox->text_words++;
            if (chars_in_line + chars_in_word <= max_width) {
                chars_in_line += chars_in_word;
            } else {
                textbox->text_rows++;
                chars_in_line = chars_in_word;
            }
            chars_in_word = 0;
        }
    }
}

/***[Public functions]********************************************************/

widget_t *textbox_new(const char *text) {
    widget_t *widget = (widget_t *)malloc(sizeof(widget_t));
    textbox_t *textbox = (textbox_t *)malloc(sizeof(textbox_t));

    if ((widget == NULL) || (textbox == NULL)) {
        free(widget);
        free(textbox);
        return NULL;
    }

    widget_init(widget, (void *)textbox, WIDGET_TEXTBOX, NULL,
                textbox_on_refresh, textbox_del, textbox_on_focus,
                textbox_on_lose_focus, textbox_on_resize);

    textbox->text_color = COLOR_WHITE_BLACK;
    textbox->border_color = COLOR_WHITE_BLACK;
    textbox->stored_border_color = COLOR_WHITE_BLACK;

    textbox_set_text(widget, text);
    textbox->top_displayed_text_row = 0;

    textbox_set_border(widget);

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

// Return true if the key was consumed
bool textbox_on_focus(widget_t *widget, int key) {
    textbox_t *textbox = widget->data;
    bool key_was_consumed = false;

    switch (key) {
    case 's': {
        textbox->top_displayed_text_row++;
        key_was_consumed = true;
        break;
    }

    case 'w': {
        textbox->top_displayed_text_row =
            (textbox->top_displayed_text_row > 0)
                ? textbox->top_displayed_text_row - 1
                : textbox->top_displayed_text_row;
        key_was_consumed = true;
        break;
    }

    default: {
        break;
    }
    }

    textbox_set_border_color(widget, COLOR_RED_BLACK);

    return key_was_consumed;
}

void textbox_on_lose_focus(widget_t *widget) {
    textbox_t *textbox = widget->data;
    textbox_set_border_color(widget, textbox->stored_border_color);
}

bool textbox_on_refresh(widget_t *widget) {

    // TODO werase or wclear ?
    werase(widget->base.window);

    alignment_to_position(widget);

    textbox_set_border(widget);
    wnoutrefresh(widget->base.window);
    return true;
}

void textbox_on_resize(widget_t *widget, dim_t dim, pos_t pos) {
    if (widget->base.window != NULL) {
        wclear(widget->base.window);
        delwin(widget->base.window);
    }
    widget->base.window = newwin(dim.height, dim.width, pos.y, pos.x);

    textbox_on_refresh(widget);
}

void textbox_set_color(widget_t *widget, color_t color) {
    textbox_t *textbox = (textbox_t *)widget->data;
    textbox->text_color = color;
    textbox_on_refresh(widget);
}

void textbox_set_alignment(widget_t *widget, text_align_t alignment) {

    textbox_t *textbox = (textbox_t *)widget->data;

    textbox->alignment = alignment;
}

void textbox_set_border_color(widget_t *widget, color_t color) {
    textbox_t *textbox = (textbox_t *)widget->data;
    // TODO, there is some badness here with the stored color
    textbox->border_color = color;
}

void textbox_set_border(widget_t *widget) {
    textbox_t *textbox = (textbox_t *)widget->data;
    wattron(widget->base.window, COLOR_PAIR(textbox->border_color));
    wborder(widget->base.window, 0, 0, 0, 0, 0, 0, 0, 0);
    wattroff(widget->base.window, COLOR_PAIR(textbox->border_color));
}

void textbox_set_text(widget_t *widget, const char *text) {
    textbox_t *textbox = (textbox_t *)(widget->data);
    strncpy(textbox->text, text, sizeof(textbox->text));
    calculate_rows_and_words(widget);
}
