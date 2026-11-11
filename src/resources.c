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



static struct textures* g_textures = NULL;
static int g_size = 0;
static int g_capacity = 0;

bool res_load_texture(resourcetype_t type){
    if(type >= RESTYPE_END_OF_ENUM){
        return false;
    }
    if(type >= ARR_LEN(texturePaths)){
        DEBUG_INFO("texture exists, but isn't in the texturePaths array\n");
        return false;
    }
    if(g_size == 0){
        g_size = 30;
        g_textures = calloc(g_size, sizeof(struct textures));
    }
    if(g_capacity >= g_size){
        g_size *= 2;
        g_textures = realloc(g_textures, g_size * sizeof(struct textures));
    }
    g_textures[g_capacity].type = type;
    g_textures[g_capacity].id = rndr_load_texture(texturePaths[type]);
    if(g_textures[g_capacity].id == 0){
        DEBUG_FAIL("couldn't load texture\n");
        return false;
    }
    g_capacity++;
    return true;
}
// TODO: we can optimize this lol
// every texture in the texture array is ironically corresponding with their specific index
// we can just do g_textures[type].id, however i'm not doing this here because
// how about skins? maybe we can make it a seperate array but idk
// we shall see :^)
handle_t res_get_texture(resourcetype_t type){
    for(int i = 0; i < g_capacity; i++){
        if(g_textures[i].type == type){
            return g_textures[i].id;
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
    for(int i = 0; i < g_capacity; i++){
        rndr_unload_texture(g_textures[i].id);
    }
}
