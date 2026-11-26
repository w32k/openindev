#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED



#include <openindev.h>



typedef enum resourcetype {
    RESTYPE_NONE,
    RESTYPE_STONE_BLOCK,
    RESTYPE_DIRT_BLOCK,
    RESTYPE_SNOW_BLOCK,
    RESTYPE_DIAMOND_BLOCK,
    RESTYPE_DIAMOND_ITEM,
    RESTYPE_DIAMOND_SWORD,
    RESTYPE_GRASS_BLOCK_SIDE,
    RESTYPE_GRASS_BLOCK_TOP,
    RESTYPE_GRASS_BLOCK_OVERLAY,
    RESTYPE_END_OF_ENUM
} resourcetype_t;


bool res_load_texture(resourcetype_t type);
handle_t res_get_texture(resourcetype_t type);
void res_load_all_default_textures();
void res_unload_all_loaded_textures();





#endif
