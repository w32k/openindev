#include <window.h>
#include <SDL3/SDL.h>

#include <renderer.h>


// static window_t* g_Window;
// static SDL_Window* window = NULL;
// #ifdef GRAPHICS_OPENGLES_20
// static SDL_GLContext context = NULL
// #endif



int win_create(window_t* window){
    SDL_Init(0);
    int flag = 0;
    #ifdef GRAPHICS_OPENGLES_20
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    flag = SDL_WINDOW_OPENGL;
    #endif
    SDL_Window* sdlWindow = SDL_CreateWindow(window->title, window->width, window->height, flag | SDL_WINDOW_RESIZABLE);
    if(sdlWindow == NULL){
        return false;
    }
    #ifdef GRAPHICS_OPENGLES_20
    SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
    if(glContext == NULL){
        return false;
    }
    SDL_GL_MakeCurrent(sdlWindow, glContext);
    #endif
    bool result = rndr_create();
    if(result == false) return -2;
    rndr_resize(window->width, window->height);

    window->init(window->userData);
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
                    window->resize(window->userData, event.window.data1, event.window.data2);
                    break;
                }
            }
        }
        rndr_begin();
        window->render(window->userData);
        rndr_end();
        #ifdef GRAPHICS_OPENGLES_20
        SDL_GL_SwapWindow(sdlWindow);
        #endif
    }
    window->cleanup(window->userData);
    rndr_destroy();
    #ifdef GRAPHICS_OPENGLES_20
    SDL_GL_DestroyContext(glContext);
    #endif
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    return 0;
}
