#include <renderer.h>
#include <glad/gles2.h>
#ifdef WINDOW_SDL3
#include <SDL3/SDL.h>
#endif
// TODO: use cglm instead
// or make it seperate between a C and H file
#include <linmath.h>
#include <strings.h>
// TODO: put this in seperate files
// static const char* vertexSource = "attribute vec4 vPosition;\n"
//                                   "void main(){\n"
//                                   "  gl_Position = vPosition;\n"
//                                   "  gl_PointSize = 10.0;\n"
//                                   "}\n";

// static const char* fragmentSource = "void main(){\n"
//                                     "gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);\n"
//                                     "}\n";

static int shaderProgram = 0;
static float g_Width = 0;
static float g_Height = 0;

static char* load_shader_from_file(const char* path){
    char* buffer = NULL;
    FILE* fp = fopen(path, "rb");
    if(fp == NULL){
        DEBUG_FAIL("couldn't load shader with path %s\n", path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    //DEBUG_INFO("fileSize is %lld\n", fileSize);
    buffer = calloc(fileSize + 1, sizeof(char));
    if(buffer == NULL){
        DEBUG_FAIL("couldn't allocate memory for shader\n");
        goto LOAD_SHADER_EXIT;
    }
    size_t returnedSize = fread((void*)buffer, sizeof(char), fileSize, fp);
    if(returnedSize != fileSize){
        //DEBUG_FAIL("file size (%lld) and size read (%lld) don't match\n", fileSize, returnedSize);
        free(buffer);
        goto LOAD_SHADER_EXIT;
    }
    //DEBUG_INFO("loaded shader %s with size of %lld\n", path, returnedSize);
LOAD_SHADER_EXIT:
    fclose(fp);
    return buffer;
}

static unsigned int compile_shader(int type, const char* source){
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

static unsigned int create_shader_program(const char* vertexPath, const char* fragPath){
    unsigned int program = 0;
    char* vertexShaderSource = load_shader_from_file(vertexPath);
    if(vertexShaderSource == NULL){
        DEBUG_FAIL("couldn't load vertex shader\n");
        return 0;
    }
    unsigned int vertexShader = compile_shader(GL_VERTEX_SHADER, vertexShaderSource);
    if(vertexShader == 0){
        DEBUG_FAIL("couldn't compile vertex shader\n");
        goto EXIT1;
    }
    char* fragShaderSource = load_shader_from_file(fragPath);
    if(fragShaderSource == NULL){
        DEBUG_FAIL("couldn't load shader!\n");
        goto EXIT2;
    }
    unsigned int fragShader = compile_shader(GL_FRAGMENT_SHADER, fragShaderSource);
    if(fragShader == 0){
        DEBUG_FAIL("couldn't compile vertex shader\n");
        goto EXIT3;
    }
    program = link_program(vertexShader, fragShader);
    if(program == 0){
        DEBUG_FAIL("couldn't link shader program\n");
    }
    glDeleteShader(fragShader);
EXIT3:
    free(fragShaderSource);
EXIT2:
    glDeleteShader(vertexShader);
EXIT1:
    free(vertexShaderSource);
    return program;
}


bool rndr_create(){
    #ifdef WINDOW_SDL3
    if(gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress) == 0){
        DEBUG_FAIL("couldn't initialize GLAD\n");
        return false;
    }
    printf("VENDOR: %s\nVERSION: %s\nRENDERER: %s\n",
            (char*)glGetString(GL_VENDOR), (char*)glGetString(GL_VERSION), (char*)glGetString(GL_RENDERER));
    #endif
    shaderProgram = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");
    if(shaderProgram == 0){
        DEBUG_FAIL("couldn't create shader program\n");
        return false;
    }
    return true;
}

void rndr_resize(int width, int height){
    glViewport(0, 0, width, height);
    g_Width = (float)width;
    g_Height = (float)height;
}

void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha){
    glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void rndr_begin(){
    glUseProgram(shaderProgram);
    mat4x4 perspective;
    mat4x4_identity(perspective);
    mat4x4_perspective(perspective, 90, g_Width/g_Height, 0.1f, 100.0f);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "perspective");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const float*)perspective);

    mat4x4 view;
    mat4x4_identity(view);
    mat4x4_translate(view, 0.0f, 0.0f, -3.0f);
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float*)view);
}

void rndr_draw_vertices_with_indices(float vertices[], uint32_t vFirst, uint32_t vCount, uint32_t indices[], uint32_t iCount){
    glVertexAttribPointer(vFirst, vCount, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, indices);
}

void rndr_draw_square(float x, float y, float size, float rotation){
    (void)x;
    (void)y;
    (void)size;
    (void)rotation;
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3
    };
    mat4x4 v1;
    mat4x4_identity(v1);
    mat4x4 v2;

    mat4x4 result;
    mat4x4_scale(v2, v1, size);
    mat4x4_translate(v2, x, y, 0.2f);
    mat4x4_rotate_X(result, v2, rotation);



    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const float*)result);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
}

void rndr_end(){
    // stub
}

void rndr_destroy(){
    if(shaderProgram != 0) glDeleteProgram(shaderProgram);
}
