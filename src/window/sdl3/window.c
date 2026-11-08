#include <window.h>
#include <SDL3/SDL.h>

#include <renderer.h>


// static window_t* g_Window;
// static SDL_Window* window = NULL;
// #ifdef GRAPHICS_GLES2
// static SDL_GLContext context = NULL
// #endif



int win_create(window_t* window){
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_VERBOSE);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_VERBOSE);
    bool result = SDL_Init(SDL_INIT_VIDEO);
    if(result == false){
        DEBUG_FAIL("couldn't initialize SDL!\n");
        return -1;
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
    SDL_Window* sdlWindow = SDL_CreateWindow(window->title, window->width, window->height, flag | SDL_WINDOW_RESIZABLE);
    if(sdlWindow == NULL){
        DEBUG_INFO("can't create window: %s\n", SDL_GetError());
        goto END_4;
        return false;
    }
    #ifdef GRAPHICS_GLES2
    SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
    if(glContext == NULL){
        DEBUG_INFO("can't create gl context: %s\n", SDL_GetError());
        goto END_3;
        return false;
    }
    SDL_GL_MakeCurrent(sdlWindow, glContext);
    #endif
    result = rndr_create();
    if(result == false){
        DEBUG_FAIL("couldn't setup renderer\n");
        goto END_2;
    }
    rndr_resize(window->width, window->height);

    result = window->init(window->userData);
    if(result == false) goto END_1;
    SDL_Event event;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT:{
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED:{
                    rndr_resize(event.window.data1, event.window.data2);
                    bool result = window->resize(window->userData, event.window.data1, event.window.data2);
                    if(result == false) break;
                    break;
                }
            }
        }
        rndr_begin();
        bool result = window->render(window->userData);
        if(result == false) break;
        rndr_end();
        #ifdef GRAPHICS_GLES2
        SDL_GL_SwapWindow(sdlWindow);
        #endif
    }
    window->cleanup(window->userData);
END_1:
    rndr_destroy();
END_2:
    #ifdef GRAPHICS_GLES2
    SDL_GL_DestroyContext(glContext);
    #endif
END_3:
    SDL_DestroyWindow(sdlWindow);
END_4:
    SDL_Quit();
    return 0;
}
