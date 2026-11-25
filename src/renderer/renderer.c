#include <renderer.h>
#include <string.h>






void rndr_draw_cube(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture){
    float vertices[] = {
        -1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,

        -1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,

         1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,

        -1.0f,  1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f
    };
    renderobject_t object = {0};
    object.vertices = vertices;
    object.indices = NULL;
    memcpy(object.position, position, sizeof(vec3_t));
    memcpy(object.scale, scale, sizeof(vec3_t));
    memcpy(object.rotation, rotation, sizeof(vec3_t));
    object.amountOfVertices = 36;
    object.rotationAngle = rotationAngle;
    object.texture = texture;
    rndr_draw_vertices_3d(&object);
}

void rndr_draw_texture(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture){
    float vertices[] = {
         1.0f,  1.0f,  0.0f,        1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  0.0f,        1.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  0.0f,        0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f,         0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3
    };
    renderobject_t object = {0};
    object.vertices = vertices;
    object.indices = indices;
    memcpy(object.position, position, sizeof(vec3_t));
    memcpy(object.scale, scale, sizeof(vec3_t));
    memcpy(object.rotation, rotation, sizeof(vec3_t));
    object.amountOfVertices = 0;
    object.amountOfIndices = 6;
    object.rotationAngle = rotationAngle;
    object.texture = texture;

    rndr_draw_vertices_with_indices_3d(&object);
}


void rndr_draw_texture2(vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture){
    float vertices[] = {
         1.0f,  1.0f, 0.0f,         1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,         1.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,         0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, 0.0f,         1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,         0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,         0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f
    };
    renderobject_t object = {0};
    object.vertices = vertices;
    object.indices = NULL;
    memcpy(object.position, position, sizeof(vec3_t));
    memcpy(object.scale, scale, sizeof(vec3_t));
    memcpy(object.rotation, rotation, sizeof(vec3_t));
    object.amountOfVertices = 6;
    object.rotationAngle = rotationAngle;
    object.texture = texture;
    rndr_draw_vertices_3d(&object);
}
