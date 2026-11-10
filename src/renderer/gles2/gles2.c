#include <renderer.h>
#include <glad/gles2.h>
#ifdef WINDOW_SDL3
#include <SDL3/SDL.h>
#endif
// TODO: use cglm instead
// or make it seperate between a C and H file
#include <cglm/cglm.h>
#include <strings.h>
#include <stb_image.h>
#include <resources.h>

static int shaderProgram = 0;
static float g_width = 0;
static float g_height = 0;


// this entire file is so ugly
// i need to clean this up

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
    stbi_set_flip_vertically_on_load(true);
    #ifdef WINDOW_SDL3
    if(gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress) == 0){
        DEBUG_FAIL("couldn't initialize GLAD\n");
        return false;
    }
    DEBUG_INFO("\n\tVENDOR: %s\n\tVERSION: %s\n\tRENDERER: %s\n",
            (char*)glGetString(GL_VENDOR), (char*)glGetString(GL_VERSION), (char*)glGetString(GL_RENDERER));
    #endif
    shaderProgram = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");
    if(shaderProgram == 0){
        DEBUG_FAIL("couldn't create shader program\n");
        return false;
    }


    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void rndr_resize(int width, int height){
    glViewport(0, 0, width, height);
    g_width = (float)width;
    g_height = (float)height;
}

void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha){
    glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rndr_begin(){
    glUseProgram(shaderProgram);
    mat4_t perspective;
    glm_mat4_identity(perspective);
    glm_perspective(90.0f * (M_PI / 180.0f), g_width/g_height, 0.1f, 100.0f, perspective);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "perspective");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const float*)perspective);

    vec3_t eye = {0};
    eye[2] = 3.0f;
    vec3_t center = {0};
    vec3_t up = {0};
    up[1] = 1.0f;
    mat4_t view;

    glm_mat4_identity(view);
    glm_lookat(eye, center, up, view);
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float*)view);
}


void rndr_draw_vertices_with_indices(float vertices[], uint32_t indices[], float indicesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture)
{

    (void)rotationAngle;
    (void)rotation;
    mat4_t result;
    glm_translate_make(result, position);
    glm_rotate(result, rotationAngle * (M_PI / 180.0f), rotation);
    glm_scale(result, scale);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const float*)result);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, texCoordSize, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res_get_texture(texture));
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, indices);
}

void rndr_draw_vertices(float vertices[], int verticesSize, float texCoords[], float texCoordSize,
                     vec3_t position, vec3_t scale, vec3_t rotation, float rotationAngle, resourcetype_t texture)
{

    (void)rotationAngle;
    (void)rotation;
    (void)verticesSize;
    mat4_t result;
    glm_translate_make(result, position);
    glm_rotate(result, rotationAngle * (M_PI / 180.0f), rotation);
    glm_scale(result, scale);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const float*)result);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, texCoordSize, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res_get_texture(texture));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


// TODO: optimize
handle_t rndr_load_texture(const char* path){
    handle_t texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    uint8_t* data = stbi_load(path, &width, &height, &nrChannels, 4);
    if(data == NULL){
        DEBUG_INFO("can't load texture\n");
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return (handle_t)texture;
}

void rndr_unload_texture(handle_t textureID){
    if(textureID != 0) glDeleteTextures(1, &textureID);
}
// "assets/minecraft/textures/blocks/dirt.png"

void rndr_end(){
    // stub
}

void rndr_destroy(){
    if(shaderProgram != 0) glDeleteProgram(shaderProgram);
}
