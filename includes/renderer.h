#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED





#include <openindev.h>



bool rndr_create();
void rndr_resize(int width, int height);
void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void rndr_begin();
void rndr_end();
void rndr_draw_vertices_with_indices(float vertices[], uint32_t vFirst, uint32_t vCount, uint32_t indices[], uint32_t iCount);
void rndr_destroy();








#endif
