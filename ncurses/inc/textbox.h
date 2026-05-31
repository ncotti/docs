#ifndef TEXTBOX_H_
#define TEXTBOX_H_

/***[Includes]****************************************************************/
#include "color.h"
#include "widget.h"
#include <ncurses.h>
#include <stdint.h>

/***[Macros]******************************************************************/

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
    char *text;
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

/**
 * @brief Creates a new textbox widget, i.e., a placeholder for text.
 * @param text Text to be written.
 * @return The newly created textbox widget, or NULL in case of error.
 */
widget_t *textbox_new(const char *text);

/**
 * @brief Clears the written text and deletes the textbox window.
 * @param widget Textbox widget.
 * @return "WIDGET_OK" on success, or an error code on failure.
 */
widget_status_t textbox_del(widget_t *widget);

/**
 * @brief Sets the text to be displayed.
 * @param widget Textbox widget.
 * @param text Text.
 * @return WIDGET_OK on success.
 */
widget_status_t textbox_set_text(widget_t *widget, const char *text);

/**
 * @brief Sets the color ot the text.
 * @param widget Textbox widget.
 * @param color Color pair to use (foreground, background)
 * @return WIDGET_OK on success.
 */
widget_status_t textbox_set_color(widget_t *widget, color_t color);

/**
 * @brief Changes the text's vertical and horizontal alignment.
 * @param widget Textbox widget.
 * @param alignment Alignment.
 * @return WIDGET_OK on success.
 */
widget_status_t textbox_set_alignment(widget_t *widget, text_align_t alignment);

widget_status_t textbox_set_border_color(widget_t *widget, color_t color);

#endif // TEXTBOX_H_
