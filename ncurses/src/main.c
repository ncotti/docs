#include "color.h"
#include "layout.h"
#include "textbox.h"
#include <ncurses.h>

int main(void) {
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

    layout_show(layout);
    layout_del(layout);

    textbox_t *textbox = (textbox_t *)text_ul->data;
    printf("%d, %d\n", LINES, COLS);
    printf("COLORS: %d, COLOR_PAIRS: %d\n", COLORS, COLOR_PAIRS);
    printf("%d, %d\n", textbox->text_rows, textbox->text_words);
    return 0;
}
