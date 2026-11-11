#include <window.h>
#include <renderer.h>
#include <math.h>
#include <cglm/cglm.h>

#define SENS 0.1f

int main(){
    bool result = win_create("Sift", 800, 600);
    if(result == false){
        return -1;
    }
    camera_t camera = {
        {0.0f, 0.0f, 3.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        90.0f
    };
    result = res_load_all_textures();
    if(result == false){
        return -2;
    }
    float i = 0.0f;
    float yaw = 0;
    float pitch = 0;
    vec3 r = {0.0f};
    bool isLocked = true;
    bool mouseInit = false;
    win_lock_mouse();
    while(!win_should_close()){
        float speed = 2.5f * win_return_dt();
        if(win_is_key_down(KEY_W)){
            glm_vec3_scale(camera.center, speed, r);
            glm_vec3_add(camera.eye, r, camera.eye);
        }
        if(win_is_key_down(KEY_S)){
            glm_vec3_scale(camera.center, speed, r);
            glm_vec3_sub(camera.eye, r, camera.eye);
        }
        if(win_is_key_down(KEY_A)){
            glm_vec3_cross(camera.center, camera.up, r);
            glm_vec3_normalize(r);
            glm_vec3_scale(r, speed, r);
            glm_vec3_sub(camera.eye, r, camera.eye);
        }
        if(win_is_key_down(KEY_D)){
            glm_vec3_cross(camera.center, camera.up, r);
            glm_vec3_normalize(r);
            glm_vec3_scale(r, speed, r);
            glm_vec3_add(camera.eye, r, camera.eye);
        }
        if(win_is_key_down(KEY_ESC)){
            if(isLocked == true){
                win_unlock_mouse();
                isLocked = false;
            } else {
                win_lock_mouse();
                isLocked = true;
            }
        }
        if(isLocked){
            if(win_did_mouse_move()){
                float xPos = 0;
                float yPos = 0;
                win_return_rel_mouse_state(&xPos, &yPos);
                yPos *= -1;
                if(mouseInit){
                    yaw += xPos * SENS;
                    pitch += yPos * SENS;
                    vec3 direction = {0};
                    direction[0] = cosf(RADIAN(yaw)) * cosf(RADIAN(pitch));
                    direction[1] = sinf(RADIAN(pitch));
                    direction[2] = sinf(RADIAN(yaw)) * cosf(RADIAN(pitch));
                    glm_vec3_normalize(direction);
                    glm_vec3_copy(direction, camera.center);
                } else mouseInit = true;

            }
        }

        rndr_clear(180, 225, 255, 255);
        rndr_begin(&camera);
        rndr_draw_cube((vec3_t){0.0f, 0.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, 0, RESTYPE_DIAMOND_BLOCK);
        rndr_draw_texture((vec3_t){1.0f, 1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
        rndr_draw_texture((vec3_t){-1.0f, -1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
        rndr_draw_texture((vec3_t){1.0f, -1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);
        rndr_draw_texture((vec3_t){-1.0f, 1.0f, 0.0f}, (vec3_t){0.5f, 0.5f, 0.5f}, (vec3_t){1.0f, 1.0f, 1.0f}, i, RESTYPE_DIAMOND_ITEM);

        i += 100.0f * win_return_dt();
        rndr_end();
        win_swap_window();
    }
    res_unload_all_loaded_textures();
    win_close();
    return 0;
}
