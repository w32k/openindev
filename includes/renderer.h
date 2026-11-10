#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED





#include <openindev.h>
#include <resources.h>


bool rndr_create();
void rndr_resize(int width, int height);
void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void rndr_begin();
void rndr_end();
void rndr_draw_vertices_with_indices(float vertices[], uint32_t indices[], float indicesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_vertices(float vertices[], int verticesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_texture(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_cube(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
handle_t rndr_load_texture(const char* path);
void rndr_unload_texture(handle_t textureID);
void rndr_destroy();








#endif
