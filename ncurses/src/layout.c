/***[Includes]****************************************************************/
#include "layout.h"
#include "color.h"
#include "textbox.h"
#include "widget.h"
#include <ncurses.h>
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

    if (widget == NULL || layout == NULL) {
        free(widget);
        free(layout);
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

    wclear(parent);
    wrefresh(parent);
    widget->base.window = parent;
    layout->rows = rows;
    layout->cols = cols;
    layout->widgets = malloc(rows * cols * sizeof(widget_t *));

    layout->focus_row = 0;
    layout->focus_col = 0;
    layout->focus_idx = 0;

    return widget;
}

bool layout_on_focus(widget_t *widget, int key) { return true; }

void layout_on_lose_focus(widget_t *widget) {}

void layout_show(widget_t *widget) {
    bool key_was_consumed_by_widget;
    bool exit = false;
    int key;

    layout_t *layout = (layout_t *)widget->data;

    layout_on_resize(widget, getmaxy(widget->base.window),
                     getmaxx(widget->base.window), getbegy(widget->base.window),
                     getbegx(widget->base.window));
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
        layout_on_resize(
            widget, getmaxy(widget->base.window), getmaxx(widget->base.window),
            getbegy(widget->base.window), getbegx(widget->base.window));
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

void layout_on_resize(widget_t *widget, int height, int width, int ypos,
                      int xpos) {
    uint8_t idx;

    layout_t *layout = (layout_t *)widget->data;

    int widget_height = LINES / layout->rows;
    int widget_width = COLS / layout->cols;

    for (uint8_t row = 0; row < layout->rows; row++) {
        for (uint8_t col = 0; col < layout->cols; col++) {
            idx = pos2idx(layout, row, col);

            layout->widgets[idx]->base.on_resize_fn(
                layout->widgets[idx], widget_height, widget_width,
                row * widget_height, col * widget_width);
        }
    }

    layout_on_refresh(widget);
}

void layout_add(widget_t *widget, widget_t *new_widget, uint8_t row,
                uint8_t col) {
    uint8_t idx;

    layout_t *layout = (layout_t *)widget->data;

    if (row > layout->rows) {
        // Error
    }

    if (col > layout->cols) {
        // Error
    }

    idx = pos2idx(layout, row, col);
    layout->widgets[idx] = new_widget;
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
