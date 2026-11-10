#include <renderer.h>
#include <resources.h>

#include <window.h>


static float i = 0;

bool init(void* data){
    (void)data;
    printf("Sift Version %s, Implementing %s\n", VER, IMPL);
    bool result = res_load_all_textures();
    if(result == false){
        return false;
    }
    return true;
}

bool resize(void* data, int width, int height){
    (void)data;
    (void)width;
    (void)height;
    return true;
}

bool render(void* data){
    (void)data;
    rndr_clear(0, 1.0f, 0, 0);
    rndr_draw_cube((vec3_t){0.0f, 0.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_BLOCK);
    rndr_draw_texture((vec3_t){1.0f, 1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
    rndr_draw_texture((vec3_t){-1.0f, -1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
    rndr_draw_texture((vec3_t){1.0f, -1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
    rndr_draw_texture((vec3_t){-1.0f, 1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);

    i += 100.0f * win_return_dt();
    return true;
}

bool cleanup(void* data){
    (void)data;
    res_unload_all_loaded_textures();
    return true;
}

int main(){
    window_t window = {
        "OpenIndev",
        init, resize, render, cleanup,
        NULL, 600, 600
    };
    return win_create(&window);
}
