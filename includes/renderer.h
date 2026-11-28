#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED





#include <vermilion.h>
#include <resources.h>
#include <math.h>

typedef struct camera {
    vec3_t position;
    vec3_t target;
    vec3_t up;
    float fov;
    float yaw;
    float pitch;
} camera_t;

typedef struct renderobject {
    float* vertices;
    unsigned int* indices;
    vec3_t position;
    vec3_t scale;
    vec3_t rotation;
    float amountOfVertices;
    float amountOfIndices;
    float rotationAngle;
    resourcetype_t texture;
} renderobject_t;

#define RADIAN(x) (x * (M_PI / 180.0f))

// platform specific functions
bool rndr_create();
void rndr_resize(int width, int height);
void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void rndr_begin();
void rndr_end();
void rndr_begin_3d(camera_t* camera);
void rndr_end_3d();
void rndr_draw_vertices_with_indices_3d(renderobject_t* object);
void rndr_draw_vertices_3d(renderobject_t* object);
shandle_t rndr_load_texture(const char* path);
void rndr_start_texture_array_loading();
bool rndr_load_texture_to_texture_array(const char* path, unsigned int index);
void rndr_end_texture_array_loading();
void rndr_unload_texture(shandle_t textureID);
void rndr_destroy();

void rndr_draw_cube_ex(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture[]);
void rndr_draw_cube(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_texture(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);
void rndr_draw_texture2(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture);








#endif
