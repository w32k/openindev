#include "window.h"
#include "graphics.h"


bool on_load(){
    gfx_init();
    return TRUE;
}

bool on_draw(){
    gfx_draw_triangle();
    return TRUE;
}

bool on_resize(size_t width, size_t height){
    gfx_resize(width, height);
    return TRUE;
}
bool on_close(){
    gfx_destroy();
    return TRUE;
}


int main(){
    windowinfo_t window = {
        "OpenIndev", 800, 600,
        NULL, NULL, NULL,
        NULL, NULL, NULL
    };
    window.onDraw = on_draw;
    window.onLoad = on_load;
    window.onClose = on_close;
    window.onResize = on_resize;
    win_create_window(&window);
    return 0;
}
