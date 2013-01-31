
#ifndef _TILE_SWAP_H_
#define _TILE_SWAP_H_


#include "tile_types.h"

namespace GraphLib
{

    /**
     * 将tile swap到硬盘上，实现tile的复用。
     *
     * */
    class TileSwap
    {
        public:
            virtual ~TileSwap() {}
            static TileSwap *instance();

            void init(const char *path);
            void exit();

            void tile_swap_in(Tile *);
            void tile_swap_out(Tile *);
            void tile_swap_delete(Tile *);

            static void dump();

        protected:
            TileSwap() {}
    };

}
#endif
