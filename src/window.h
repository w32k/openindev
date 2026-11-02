#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED





#include "openindev.h"

typedef bool (*winonload_t)();
typedef bool (*winonresize_t)(size_t width, size_t height);
typedef bool (*winondraw_t)();
typedef bool (*winonupdate_t)();
typedef bool (*winonkeypress_t)(char keyCode);
typedef bool (*winonclose_t)();

typedef struct windowinfo {
    const char* title;
    size_t width;
    size_t height;
    winonload_t onLoad;
    winonresize_t onResize;
    winondraw_t onDraw;
    winonupdate_t onUpdate;
    winonkeypress_t onKeypress;
    winonclose_t onClose;
} windowinfo_t;


bool win_create_window(windowinfo_t* winInfo);
const windowinfo_t* win_return_window_info();








#endif
