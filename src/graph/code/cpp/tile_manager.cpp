
#include "tile_types.h"

#include "tile_manager.h"
#include "tile_cache.h"
#include "tile_swap.h"
#include "tile.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <QDebug>


namespace GraphLib
{

    // 无效化一个tile, ERROR: 还未实现完成
    static void tile_manager_invalidate_tile(_TileManager *tm,
            int tile_num)
    {
        Tile *tile = tm->tiles[tile_num];
        if (! tile->valid)
            return;

        if (tile_num == tm->cached_num) {
            // 释放tile
        }

        if (tile->cached) {
            TileCache::instance()->flush(tile);
        }

        if (tile->share_count > 1) {
            // tile 被共享了
            /* This tiel is shared. Replase it with a new invalid tile. 
            */
        }

        tile->valid = false;
        if (tile->data) {
            delete[] tile->data;
            tile->data = NULL;
        }

        if (tile->swap_offset != -1) {
            /* If the tile is on disk. then delete its
             * presence there.
             * */
            TileSwap::instance()->tile_swap_delete(tile);
        }
    }

    static void tile_manager_invalidate_pixel(_TileManager *tm,
            int xpixel, int ypixel)
    {
        int num = tm->get_tile_num(xpixel, ypixel);
        if (num < 0)
            return;
        
        tile_manager_invalidate_tile(tm, num);
    }

    _TileManager::_TileManager(int width, int height, int bpp)
    {
        this->ref_count = 1;
        this->width = width;
        this->height = height;
        this->bpp = bpp;

        this->ntile_rows = (height + TILE_HEIGHT - 1) / TILE_HEIGHT;
        this->ntile_cols = (width + TILE_WIDTH - 1) / TILE_WIDTH;

        // tile manger自带的缓冲机制，只保留一个tile 的data.
        this->cached_num = -1;
        this->cached_tile = 0;

        this->tiles = 0;
    }
    _TileManager::~_TileManager()
    {
        this->ref_count--;

        if (this->ref_count < 1) {
            if (this->cached_tile)
                Tile::release(this->cached_tile, false);

            if (this->tiles) {
                int ntiles = this->ntile_rows * this->ntile_cols;
                for (int i = 0; i < ntiles; ++i) {
                    Tile::detach(this->tiles[i], this, i);
                }
                delete []this->tiles;
                this->tiles = 0;
            }
        }
    }

    Tile *_TileManager::get_tile(int xpixel, int ypixel, 
            bool wantread, bool wantwrite)
    {
        return get(get_tile_num(xpixel, ypixel),
                wantread, wantwrite);
    }

    Tile *_TileManager::get_at(int tile_col, int tile_row, bool wantread,
            bool wantwrite)
    {
        if (tile_col < 0 || tile_col >= this->ntile_cols
                || tile_row < 0 || tile_row >= this->ntile_rows)
            return 0;

        return get(tile_row * this->ntile_cols + tile_col,
                wantread, wantwrite);
    }

    int _TileManager::get_tile_num(int xpixel, int ypixel)
    {
        if ((xpixel < 0) || (xpixel >= this->width) ||
                (ypixel < 0) || (ypixel >= this->height))
            return -1;

        return (ypixel / TILE_HEIGHT) * this->ntile_cols
            + (xpixel / TILE_WIDTH);
    }

    Tile *_TileManager::get(int tile_num, bool wantread, bool wantwrite)
    {
        Tile *tile = 0;
        int ntiles;

        ntiles = this->ntile_rows * this->ntile_cols;

        if ((tile_num < 0) || (tile_num >= ntiles))
            return 0;

        if (! this->tiles)
            allocate_tiles();

        tile = this->tiles[tile_num];
        if (wantread) {
            if (wantwrite) {
                if (tile_num == this->cached_num) {
                    Tile::release(this->cached_tile, false);
                    // tile release
                    this->cached_tile = 0;
                    this->cached_num = -1;
                }

                //tile在不同的tile manager 共享的时候，要能够保留当前的内容
                if (tile->share_count > 1) {
                    /* copy-on-required */
                }
                /* must lock before making dirty. */
                Tile::lock(tile);
                tile->write_count++;
                tile->dirty = true;
            } else {
                Tile::lock(tile);
            }
        }

        return tile;
    }

