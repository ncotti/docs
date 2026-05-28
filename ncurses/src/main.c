#include "color.h"
#include "layout.h"
#include "textbox.h"
#include <ncurses.h>

int main(void) {
    int input;

    layout_t *layout;
    widget_t *text_ul;
    widget_t *text_ur;
    widget_t *text_dl;
    widget_t *text_dr;

    layout = layout_new(stdscr, 2, 2);

    text_ul = textbox_new("This text is very large, so it will likely take "
                          "more than one row to be written. Also, since I am "
                          "here, I would like to say gambatte!, you can do it!",
                          true);
    text_ur = textbox_new("UR", true);
    text_dl = textbox_new("DL", false);
    text_dr = textbox_new("DR", true);

    layout_add(layout, text_ul, 0, 0);
    layout_add(layout, text_ur, 0, 1);
    layout_add(layout, text_dl, 1, 0);
    layout_add(layout, text_dr, 1, 1);

    layout_refresh(layout);

    do {
        input = getch();

        switch (input) {
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
            textbox_set_color(text_ul, COLOR_GREEN_BLACK);
            break;
        }

        case 'b': {
            textbox_set_color(text_ul, COLOR_YELLOW_BLACK);
            break;
        }

        case 'c': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_TOP_LEFT);
            break;
        }

        case 'd': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_TOP_CENTER);
            break;
        }

        case 'e': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_TOP_RIGHT);
            break;
        }

        case 'f': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_MIDDLE_LEFT);
            break;
        }

        case 'g': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_MIDDLE_CENTER);
            break;
        }

        case 'h': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_MIDDLE_RIGHT);
            break;
        }

        case 'i': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_BOTTOM_LEFT);
            break;
        }

        case 'j': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_BOTTOM_CENTER);
            break;
        }

        case 'k': {
            textbox_set_alignment(text_ul, TEXT_ALIGN_BOTTOM_RIGHT);
            break;
        }
        }

        layout_refresh(layout);
    } while (input != 'q');

    layout_del(layout);

    printf("%d, %d\n", LINES, COLS);
    printf("COLORS: %d, COLOR_PAIRS: %d\n", COLORS, COLOR_PAIRS);
    return 0;
}
