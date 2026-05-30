#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "widget.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

#define TIMEOUT_MS 100

typedef enum layout_dir_t {
    LAYOUT_UP,
    LAYOUT_DOWN,
    LAYOUT_LEFT,
    LAYOUT_RIGHT,
} layout_dir_t;

typedef struct layout_t {
    WINDOW *parent;
    uint8_t rows;
    uint8_t cols;
    widget_t **widgets;
    uint8_t focus_row;
    uint8_t focus_col;
    uint8_t focus_idx;
} layout_t;

/// @brief Creates a new layout inside "parent" window.
/// @param parent Parent window where the layout will be built.
/// If "parent" is NULL, then ncurses will be initialized and "stdscr"
/// will be used.
/// @param rows Number of rows. Height will be equally distributed between
/// widgets.
/// @param cols Number of columns. Width will be equally distributed between
/// widgets.
/// @return The newly created widget, or NULL in case of error.
layout_t *layout_new(WINDOW *parent, uint8_t rows, uint8_t cols);

/// @brief Add or replace a widget intto an existing layout.
/// @param layout Layout where to place the widget.
/// @param widget Widget to be placed.
/// @param row Row position for the new widget.
/// @param col Column position for the new widget.
void layout_add(layout_t *layout, widget_t *widget, uint8_t row, uint8_t col);

/// @brief Refresh all widgets inside the layout.
/// @param layout Layout to be refreshed.
void layout_show(layout_t *layout);

/// @brief Frees resources from layout.
/// @param layout Layout to be deleted. If the layout's parent window is
/// "stdscr", then ncurses will be uninitialized.
void layout_del(layout_t *layout);

void layout_change_focus(layout_t *layout, layout_dir_t dir);

bool layout_consume_key(layout_t *layout, int key);

void layout_resize(layout_t *layout);
void layout_refresh(layout_t *layout);

#endif // LAYOUT_H