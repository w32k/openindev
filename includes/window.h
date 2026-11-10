#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED





#include <openindev.h>


typedef bool (*wininit_t)(void* data);
typedef bool (*winonresize_t)(void* data, int width, int height);
typedef bool (*winrender_t)(void* data);
typedef bool (*wincleanup_t)(void* data);

typedef struct window {
    const char* title;
    wininit_t init;
    winonresize_t resize;
    winrender_t render;
    wincleanup_t cleanup;
    void* userData;
    int width;
    int height;
} window_t;


int win_create(window_t* window);
uint32_t win_return_fps();
float win_return_dt();






#endif
