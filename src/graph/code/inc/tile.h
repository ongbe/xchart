
#ifndef _TILE_1_H_
#define _TILE_1_H_


#include "tile_types.h"
#include "tile_rowhints.h"

namespace GraphLib
{
#define TILE_WIDTH 256
#define TILE_HEIGHT 256

    /**
     * 一个tile可以被多次复用。
     * 可以关联到不同的tile-manager中上，实现内存的共享
     * 在每个tile-manager 中，需要关注的是两个信息:
     *     tile_num 和 tile-manager 指针。
     * 单链表的方式，存储tile与tile-maanger直接的关联。
     * 从tile的角度，可以方便的找到自己在tile-manager中的定位。
     * */
    typedef struct _TileLink TileLink;

    struct _TileLink
    {
        TileLink *next;         
        int tile_num;/* the number of this tile within the drawable ,
                        所谓的drawable, 就是可以被绘制的对象*/

        TileManager *tm; /* A pointer to the tile manager for this tile.
                          *  We need this in order to call the tile managers
                          *  validate proc whenever the tile is referenced
                          *  yet invalid.
                          */
    };

    /**
     *  tile代表内存的一种组织方式.
     *  本身的指针代表一个索引，所表示的内容分成了两部分，一部分存放
     *  内存中，另一部分放在文件中。
     * */
    struct _Tile
    {
        public:
            short ref_count; /* reference count. when the reference count is
                              *  non-zero then the "data" for this tile must
                              *  be valid. when the reference count for a tile
                              *  is 0 then the "data" for this tile must be
                              *  NULL.
                              */
            short write_count; /* write_count: number of references that are
                                  for write access. */

            short share_count; /* share count: number of tile managers that 
                                  hold this tile */

            unsigned char valid : 1;
            unsigned char dirty : 1;
            unsigned char cached : 1;


            unsigned short ewidth; /* the effective width of the tile */
            unsigned short eheight;/* the effective height of the tile */
            unsigned char bpp; /* the byte per pixel (1, 2, 3, or 4) */


            int size; /* size of the tile data (ewidth * eheight * bpp) */

            TileRowHint *rowhint; /* An array of hints for rendering 
                                     purposes */

            unsigned char *data; /*the data for the tile. this may be NULL
                                   in which case the tile data is on disk. */

            long swap_offset; /* the offset within the swap file of the tile
                                 data. if the tile data is in memeory this will
                                 be set to -1. */

            TileLink *tlink; /* 指向tile manager 的链表 */

            Tile *next; /* list pointers for the tile cache lists **/
            Tile *prev;

        public:

            _Tile(int bpp = 4);
            ~_Tile();

            void alloc();
            unsigned char is_valid();

            static void attach(Tile *tile, TileManager *tm, int tile_num);
            static void detach(Tile *tile, TileManager *tm, int tile_num);


            /**
             * tile_lock locks a tile into memory. This does what tile_ref 
             * used to do. It is the responsibility of the tile manager to 
             * take care of the copy-on-write semantics. Locks stack; a tile
             * remains locked in memory as long as it's been locked more
             * times than it has been released. tile_releas needs to know
             * whether the release was for write access. (This is a hack,
             * and should be handled better.)
             * */
            static void lock(Tile *t);
            static void release(Tile *t, bool dirty);

            unsigned char *data_pointer(int xoff, int yoff);
    };
}

#endif
