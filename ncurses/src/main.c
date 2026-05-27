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

    text_ul = textbox_new("UL", true);
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
        }

        layout_refresh(layout);
    } while (input != 'q');

    layout_del(layout);

    printf("%d, %d\n", LINES, COLS);
    return 0;
}
