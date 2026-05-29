#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "color.h"
#include "widget.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

#define TEXT_CENTER_ALIGN 1
#define TEXT_LEFT_ALIGN
#define TEXT_RIGHT_ALIGN

typedef enum text_align_t {
    TEXT_ALIGN_TOP_LEFT,
    TEXT_ALIGN_TOP_CENTER,
    TEXT_ALIGN_TOP_RIGHT,
    TEXT_ALIGN_MIDDLE_LEFT,
    TEXT_ALIGN_MIDDLE_CENTER,
    TEXT_ALIGN_MIDDLE_RIGHT,
    TEXT_ALIGN_BOTTOM_LEFT,
    TEXT_ALIGN_BOTTOM_CENTER,
    TEXT_ALIGN_BOTTOM_RIGHT,
} text_align_t;

typedef struct textbox_t {
    char text[1024]; // TODO don't fix its size
    uint32_t text_style;
    color_t text_color;
    text_align_t alignment;
    WINDOW *window;
} textbox_t;

/// @brief Creates a new textbox widget. Textbox is a placeholder for text.
/// @param text Text to be written.
/// @param boxed Whether to draw a border around the textbox or not.
/// @return The newly created textbox widget, or NULL in case of error.
widget_t *textbox_new(const char *text, bool boxed);

/// @brief Frees resources from a previously created textbox.
/// @param widget A textbox widget.
/// @return "WIDGET_OK" on success, or an error code on failure.
widget_status_t textbox_del(widget_t *widget);

void textbox_refresh(widget_t *widget, int height, int width, int ypos,
                     int xpos);

void textbox_set_color(widget_t *widget, color_t color);
void textbox_set_text(widget_t *textbox, const char *text);
void textbox_set_border(widget_t *widget, bool boxed, widget_border_t border);
bool textbox_on_focus(widget_t *widget, int key_pressed);
void textbox_set_alignment(widget_t *widget, text_align_t alignment);

#endif // TEXTBOX_H_