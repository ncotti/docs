#ifndef WIDGET_H_
#define WIDGET_H_

#include <ncurses.h>

/// @brief Widgets are defined by a list of common attributes in their "base",
/// and a list of custom attributes in their "data".
typedef struct widget_t widget_t;

/// @brief Common attributes to all widgets.
typedef struct widget_base_t widget_base_t;

/// @brief Return values for widget functions that may fail.
typedef enum widget_status_t {
    WIDGET_OK = 0,
    WIDGET_NULL = -1,
    WIDGET_WRONG_TYPE = -2,
    WIDGET_ENOMEM = -3,
} widget_status_t;

typedef widget_status_t (*widget_del_fn_t)(widget_t *);
typedef void (*widget_refresh_fn_t)(widget_t *, int, int, int, int);
typedef bool (*widget_on_focus_fn_t)(widget_t *, int);

/// @brief Widget types
typedef enum widget_type_t {
    WIDGET_TEXTBOX,
    WIDGET_BUTTON,
} widget_type_t;

typedef struct widget_border_t {
    chtype ls;
    chtype rs;
    chtype ts;
    chtype bs;
    chtype tl;
    chtype tr;
    chtype bl;
    chtype br;
} widget_border_t;

struct widget_base_t {
    widget_type_t type;
    WINDOW *window;
    widget_refresh_fn_t refresh_fn;
    widget_del_fn_t del_fn;
    widget_on_focus_fn_t on_focus_fn;
    int ypos;
    int xpos;
    int height;
    int width;
};

struct widget_t {
    widget_base_t base;
    void *data;
};

typedef struct pos_t {
    int ypos;
    int xpos;
} pos_t;

#endif // WIDGET_H_