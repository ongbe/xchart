
#include "tile_rowhints.h"
#include "tile_types.h"
#include "tile.h"

#include <stdlib.h>


namespace GraphLib 
{
    void tile_allocate_rowhints(Tile *tile)
    {
        if (!tile->rowhint) {
            tile->rowhint = new TileRowHint[sizeof(TileRowHint) *
                TILE_HEIGHT];
        }
    }

    TileRowHint tile_get_rowhint (Tile *tile, int yoff)
    {
        if (NULL == tile->rowhint)
            return TILEROWHINT_UNKNOWN;
        if (yoff < tile->eheight && yoff > 0)
            return tile->rowhint[yoff];
        return TILEROWHINT_OUTOFRANGE;
    }

    void tile_set_rowhint(Tile *tile, int yoff, 
            TileRowHint rowhint)
    {
        tile->rowhint[yoff] = rowhint;
    }

    void tile_update_rowhints(Tile *tile,
            int start,
            int rows)
    {

    }

}
