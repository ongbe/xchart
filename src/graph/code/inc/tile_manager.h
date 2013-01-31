
#ifndef _TILE_MANAGER_H_
#define _TILE_MANAGER_H_

#include "tile_types.h"

namespace GraphLib
{
    //typedef struct _TileManager TileManager;
    struct _TileManager
    {
        int ref_count;

        int width; /*  the width of the tiled area */
        int height;/*  the height of the tiled area */
        int bpp;/*  the bpp of each tile   */

        int ntile_rows; /* the number of tiles in each row */
        int ntile_cols; /* the number of tiles in each columns */

        Tile **tiles;


        int cached_num;
        Tile *cached_tile;


        public:
        _TileManager(int width, int height, int bpp);
        ~_TileManager();

        public:
        /* Get a specified tile from a tile manager. */
        Tile *get_tile(int xpixel, int ypixel, 
                bool wantread, bool wantwrite);
        Tile *get(int tile_num, bool wantread, bool wantwrite);
        Tile *get_at(int tile_col, int tile_row, bool wantread,
                bool wantwrite);
        void map_tile(int xpixel, int ypixel, Tile *srctile);
        void map(int tile_num, Tile *srctile);

        void validate_tile(Tile *tile);
        void invalidate_area(int x, int y, int w, int h);

        int tiles_per_col();
        int tiles_per_row();
        long get_memsize(bool sparse = false);
        void get_tile_coordinates(Tile *, int *x, int *y);
        void get_tile_col_row(Tile *, int *tile_col, int *tile_row);
        int get_tile_num(int xpixel, int ypixel);

        private:
        void allocate_tiles();
        int locate_tile(Tile *tile);
    };

}

#endif
