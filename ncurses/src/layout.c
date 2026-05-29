#include "layout.h"
#include "color.h"
#include "textbox.h"
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
        init_pair(COLOR_RED_BLACK, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
        init_pair(COLOR_MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
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

bool layout_refresh_widget(layout_t *layout, uint8_t row, uint8_t col,
                           int key) {
    bool key_consumed = false;
    uint8_t idx = row * layout->cols + col;

    int height = LINES / layout->rows;
    int width = COLS / layout->cols;

    layout->widgets[idx]->base.refresh_fn(layout->widgets[idx], height, width,
                                          row * height, col * width);

    if (row == layout->focus_row && col == layout->focus_col) {
        key_consumed =
            layout->widgets[idx]->base.on_focus_fn(layout->widgets[idx], key);
    }

    return key_consumed;
}

void layout_show(layout_t *layout) {
    bool key_was_consumed_by_widget;
    bool exit = false;
    bool first_time = true;
    int key;

    int iterations = 0;

    do {
        key = (!first_time) ? getch() : 0;
        key_was_consumed_by_widget = false;

        for (uint8_t row = 0; row < layout->rows; row++) {
            for (uint8_t col = 0; col < layout->cols; col++) {
                if (layout_refresh_widget(layout, row, col, key)) {
                    key_was_consumed_by_widget = true;
                }
            }
        }

        if (!key_was_consumed_by_widget) {
            exit = layout_consume_key(layout, key);

            // Need to rerun widget changes
            for (uint8_t row = 0; row < layout->rows; row++) {
                for (uint8_t col = 0; col < layout->cols; col++) {
                    layout_refresh_widget(layout, row, col, key);
                }
            }
        }

        doupdate();
        wrefresh(layout->parent);
        first_time = false;
        iterations++;
    } while (!exit);
}

bool layout_consume_key(layout_t *layout, int key) {
    bool exit = false;
    switch (key) {
    case KEY_LEFT: {
        layout_change_focus_left(layout);
        break;
    }

    case KEY_RIGHT: {
        layout_change_focus_right(layout);
        break;
    }

    case KEY_UP: {
        layout_change_focus_up(layout);
        break;
    }

    case KEY_DOWN: {
        layout_change_focus_down(layout);
        break;
    }

    case 'a': {
        textbox_set_color(layout->widgets[0], COLOR_GREEN_BLACK);
        break;
    }

    case 'b': {
        textbox_set_color(layout->widgets[0], COLOR_YELLOW_BLACK);
        break;
    }

    case 'c': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_TOP_LEFT);
        break;
    }

    case 'd': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_TOP_CENTER);
        break;
    }

    case 'e': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_TOP_RIGHT);
        break;
    }

    case 'f': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_MIDDLE_LEFT);
        break;
    }

    case 'g': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_MIDDLE_CENTER);
        break;
    }

    case 'h': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_MIDDLE_RIGHT);
        break;
    }

    case 'i': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_BOTTOM_LEFT);
        break;
    }

    case 'j': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_BOTTOM_CENTER);
        break;
    }

    case 'k': {
        textbox_set_alignment(layout->widgets[0], TEXT_ALIGN_BOTTOM_RIGHT);
        break;
    }

    case 'q': {
        exit = true;
        break;
    }

    default: {
        break;
    }
    }

    return exit;
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
