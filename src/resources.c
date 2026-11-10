#include <resources.h>
#include <strings.h>
#include <renderer.h>
// TODO: optimize, also use arenas

struct textures {
    resourcetype_t type;
    handle_t id;
};

// TODO: make these not hardcoded
// because we want support for texture packs
static const char* texturePaths[] = {
    [RESTYPE_STONE_BLOCK] = "assets/minecraft/textures/blocks/stone.png",
    [RESTYPE_DIAMOND_BLOCK] = "assets/minecraft/textures/blocks/diamond_block.png",
    [RESTYPE_DIAMOND_ITEM] = "assets/minecraft/textures/items/diamond.png",
};



static struct textures* g_Textures = NULL;
static int size = 0;
static int capacity = 0;

bool res_load_texture(resourcetype_t type){
    if(type >= RESTYPE_END_OF_ENUM){
        return false;
    }
    if(type >= ARR_LEN(texturePaths)){
        DEBUG_INFO("texture exists, but isn't in the texturePaths array\n");
        return false;
    }
    if(size == 0){
        size = 30;
        g_Textures = calloc(size, sizeof(struct textures));
    }
    if(capacity >= size){
        size *= 2;
        g_Textures = realloc(g_Textures, size * sizeof(struct textures));
    }
    g_Textures[capacity].type = type;
    g_Textures[capacity].id = rndr_load_texture(texturePaths[type]);
    if(g_Textures[capacity].id == 0){
        DEBUG_FAIL("couldn't load texture\n");
        return false;
    }
    capacity++;
    return true;
}
// TODO: we can optimize this lol
// every texture in the texture array is ironically corresponding with their specific index
// we can just do g_Textures[type].id, however i'm not doing this here because
// how about skins? maybe we can make it a seperate array but idk
// we shall see :^)
handle_t res_get_texture(resourcetype_t type){
    for(int i = 0; i < capacity; i++){
        if(g_Textures[i].type == type){
            return g_Textures[i].id;
        }
    }
    return 0;
}


bool res_load_all_textures(){
    int texturePathArraySize = ARR_LEN(texturePaths);
    for(int i = 0; i < RESTYPE_END_OF_ENUM; i++){
        bool result = res_load_texture(i);
        if(result == false){
            if(i < texturePathArraySize){
                DEBUG_WARNING("resource id %d with path %s wasn't loaded\n", i, texturePaths[i]);
            } else {
                DEBUG_WARNING("resource id %d wasn't loaded\n", i);
            }
        }
    }
    return true;
}

void res_unload_all_loaded_textures(){
    for(int i = 0; i < capacity; i++){
        rndr_unload_texture(g_Textures[i].id);
    }
}
