#include <blocks.h>
#include <resources.h>





struct blocktexture {
    resourcetype_t resource;
    uint32_t color;
};



struct blocktexture textures[][6] = {
    [BLOCK_AIR] = {{RESTYPE_NONE, 0x0}, {RESTYPE_NONE, 0x0}, {RESTYPE_NONE, 0x0},
                   {RESTYPE_NONE, 0x0}, {RESTYPE_NONE, 0x0}, {RESTYPE_NONE, 0x0}},
    [BLOCK_STONE] = {{RESTYPE_STONE_BLOCK, 0x000000FF}, {RESTYPE_STONE_BLOCK, 0x000000FF},
                     {RESTYPE_STONE_BLOCK, 0x000000FF}, {RESTYPE_STONE_BLOCK, 0x000000FF},
                     {RESTYPE_STONE_BLOCK, 0x000000FF}, {RESTYPE_STONE_BLOCK, 0x000000FF}}
};
