#include <renderer.h>
#include <glad/gles2.h>
#ifdef WINDOW_SDL3
#include <SDL3/SDL.h>
#endif
#include <cglm/cglm.h>
#include <strings.h>
#include <stb_image.h>
#include <resources.h>
#include <window.h>

struct renderpipeline {
    int shaderProgram;
    int projectionUniform;
    int transformUniform;
    int viewUniform;
    handle_t textureArray;
};

static struct renderpipeline g_pipeline3D = {0};


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
    // DEBUG_INFO("\n\tVENDOR: %s\n\tVERSION: %s\n\tRENDERER: %s\n",
    //         (char*)glGetString(GL_VENDOR), (char*)glGetString(GL_VERSION), (char*)glGetString(GL_RENDERER));
    DEBUG_INFO("Starting renderer..\n");
    DEBUG_INFO("VENDOR: %s\n", (char*)glGetString(GL_VENDOR));
    DEBUG_INFO("VERSION: %s\n", (char*)glGetString(GL_VERSION));
    DEBUG_INFO("RENDER DEVICE: %s\n", (char*)glGetString(GL_RENDERER));
    DEBUG_INFO("SHADER: %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    #endif
    g_pipeline3D.shaderProgram = create_shader_program("shaders/default.vsh", "shaders/default.fsh");
    if(g_pipeline3D.shaderProgram == 0){
        DEBUG_FAIL("couldn't create shader program for g_pipeline3D\n");
        return false;
    }
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(g_pipeline3D.shaderProgram);
    g_pipeline3D.projectionUniform = glGetUniformLocation(g_pipeline3D.shaderProgram, "perspective");
    g_pipeline3D.transformUniform = glGetUniformLocation(g_pipeline3D.shaderProgram, "transform");
    g_pipeline3D.viewUniform = glGetUniformLocation(g_pipeline3D.shaderProgram, "view");

    glUseProgram(0);
    return true;
}

void rndr_resize(int width, int height){
    glViewport(0, 0, width, height);
}

void rndr_clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha){
    glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rndr_begin(){

}

void rndr_end(){
    glUseProgram(0);
}

void rndr_begin_3d(camera_t* camera){
    glUseProgram(g_pipeline3D.shaderProgram);
    mat4_t perspective;
    glm_mat4_identity(perspective);
    int width, height = 0;
    win_return_window_size((int*)&width, &height);
    glm_perspective(RADIAN(camera->fov), (float)width/(float)height, 0.1f, 100.0f, perspective);
    glUniformMatrix4fv(g_pipeline3D.projectionUniform, 1, GL_FALSE, (const float*)perspective);

    mat4 view;
    vec3 eyePlusCenter;
    glm_mat4_identity(view);
    glm_vec3_add(camera->position, camera->target, eyePlusCenter);
    glm_lookat(camera->position, eyePlusCenter, camera->up, view);
    glUniformMatrix4fv(g_pipeline3D.viewUniform, 1, GL_FALSE, (const float*)view);
}


void rndr_draw_vertices_with_indices_3d(renderobject_t* object){
    mat4_t result;
    glm_translate_make(result, object->position);
    glm_rotate(result, object->rotationAngle * (M_PI / 180.0f), object->rotation);
    glm_scale(result, object->scale);
    glUniformMatrix4fv(g_pipeline3D.transformUniform, 1, GL_FALSE, (const float*)result);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), object->vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)((uintptr_t)object->vertices + (3 * sizeof(float))));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)((uintptr_t)object->vertices + (5 * sizeof(float))));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)((uintptr_t)object->vertices + (9 * sizeof(float))));
    glEnableVertexAttribArray(3);

    glActiveTexture(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLE_STRIP, object->amountOfIndices, GL_UNSIGNED_INT, object->indices);
}

void rndr_draw_vertices_3d(renderobject_t* object){
    mat4_t result;
    glm_translate_make(result, object->position);
    glm_rotate(result, RADIAN(object->rotationAngle), object->rotation);
    glm_scale(result, object->scale);
    glUniformMatrix4fv(g_pipeline3D.transformUniform, 1, GL_FALSE, (const float*)result);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), object->vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)((uintptr_t)object->vertices + (3 * sizeof(float))));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)((uintptr_t)object->vertices + (5 * sizeof(float))));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)((uintptr_t)object->vertices + (9 * sizeof(float))));
    glEnableVertexAttribArray(3);

    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, object->amountOfVertices);
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

void rndr_start_texture_array_loading(){
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &g_pipeline3D.textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, g_pipeline3D.textureArray);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 16, 16, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

bool rndr_load_texture_to_texture_array(const char* path, unsigned int index){
    if(path != NULL){
        int width, height, nrChannels;
        uint8_t* data = stbi_load(path, &width, &height, &nrChannels, 4);
        if(data == NULL){
            DEBUG_INFO("can't load texture\n");
            return false;
        }
        if(width > 16) width = 16;
        if(height > 16) height = 16;
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, 16, 16, 1, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }


    return true;
}

void rndr_end_texture_array_loading(){
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}


void rndr_unload_texture(handle_t textureID){
    if(textureID != 0) glDeleteTextures(1, &textureID);
}

void rndr_end_3d(){
    glUseProgram(0);
}

void rndr_destroy(){
    if(g_pipeline3D.shaderProgram != 0) glDeleteProgram(g_pipeline3D.shaderProgram);
    if(g_pipeline3D.textureArray != 0) glDeleteTextures(1, &g_pipeline3D.textureArray);
}
