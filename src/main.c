#include <SDL3/SDL.h>
#include <glad/gles2.h>

#include <stdio.h>
#include <stdlib.h>

const char* vs = "attribute vec4 vPosition;\n"
                 "void main(){\n"
                 "  gl_Position = vPosition;\n"
                 "  gl_PointSize = 10.0;\n"
                 "}\n";

const char* fs = "void main(){\n"
                 "gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
                 "}\n";



unsigned int load_shader(int type, const char* source){
    int compiled = 0;
    unsigned int shader = glCreateShader(type);
    if(shader == 0) return 0;
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
        int infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1){
            char* info = calloc(infoLen, sizeof(char));
            glGetShaderInfoLog(shader, infoLen, NULL, info);
            printf("shader compile error: %s\n%s\n", info, source);
            free(info);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int link_program(int vShader, int fShader){
    int prog = glCreateProgram();
    glAttachShader(prog, vShader);
    glAttachShader(prog, fShader);
    glBindAttribLocation(prog, 0, "vPosition");
    glLinkProgram(prog);
    return prog;
}


int main(){
    SDL_Init(0);
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_Window* window = SDL_CreateWindow("OpenIndev", 800, 800, SDL_WINDOW_OPENGL);
    if(window == NULL){
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(context == NULL){
        return -2;
    }
    SDL_GL_MakeCurrent(window, context);
    if(gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress) == 0) return -2;
    glViewport(0, 0, 800, 800);
    SDL_Event event = {0};
    bool running = true;


    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };

    const unsigned char indices[] = {
        0, 1, 2, 0, 2, 3
    };

    int vShader = load_shader(GL_VERTEX_SHADER, vs);
    int fShader = load_shader(GL_FRAGMENT_SHADER, fs);
    if(vShader == 0 || fShader == 0) return false;
    int prog = link_program(vShader, fShader);

    while(running){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT:{
                    running = false;
                    break;
                }
            }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

        SDL_GL_SwapWindow(window);
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    glDeleteProgram(prog);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
