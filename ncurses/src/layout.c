/***[Includes]****************************************************************/
#include "layout.h"
#include "color.h"
#include "textbox.h"
#include "widget.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

/***[Static variables]********************************************************/

/***[Static functions]********************************************************/

/**
 * @brief Returns the index used to access a widget located at a certain
 * row and column in a layout.
 * @param layout Layout.
 * @param row Widget row.
 * @param col Widget column.
 * @return The widget index corresponding to the row and column numbers.
 */
static uint8_t pos2idx(layout_t *layout, uint8_t row, uint8_t col) {
    return row * layout->cols + col;
}

/***[Public functions]********************************************************/

widget_t *layout_new(WINDOW *parent, uint8_t rows, uint8_t cols) {

    widget_t *widget = (widget_t *)malloc(sizeof(widget_t));
    layout_t *layout = (layout_t *)malloc(sizeof(layout_t));
    layout->widgets =
        (widget_t **)malloc((size_t)rows * cols * sizeof(widget_t *));

    if ((widget == NULL) || (layout == NULL) || (layout->widgets == NULL)) {
        free((void *)layout->widgets);
        free((void *)layout);
        free((void *)widget);
        return NULL;
    }

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

    widget_init(widget, (void *)layout, WIDGET_LAYOUT, parent,
                layout_on_refresh, layout_del, layout_on_focus,
                layout_on_lose_focus, layout_on_resize);

    layout->rows = rows;
    layout->cols = cols;
    layout->focus_row = 0;
    layout->focus_col = 0;
    layout->focus_idx = 0;

    return widget;
}

bool layout_on_focus(widget_t *widget, int key) {
    (void)key;
    (void)widget;
    return true;
}

void layout_on_lose_focus(widget_t *widget) {}

void layout_show(widget_t *widget) {
    bool key_was_consumed_by_widget;
    bool exit = false;
    int key;

    layout_t *layout = (layout_t *)widget->data;

    pos_t pos;
    dim_t dim;
    getyx(widget->base.window, pos.y, pos.x);
    getmaxyx(widget->base.window, dim.height, dim.width);

    layout_on_resize(widget, dim, pos);
    layout_on_refresh(widget);

    do {
        key = getch();
        key_was_consumed_by_widget =
            layout->widgets[layout->focus_idx]->base.on_focus_fn(
                layout->widgets[layout->focus_idx], key);

        layout_on_refresh(widget);

        if (!key_was_consumed_by_widget) {
            exit = layout_consume_key(widget, key);
            layout_on_refresh(widget);
        }

        doupdate();
        wrefresh(widget->base.window);
    } while (!exit);
}

bool layout_consume_key(widget_t *widget, int key) {

    layout_t *layout = (layout_t *)widget->data;
    pos_t pos;
    dim_t dim;
    bool exit = false;
    switch (key) {
    case KEY_LEFT: {
        layout_change_focus(widget, LAYOUT_LEFT);
        break;
    }

    case KEY_RIGHT: {
        layout_change_focus(widget, LAYOUT_RIGHT);
        break;
    }

    case KEY_UP: {
        layout_change_focus(widget, LAYOUT_UP);
        break;
    }

    case KEY_DOWN: {
        layout_change_focus(widget, LAYOUT_DOWN);
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

    case KEY_RESIZE: {
        getmaxyx(widget->base.window, dim.height, dim.width);
        getbegyx(widget->base.window, pos.y, pos.x);
        layout_on_resize(widget, dim, pos);
    }

    default: {
        break;
    }
    }

    return exit;
}

void layout_on_refresh(widget_t *widget) {
    uint8_t idx;

    layout_t *layout = (layout_t *)widget->data;

    for (uint8_t row = 0; row < layout->rows; row++) {
        for (uint8_t col = 0; col < layout->cols; col++) {
            idx = pos2idx(layout, row, col);
            layout->widgets[idx]->base.on_refresh_fn(layout->widgets[idx]);
        }
    }
}

void layout_on_resize(widget_t *widget, dim_t dim, pos_t pos) {

    (void)dim;
    (void)pos;

    uint8_t idx;

    layout_t *layout = (layout_t *)widget->data;

    pos_t widget_pos;
    dim_t widget_dim = {
        .height = LINES / layout->rows,
        .width = COLS / layout->cols,
    };

    for (uint8_t row = 0; row < layout->rows; row++) {
        for (uint8_t col = 0; col < layout->cols; col++) {
            idx = pos2idx(layout, row, col);

            widget_pos.y = row * widget_dim.height;
            widget_pos.x = col * widget_dim.width;

            layout->widgets[idx]->base.on_resize_fn(layout->widgets[idx],
                                                    widget_dim, widget_pos);
        }
    }

    layout_on_refresh(widget);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
widget_status_t layout_add(widget_t *parent, widget_t *child, uint8_t row,
                           uint8_t col) {
    uint8_t idx;
    layout_t *layout;
    widget_status_t ret;

    ret = widget_cast(parent, (void **)&layout, WIDGET_LAYOUT);
    if (ret != WIDGET_OK) {
        return ret;
    }

    if (row > layout->rows || col > layout->cols) {
        return WIDGET_OUT_OF_BOUNDS;
    }

    if (child == NULL) {
        return WIDGET_NULL;
    }

    idx = pos2idx(layout, row, col);
    layout->widgets[idx] = child;

    return WIDGET_OK;
}

void layout_change_focus(widget_t *widget, layout_dir_t dir) {

    bool lost_focus = false;
    layout_t *layout = (layout_t *)widget->data;

    switch (dir) {
    case LAYOUT_UP: {
        if (layout->focus_row > 0) {
            layout->focus_row--;
            lost_focus = true;
        }
        break;
    }

    case LAYOUT_DOWN: {
        if (layout->focus_row < (layout->rows - 1)) {
            layout->focus_row++;
            lost_focus = true;
        }
        break;
    }

    case LAYOUT_LEFT: {
        if (layout->focus_col > 0) {
            layout->focus_col--;
            lost_focus = true;
        }
        break;
    }

    case LAYOUT_RIGHT: {
        if (layout->focus_col < (layout->cols - 1)) {
            layout->focus_col++;
            lost_focus = true;
        }

        break;
    }
    }

    if (lost_focus) {
        layout->widgets[layout->focus_idx]->base.on_lose_focus_fn(
            layout->widgets[layout->focus_idx]);
        layout->focus_idx =
            pos2idx(layout, layout->focus_row, layout->focus_col);
    }
}

widget_status_t layout_del(widget_t *widget) {
    layout_t *layout = (layout_t *)widget->data;

    for (uint8_t row = 0; row < layout->rows; row++) {
        for (uint8_t col = 0; col < layout->cols; col++) {
            layout->widgets[row * layout->cols + col]->base.del_fn(
                layout->widgets[row * layout->cols + col]);
        }
    }

    if (widget->base.window == stdscr) {
        endwin();
    }

    free((void *)layout->widgets);
    free((void *)layout);

    return WIDGET_OK;
}
