#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED



#include <openindev.h>



typedef enum resourcetype {
    RESTYPE_STONE_BLOCK,
    RESTYPE_DIAMOND_BLOCK,
    RESTYPE_DIAMOND_ITEM,
    RESTYPE_DIAMOND_SWORD,
    RESTYPE_END_OF_ENUM
} resourcetype_t;


bool res_load_texture(resourcetype_t type);
handle_t res_get_texture(resourcetype_t type);
bool res_load_all_textures();
void res_unload_all_loaded_textures();





#endif
