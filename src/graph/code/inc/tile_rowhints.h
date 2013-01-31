


#ifndef _TILE_ROWHINTS_H_
#define _TILE_ROWHINTS_H_


#include "tile_types.h"

namespace GraphLib 
{
    /*
     *  rowhint 指的就是行指针，可以访问到每行的数据
     *  在 相邻 tile 合并的时候，可以快速的访问。
     * */
    typedef unsigned char TileRowHint;

#define TILEROWHINT_UNKNOWN 0
#define TILEROWHINT_OPAQUE 1
#define TILEROWHINT_TRANSPARENT 2
#define TILEROWHINT_MIXED 3
#define TILEROWHINT_OUTOFRANGE 4
#define TILEROWHINT_UNDEFINED 5
#define TILEROWHINT_BROKEN 6

    TileRowHint tile_get_rowhint (Tile *tile, int yoff);

    void tile_set_rowhint(Tile *tile, int yoff, 
            TileRowHint rowhint);

    void tile_allocate_rowhints(Tile *tile);
    void tile_update_rowhints(Tile *tile,
            int start,
            int rows);
}

#endif
