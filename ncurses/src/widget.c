/***[Includes]****************************************************************/
#include "widget.h"

/***[Static variables]********************************************************/

/***[Static functions]********************************************************/

/***[Public functions]********************************************************/

void widget_init(widget_t *widget, void *data, widget_type_t type,
                 WINDOW *window, widget_on_refresh_fn_t on_refresh_fn,
                 widget_del_fn_t del_fn, widget_on_focus_fn_t on_focus_fn,
                 widget_on_lose_focus_fn_t on_lose_focus_fn,
                 widget_on_resize_t on_resize_fn) {

    widget->base.window = window;
    widget->data = data;
    widget->base.on_refresh_fn = on_refresh_fn;
    widget->base.del_fn = del_fn;
    widget->base.on_focus_fn = on_focus_fn;
    widget->base.on_lose_focus_fn = on_lose_focus_fn;
    widget->base.on_resize_fn = on_resize_fn;
    widget->base.type = type;
}
