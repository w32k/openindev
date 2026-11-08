#include <renderer.h>
#include <window.h>

static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
};

static unsigned int indices[] = {
    0, 1, 2, 0, 2, 3
};

bool init(void* data){
    (void)data;
    return true;
}

bool resize(void* data, int width, int height){
    (void)data;
    (void)width;
    (void)height;
    return true;
}

bool render(void* data){
    (void)data;
    rndr_clear(0, 0, 0, 0);
    rndr_draw_vertices_with_indices(vertices, 0, 3, indices, 6);
    return true;
}

bool cleanup(void* data){
    (void)data;
    return true;
}

int main(){
    window_t window = {
        "OpenIndev",
        init, resize, render, cleanup,
        NULL, 600, 600
    };
    return win_create(&window);
}
