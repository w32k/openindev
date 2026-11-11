#include <window.h>
#include <SDL3/SDL.h>

#include <renderer.h>

static const SDL_Scancode keyCodeToSDLScanCodeMap[] = {
    [KEY_W] = SDL_SCANCODE_W,
    [KEY_A] = SDL_SCANCODE_A,
    [KEY_S] = SDL_SCANCODE_S,
    [KEY_D] = SDL_SCANCODE_D,
    [KEY_ESC] = SDL_SCANCODE_ESCAPE
};



// TODO: put all of this in a struct
// so its cleaner
static uint64_t g_lastTick = 0;
static uint64_t g_fps = 0;
static uint64_t g_fpsCounter = 0;
static uint64_t g_lastTime = 0;
static uint64_t g_curTime = 0;
static SDL_Window* g_sdlWindow = NULL;
static SDL_Event g_event = {0};
static bool g_mouseMoved = false;
#ifdef GRAPHICS_GLES2
static SDL_GLContext g_glContext = NULL;
#endif

bool win_create(const char* title, int width, int height){
    bool result = SDL_Init(SDL_INIT_VIDEO);
    if(result == false){
        DEBUG_FAIL("couldn't initialize SDL!\n");
        return false;
    }
    int flag = 0;
    #ifdef GRAPHICS_GLES2
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    flag = SDL_WINDOW_OPENGL;
    #endif

    DEBUG_INFO("Creating SDL3 Window\n");
    g_sdlWindow = SDL_CreateWindow(title, width, height, flag | SDL_WINDOW_RESIZABLE);
    if(g_sdlWindow == NULL){
        DEBUG_INFO("can't create window: %s\n", SDL_GetError());
        return false;
    }
    #ifdef GRAPHICS_GLES2
    g_glContext = SDL_GL_CreateContext(g_sdlWindow);
    if(g_glContext == NULL){
        DEBUG_INFO("can't create gl context: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_sdlWindow);
        SDL_Quit();
        return false;
    }
    SDL_GL_MakeCurrent(g_sdlWindow, g_glContext);
    #endif
    result = rndr_create();
    if(result == false){
        DEBUG_FAIL("couldn't setup renderer\n");
        SDL_DestroyWindow(g_sdlWindow);
        #ifdef GRAPHICS_GLES2
        SDL_GL_DestroyContext(g_glContext);
        #endif
        SDL_Quit();
        return false;
    }
    rndr_resize(width, height);
    g_lastTick = SDL_GetTicks();
    return true;
}

bool win_should_close(){
    g_mouseMoved = false;
    while(SDL_PollEvent(&g_event)){
        switch(g_event.type){
            case SDL_EVENT_QUIT:{
                return true;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:{
                rndr_resize(g_event.window.data1, g_event.window.data2);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:{
                g_mouseMoved = true;
                break;
            }
        }
    }
    return false;
}

uint64_t win_return_ticks(){
    return SDL_GetTicks();
}

void win_swap_window(){
    #ifdef GRAPHICS_GLES2
    SDL_GL_SwapWindow(g_sdlWindow);
    #endif
    g_lastTime = g_curTime;
    g_curTime = SDL_GetTicks();
    if(SDL_GetTicks() - g_lastTick >= 1000){
        g_fps = g_fpsCounter;
        DEBUG_INFO("FPS: " U64_PRINT " \n", g_fps);
        g_fpsCounter = 0;
        g_lastTick = SDL_GetTicks();
    } else g_fpsCounter++;
}

void win_close(){
    rndr_destroy();
    #ifdef GRAPHICS_GLES2
    SDL_GL_DestroyContext(g_glContext);
    #endif
    SDL_DestroyWindow(g_sdlWindow);
    SDL_Quit();
}

uint64_t win_return_fps(){
    return g_fps;
}

float win_return_dt(){
    return (float)(g_curTime - g_lastTime) / 1000.0f;
}
// a bit unsafe?
// TODO: find a way to make this a bit safer
bool win_is_key_down(keycodes_t key){
    if(key >= KEY_END_OF_ENUM) return false;
    const bool* keyboardState = SDL_GetKeyboardState(NULL);
    return keyboardState[keyCodeToSDLScanCodeMap[key]]; // uhh
}


void win_return_rel_mouse_state(float* x, float* y){
    SDL_GetRelativeMouseState(x, y);
}

void win_lock_mouse(){
    //SDL_HideCursor();
    SDL_SetWindowRelativeMouseMode(g_sdlWindow, true);
}

void win_unlock_mouse(){
    //SDL_ShowCursor();
    SDL_SetWindowRelativeMouseMode(g_sdlWindow, false);
}

bool win_did_mouse_move(){
    return g_mouseMoved;
}

void win_return_window_size(int* w, int* h){
    SDL_GetWindowSize(g_sdlWindow, w, h);
}
