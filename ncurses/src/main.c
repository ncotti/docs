/***[Includes]****************************************************************/
#include "main.h"
#include "button.h"
#include "color.h"
#include "layout.h"
#include "logger.h"
#include "textbox.h"
#include "widget.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/***[Static variables]********************************************************/

/***[Static functions prototypes]*********************************************/

/***[Static functions]********************************************************/
void button_press(void *params) {
    widget_t *text_widget = (widget_t *)params;
    textbox_set_color(text_widget, random() % 7);
}

/***[Public functions]********************************************************/
int main(void) {
    logger_init();
    widget_t *layout = layout_new(stdscr, 2, 2);
    // widget_t *text_ul =
    //     textbox_new("This text is very large, so it will likely take "
    //                 "more than one row to be written. Also, since I am "
    //                 "here, I would like to say gambatte!, you can do it!");

    widget_t *text_ul = textbox_new("UL");
    widget_t *text_dl = textbox_new("DL");
    widget_t *text_dr = textbox_new("DR");

    widget_t *button =
        button_new(button_press, (void *)text_dr, "hi", '9', COLOR_CYAN_BLACK);

    layout_add(layout, text_ul, 0, 0);
    layout_add(layout, button, 0, 1);
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
