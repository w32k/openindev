#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED





#include <openindev.h>


typedef void (*wininit_t)(void* data);
typedef void (*winonresize_t)(void* data, int width, int height);
typedef void (*winrender_t)(void* data);
typedef void (*wincleanup_t)(void* data);

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







#endif
