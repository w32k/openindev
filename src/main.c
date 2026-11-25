#include <window.h>
#include <renderer.h>
#include <math.h>
#include <cglm/cglm.h>
#include <FastNoiseLite.h>
#include <math.h>

#define SENS 0.1f


void controls(camera_t* camera){
    vec3 r = {0.0f};
    float speed = 2.5f * win_return_dt();
    if(win_is_key_down(KEY_W)){
        glm_vec3_scale(camera->target, speed, r);
        glm_vec3_add(camera->position, r, camera->position);
    }
    if(win_is_key_down(KEY_S)){
        glm_vec3_scale(camera->target, speed, r);
        glm_vec3_sub(camera->position, r, camera->position);
    }
    if(win_is_key_down(KEY_A)){
        glm_vec3_cross(camera->target, camera->up, r);
        glm_vec3_normalize(r);
        glm_vec3_scale(r, speed, r);
        glm_vec3_sub(camera->position, r, camera->position);
    }
    if(win_is_key_down(KEY_D)){
        glm_vec3_cross(camera->target, camera->up, r);
        glm_vec3_normalize(r);
        glm_vec3_scale(r, speed, r);
        glm_vec3_add(camera->position, r, camera->position);
    }
    if(win_did_mouse_move()){
        float xPos = 0;
        float yPos = 0;
        win_return_rel_mouse_state(&xPos, &yPos);
        yPos *= -1;
        camera->yaw += xPos * SENS;
        camera->pitch += yPos * SENS;
        vec3 direction = {0};
        direction[0] = cosf(RADIAN(camera->yaw)) * cosf(RADIAN(camera->pitch));
        direction[1] = sinf(RADIAN(camera->pitch));
        direction[2] = sinf(RADIAN(camera->yaw)) * cosf(RADIAN(camera->pitch));
        glm_vec3_normalize(direction);
        glm_vec3_copy(direction, camera->target);
    }
}

int main(){
    bool result = win_create("Sift", 800, 600);
    if(result == false){
        return -1;
    }
    camera_t camera = {
        {0.0f, 0.0f, -3.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f},
        90.0f, 0.0f, 0.0f
    };
    result = res_load_all_textures();
    if(result == false){
        return -2;
    }
    float i = 0.0f;

    win_lock_mouse();


    while(!win_should_close()){

        rndr_clear(180, 225, 255, 255);
        rndr_begin();
        controls(&camera);
        if(win_is_key_down(KEY_ESC)){
            return false;
        }
        rndr_begin_3d(&camera);

        rndr_draw_cube((vec3_t){0, 0, 0}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, 0, RESTYPE_DIAMOND_BLOCK);
        rndr_draw_texture((vec3_t){1.0f, 1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_SWORD);
        rndr_draw_texture((vec3_t){1.0f, -1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
        rndr_draw_texture((vec3_t){-1.0f, 1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
        rndr_draw_texture((vec3_t){-1.0f, -1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_SWORD);

        rndr_end_3d();

        rndr_end();
        win_swap_window();
        i += 100.0f * win_return_dt();
    }
    res_unload_all_loaded_textures();
    win_close();
    return 0;
}
