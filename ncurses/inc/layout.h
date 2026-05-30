#ifndef LAYOUT_H_
#define LAYOUT_H_

/***[Includes]****************************************************************/

#include "widget.h"
#include <ncurses.h>
#include <stdint.h>

/***[Macros]******************************************************************/

/***[Types]*******************************************************************/
typedef enum layout_dir_t {
    LAYOUT_UP,
    LAYOUT_DOWN,
    LAYOUT_LEFT,
    LAYOUT_RIGHT,
} layout_dir_t;

typedef struct layout_t {
    uint8_t rows;
    uint8_t cols;
    widget_t **widgets;
    uint8_t focus_row;
    uint8_t focus_col;
    uint8_t focus_idx;
} layout_t;

/***[Extern variables]********************************************************/

/***[Functions prototypes]****************************************************/

/**
 * @brief Creates a new layout inside "parent" window.
 * @param parent Parent window where the layout will be built.
 * If "parent" is NULL, then ncurses will be initialized and "stdscr"
 * will be used.
 * @param rows Number of rows. Height will be equally distributed between
 * widgets.
 * @param cols Number of columns. Width will be equally distributed between
 * widgets.
 * @return The newly created layout, or NULL in case of error.
 */
widget_t *layout_new(WINDOW *parent, uint8_t rows, uint8_t cols);

/**
 * @brief Add or replace a widget into an existing layout.
 * @param layout Layout where to place the widget.
 * @param widget Widget to be placed.
 * @param row Row position for the new widget.
 * @param col Column position for the new widget.
 */
void layout_add(widget_t *widget, widget_t *new_widget, uint8_t row,
                uint8_t col);

/// @brief Refresh all widgets inside the layout.
/// @param layout Layout to be refreshed.
void layout_show(widget_t *widget);

/// @brief Frees resources from layout.
/// @param layout Layout to be deleted. If the layout's parent window is
/// "stdscr", then ncurses will be uninitialized.
widget_status_t layout_del(widget_t *widget);

void layout_change_focus(widget_t *widget, layout_dir_t dir);

bool layout_consume_key(widget_t *widget, int key);

void layout_on_resize(widget_t *widget, int height, int width, int ypos,
                      int xpos);

void layout_on_refresh(widget_t *widget);

bool layout_on_focus(widget_t *widget, int key);

void layout_on_lose_focus(widget_t *widget);

#endif // LAYOUT_H