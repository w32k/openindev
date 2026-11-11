#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED





#include <openindev.h>
#include <resources.h>
#include <math.h>

typedef struct camera {
    vec3_t position;
    vec3_t target;
    vec3_t up;
    float fov;
} camera_t;

#define RADIAN(x) (x * (M_PI / 180.0f))

bool rndr_create();
void rndr_resize(int width, int height);
void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void rndr_begin();
void rndr_end();
void rndr_begin_3d(camera_t* camera);
void rndr_end_3d();
void rndr_draw_vertices_with_indices_3d(float vertices[], uint32_t indices[], float indicesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_vertices_with_indices_2d(float vertices[], uint32_t indices[], float indicesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_vertices_3d(float vertices[], int verticesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_texture_3d(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_texture_2d(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);

void rndr_draw_cube(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
handle_t rndr_load_texture(const char* path);
void rndr_unload_texture(handle_t textureID);
void rndr_destroy();








#endif
