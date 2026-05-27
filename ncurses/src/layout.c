#include "layout.h"
#include <ncurses.h>

WINDOW *create_newwin(int height, int width, int starty, int startx) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0); /* 0, 0 gives default characters
                           * for the vertical and horizontal
                           * lines			*/
    wrefresh(local_win);  /* Show that box 		*/

    return local_win;
}

layout_t *layout_new(WINDOW *parent, uint8_t rows, uint8_t cols) {

    layout_t *layout = (layout_t *)malloc(sizeof(layout_t));

    if (parent == NULL) {
        parent = initscr();
        noecho();
        cbreak();
        keypad(stdscr, true);
        if (has_colors() == FALSE) {
            exit(1);
        }

        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
    }

    wclear(parent);
    wrefresh(parent);
    layout->parent = parent;
    layout->rows = rows;
    layout->cols = cols;
    layout->widgets = malloc(rows * cols * sizeof(widget_t *));

    layout->focus_row = 0;
    layout->focus_col = 0;

    return layout;
}

void layout_refresh_widget(layout_t *layout, uint8_t row, uint8_t col) {
    uint8_t idx = row * layout->cols + col;

    int height = LINES / layout->rows;
    int width = COLS / layout->cols;

    layout->widgets[idx]->base.refresh_fn(layout->widgets[idx], height, width,
                                          row * height, col * width);

    if (row == layout->focus_row && col == layout->focus_col) {
        layout->widgets[idx]->base.on_focus_fn(layout->widgets[idx]);
    }
}

void layout_refresh(layout_t *layout) {
    for (uint8_t row = 0; row < layout->rows; row++) {
        for (uint8_t col = 0; col < layout->cols; col++) {
            layout_refresh_widget(layout, row, col);
        }
    }

    doupdate();
    wrefresh(layout->parent);
}

void layout_add(layout_t *layout, widget_t *widget, uint8_t row, uint8_t col) {
    uint8_t idx;

    if (row > layout->rows) {
        // Error
    }

    if (col > layout->cols) {
        // Error
    }

    idx = row * layout->cols + col;
    layout->widgets[idx] = widget;
}

void layout_change_focus(layout_t *layout, uint8_t row, uint8_t col) {
    layout->focus_row = row;
    layout->focus_col = col;
}

void layout_change_focus_up(layout_t *layout) {
    layout->focus_row =
        (layout->focus_row > 0) ? layout->focus_row - 1 : layout->focus_row;
}

void layout_change_focus_down(layout_t *layout) {
    layout->focus_row = (layout->focus_row < (layout->rows - 1))
                            ? layout->focus_row + 1
                            : layout->focus_row;
}

void layout_change_focus_left(layout_t *layout) {
    layout->focus_col =
        (layout->focus_col > 0) ? layout->focus_col - 1 : layout->focus_col;
}

void layout_change_focus_right(layout_t *layout) {
    layout->focus_col = (layout->focus_col < (layout->cols - 1))
                            ? layout->focus_col + 1
                            : layout->focus_col;
}

void layout_del(layout_t *layout) {
    for (uint8_t row = 0; row < layout->rows; row++) {
        for (uint8_t col = 0; col < layout->cols; col++) {
            layout->widgets[row * layout->cols + col]->base.del_fn(
                layout->widgets[row * layout->cols + col]);
        }
    }

    if (layout->parent == stdscr) {
        endwin();
    }

    free((void *)layout->widgets);
    free((void *)layout);
}