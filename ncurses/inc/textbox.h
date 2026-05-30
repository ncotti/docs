#ifndef TEXTBOX_H_
#define TEXTBOX_H_

/***[Includes]****************************************************************/
#include "color.h"
#include "widget.h"
#include <ncurses.h>
#include <stdint.h>

/***[Macros]******************************************************************/

#define MAX_CHARS_IN_LINE 1024

/***[Types]*******************************************************************/
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
    char text[MAX_CHARS_IN_LINE];
    color_t text_color;
    text_align_t alignment;
    uint16_t text_rows;
    uint16_t text_words;
    uint16_t top_displayed_text_row;
    color_t border_color;
    color_t stored_border_color;
    WINDOW *window;
} textbox_t;

/***[Extern variables]********************************************************/

/***[Functions prototypes]****************************************************/

/// @brief Creates a new textbox widget, a placeholder for text, with default
/// parameters.
/// @param text Text to be written.
/// @return The newly created textbox widget, or NULL in case of error.
widget_t *textbox_new(const char *text);

/// @brief Frees resources from a previously created textbox.
/// @param widget A textbox widget.
/// @return "WIDGET_OK" on success, or an error code on failure.
widget_status_t textbox_del(widget_t *widget);

void textbox_on_resize(widget_t *widget, int height, int width, int ypos,
                       int xpos);
void textbox_on_refresh(widget_t *widget);
bool textbox_on_focus(widget_t *widget, int key_pressed);
void textbox_on_lose_focus(widget_t *widget);

void textbox_set_color(widget_t *widget, color_t color);
void textbox_set_text(widget_t *textbox, const char *text);
void textbox_set_border(widget_t *widget);
void textbox_set_alignment(widget_t *widget, text_align_t alignment);

void textbox_set_border_color(widget_t *widget, color_t color);

#endif // TEXTBOX_H_