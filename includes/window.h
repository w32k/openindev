#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED





#include <openindev.h>
#include <renderer.h>

typedef enum _keycodes {
    KEY_W, KEY_A, KEY_S, KEY_D, KEY_ESC,
    KEY_END_OF_ENUM
} keycodes_t;

bool win_create(const char* title, int width, int height);
bool win_should_close();
void win_swap_window();
void win_close();
uint64_t win_return_ticks();
uint64_t win_return_fps();
float win_return_dt();
bool win_is_key_down(keycodes_t key);
void win_return_rel_mouse_state(float* x, float* y);
void win_lock_mouse();
void win_unlock_mouse();
bool win_did_mouse_move();
void win_return_window_size(int* w, int* h);

#endif
