

#include "tile.h"
#include "tile_manager.h"
#include "tile_cache.h"
#include "tile_swap.h"

#include <memory.h>
#include <QDebug>

namespace GraphLib
{
    static void tile_destroy(Tile *tile);

    _Tile::_Tile(int bpp)
    {
        this->ewidth = TILE_WIDTH;
        this->eheight = TILE_HEIGHT;
        this->bpp = bpp;

        this->ref_count = 0;
        this->share_count = 0;

        this->size = this->ewidth * this->eheight * this->bpp;
        this->data = 0;
        this->rowhint = 0;

        this->dirty = true;

        this->tlink = 0;
        this->next = 0;
        this->prev = 0;

        this->swap_offset = -1;
    }

    _Tile::~_Tile()
    {
        if (this->data) {
            delete []this->data;
            this->data = 0;
        }

        if (this->tlink) {

            TileLink *it = this->tlink;
            while (it) {
                TileLink *tmp = it;
                it = it->next;

                delete tmp;
            }
            this->tlink = 0;
        }
    }


    void _Tile::alloc()
    {
        if (this->data)
            return;
        this->data = new unsigned char[this->size];
        memset(this->data, 0, this->size);
    }

    unsigned char _Tile::is_valid()
    {
        return this->valid;
    }

    /**
     *  一个tile可以有多个tile manager
     *  分别代表不同的level
     *
     * */
    void _Tile::attach(Tile *tile, TileManager *tm, int tile_num)
    {
        TileLink *tl;

        if ((tile->share_count > 0) && (!tile->is_valid())) {
            /* trying to share invalid tiles is problematic,
             * not to mention silly. */
            tm->validate_tile(tile);
        }

        tile->share_count++;

        tl = new TileLink;
        tl->tm = tm;
        tl->tile_num = tile_num;
        tl->next = tile->tlink;

        tile->tlink = tl;
    }

    void _Tile::detach(Tile *tile, TileManager *tm, int tile_num)
    {
        TileLink **link;
        TileLink *tmp;

        for (link = &tile->tlink; *link != 0;
                link = &(*link)->next) {
            if (((*link)->tm == tm &&
                        ((*link)->tile_num == tile_num)))
                break;
        }

        tmp = *link;
        *link = tmp->next;

        delete tmp;

        tile->share_count--;
        if (tile->share_count == 0 && tile->ref_count == 0) {
            tile_destroy(tile);
        }
    }


    void _Tile::lock(Tile *tile)
    {
        tile->ref_count++;

        if (tile->ref_count == 1) {
            /* remove from cache, move to main store. */
            TileCache::instance()->flush(tile);
        }
        if (tile->data == NULL) {
            /* there is no data, so the tile must be swapped out */
            TileSwap::instance()->tile_swap_in(tile);
        }

        if (!tile->is_valid()) {
            if (tile->tlink && tile->tlink->tm)
                tile->tlink->tm->validate_tile(tile);
        }
    }

    void _Tile::release(Tile *tile, bool dirty)
    {
        if (tile == NULL)
            return;
        //qDebug() << "++ tile::release";
        //qDebug() << "ref_count" << tile->ref_count;
        tile->ref_count--;
        //qDebug() << "ref_count" << tile->ref_count;

        if (dirty) {
            tile->write_count--;
        }

        if (tile->ref_count == 0) {
            if (tile->share_count == 0) {
                //qDebug() << "tile_destroy";
                tile_destroy(tile);
                return;
            } else {
                //qDebug() << "TileCache insert";
                TileCache::instance()->insert(tile);
            }
        }

        //qDebug() << "-- tile::release";
    }

    unsigned char *_Tile::data_pointer(int x, int y)
    {
        return this->data + (((y & TILE_HEIGHT - 1)) * this->ewidth
                + (x & (TILE_WIDTH - 1))) * this->bpp;
    }

    void tile_destroy(Tile *tile)
    {
        if (tile->data) {
            delete[] tile->data;
            tile->data = NULL;
        }

        TileCache::instance()->flush(tile);

        if (tile->swap_offset != -1) {
            TileSwap::instance()->tile_swap_delete(tile);
        }

        delete tile;
    }
    
}
