#include <renderer.h>
#include <glad/gles2.h>
#ifdef WINDOW_SDL3
#include <SDL3/SDL.h>
#endif

// TODO: put this in seperate files
static const char* vertexSource = "attribute vec4 vPosition;\n"
                                  "void main(){\n"
                                  "  gl_Position = vPosition;\n"
                                  "  gl_PointSize = 10.0;\n"
                                  "}\n";

static const char* fragmentSource = "void main(){\n"
                                    "gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);\n"
                                    "}\n";

static int fragShader = 0;
static int vertexShader = 0;
static int shaderProgram = 0;


static unsigned int create_shader(int type, const char* source){
    int compiled = 1;
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
            DEBUG_FAIL("shader compile error: %s\n%s\n", info, source);
            free(info);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static unsigned int link_program(int vShader, int fShader){
    int compiled = 1;
    int prog = glCreateProgram();
    glAttachShader(prog, vShader);
    glAttachShader(prog, fShader);
    glBindAttribLocation(prog, 0, "vPosition");
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
        int infoLen = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1){
            char* info = calloc(infoLen, sizeof(char));
            glGetProgramInfoLog(prog, infoLen, NULL, info);
            DEBUG_FAIL("link compile error: %s\n", info);
            free(info);
        }
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}



bool rndr_create(){
    #ifdef WINDOW_SDL3
    if(gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress) == 0){
        DEBUG_FAIL("couldn't initialize GLAD\n");
        return false;
    }
    #endif
    vertexShader = create_shader(GL_VERTEX_SHADER, vertexSource);
    fragShader = create_shader(GL_FRAGMENT_SHADER, fragmentSource);
    if(vertexShader == 0 || fragShader == 0) return false;
    shaderProgram = link_program(vertexShader, fragShader);
    if(shaderProgram == 0) return false;
    return true;
}

void rndr_resize(int width, int height){
    glViewport(0, 0, width, height);
}

void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha){
    glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void rndr_begin(){
    glUseProgram(shaderProgram);
}

void rndr_draw_vertices_with_indices(float vertices[], uint32_t vFirst, uint32_t vCount, uint32_t indices[], uint32_t iCount){
    glVertexAttribPointer(vFirst, vCount, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, indices);
}

void rndr_end(){
    // stub
}

void rndr_destroy(){
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    glDeleteProgram(shaderProgram);
}