    void _TileManager::allocate_tiles()
    {
        Tile **tiles;

        const int nrows = this->ntile_rows;
        const int ncols = this->ntile_cols;
        const int right_tile = this->width - 
            ((ncols - 1) * TILE_WIDTH);
        const int bottom_tile = this->height - 
            ((nrows -1) * TILE_HEIGHT);

        int i, j, k;
        assert(this->tiles == 0);

        tiles = new Tile*[nrows * ncols];
        for (i = 0, k = 0; i < nrows; ++i) {
            for (j = 0; j < ncols; ++j, ++k) {
                Tile *tmp = new Tile;

                Tile::attach(tmp, this, k);

                if ( j == (ncols - 1)) {
                    tmp->ewidth = right_tile;
                }
                if (i == (nrows - 1)) {
                    tmp->eheight = bottom_tile;
                }

                tmp->size = tmp->ewidth * tmp->eheight * tmp->bpp;
                // 初始化一个tile
                tiles[k] = tmp;
            }
        }

        this->tiles = tiles;
    }

    void _TileManager::validate_tile(Tile *tile)
    {
        if (tile == NULL)
            return;
        tile->valid = true;
        tile->data = new unsigned char[tile->size];
        memset(tile->data, 0, tile->size);
    }

    void _TileManager::invalidate_area(int x, int y, int w, int h)
    {
        qDebug() << "----invalidate_area----";

        int i;
        int j;

        if (!this->tiles)
            return;

        for (i = y; i < (y + h); 
                i += (TILE_HEIGHT - (i % TILE_HEIGHT))) {
            for (j = x; j < (x + w); 
                    j += (TILE_WIDTH - ( j % TILE_WIDTH))) {
                tile_manager_invalidate_pixel(this, j, i);
            }
        }
    }

    int _TileManager::tiles_per_col()
    {
        return this->ntile_cols;
    }
    int _TileManager::tiles_per_row()
    {
        return this->ntile_rows;
    }
    //
    //
    // FIXED ME: tile is_valid 未实现
    long _TileManager::get_memsize(bool sparse)
    {
        size_t memsize = sizeof (this);

        memsize += this->ntile_rows * this->ntile_cols *
            (sizeof (Tile) + sizeof (void *));

        if (sparse && this->tiles) {
            Tile **tiles = this->tiles;
            size_t size = TILE_WIDTH * TILE_HEIGHT * this->bpp;
            int i, j;
            for (i = 0; i < this->ntile_rows; ++i) {
                for (j = 0; j < this->ntile_cols; ++j) {
                    if((*tiles)->is_valid())
                        memsize += size;
                }
            }
        }

        return memsize;
    }

    void _TileManager::get_tile_coordinates(Tile *tile, int *x, int *y)
    {
        if (tile == 0 || x == 0 || y == 0)
            return;
        int tile_col;
        int tile_row;

        get_tile_col_row(tile, &tile_col, &tile_row);

        *x = TILE_WIDTH * tile_col;
        *y = TILE_HEIGHT * tile_row;
    }

    void _TileManager::get_tile_col_row(Tile *tile, int *tile_col, int *tile_row)
    {
        int tile_num;

        tile_num = locate_tile(tile);
        *tile_col = tile_num % this->ntile_cols;
        *tile_row = tile_num / this->ntile_cols;
    }

    int _TileManager::locate_tile(Tile *tile)
    {
        TileLink *tl;
        for (tl = tile->tlink; tl; tl = tl->next) {
            if (tl->tm == this)
                break;
        }
        if (tl == NULL) {
            // tile not attached to manager
            return 0;
        }
        return tl->tile_num;

    }
}


