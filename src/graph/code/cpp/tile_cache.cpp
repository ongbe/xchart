
#include "tile_cache.h"
#include "tile.h"
#include "tile_swap.h"

#include <stdlib.h>
#include <QDebug>

namespace GraphLib
{
    // 记录tile的头节点
    typedef struct _TileList
    {
        Tile *first;
        Tile *last;
    } TileList;

#define PENDING_WRITE(t) ((t)->dirty || (t)->swap_offset == -1)

/**
 * 为多线程程序预留的接口
 * */
#define TILE_CACHE_LOCK /* nothing */
#define TILE_CACHE_UNLOCK /* nothing */

    static long cur_cache_size = 0;
    static long max_cache_size = 0;
    static long cur_cache_dirty = 0;

    static TileList tile_list = {NULL, NULL};
    static TileCache *g_instance = NULL;

    static bool tile_cache_zorch_next();
    static void tile_cache_flush_internal(Tile *);

    void TileCache::init(long tile_cache_size)
    {
#ifdef TILE_PROFILING
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
#endif

        tile_list.first = tile_list.last = NULL;
        max_cache_size = tile_cache_size;

    }

    void TileCache::exit()
    {
#ifdef TILE_PROFILING
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
#endif
        if (cur_cache_size > 0)
            set_size(0);
    }

    void TileCache::set_size(long cache_size)
    {
#ifdef TILE_PROFILING
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
#endif

        TILE_CACHE_LOCK;

        max_cache_size = cache_size;
        while (cur_cache_size > max_cache_size) {
            if (! tile_cache_zorch_next())
                break;
        }

        TILE_CACHE_UNLOCK;
    }

    void TileCache::suspend_idle_swapper(void)
    {
    }


    void TileCache::insert(Tile *tile)
    {
#ifdef TILE_PROFILING
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
#endif

        TILE_CACHE_LOCK;

        if (!tile->data)
            goto out;

        /**
         * First check and see if the tile is already
         * in the cache. In that case we will simply place
         * it at the end of the tile list to indicate that
         * it was the most recently acessed tile.
         * */
        if (tile->cached) {
            if (tile->next) {
                tile->next->prev = tile->prev;
            } else {
                tile_list.last = tile->prev;
            }
            if (tile->prev) {
                tile->prev->next = tile->next;
            } else {
                tile_list.first = tile->next;
            }

            if (PENDING_WRITE(tile)) {
                cur_cache_dirty -= tile->size;
            }
        } else {
            /**
             * The tile was not in the cache. First check and see
             * if there is room in the cache. If not then we'll have 
             * make room first. Note: it might be the case that the
             * cache is small than the size of the tile in which was
             * it won't be possible to put in the cache.
             * */

            while ((cur_cache_size + tile->size) > max_cache_size) {
                if (!tile_cache_zorch_next()) {
#ifdef TILE_PROFILING
                    qDebug() << cur_cache_size;
                    qDebug() << tile->size;
                    qDebug() << max_cache_size;
                    qDebug() << "cache: unable to find room ,size:";
#endif
                    goto out;
                }
            }
            cur_cache_size += tile->size;
        }

        tile->next = NULL;
        tile->prev = tile_list.last;
        if (tile_list.last) {
            tile_list.last->next = tile;
        } else {
            tile_list.first = tile;
        }

        tile_list.last = tile;
        tile->cached = true;

        if (PENDING_WRITE(tile)) {
            cur_cache_dirty += tile->size;
        }

out:

#ifdef TILE_PROFILING
        /*
        // 输出每个tile的data的第一个字符，调试信息
        Tile *tmp = tile_list.first;
        while (tmp) {
            qDebug() << "tile data --- " << tmp->data[0];
            tmp = tmp->next;
        }
        */
#endif

        return;

        TILE_CACHE_UNLOCK;

    }

    void TileCache::flush(Tile *tile)
    {
#ifdef TILE_PROFILING
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
#endif


        TILE_CACHE_LOCK;

        if (tile->cached)
            tile_cache_flush_internal(tile);

        TILE_CACHE_UNLOCK;
    }

    TileCache *TileCache::instance()
    {
        static TileCache instance;
        if (g_instance == NULL) {
            g_instance = &instance;
            g_instance->init(1024 * 1024 * 2);
        }


        return g_instance;
    }

    void TileCache::dump()
    {
        qDebug() << "cur_cache_size: " << cur_cache_size;
        qDebug() << "max_cache_size: " << max_cache_size;
        qDebug() << "cur_cache_dirty: " << cur_cache_dirty;

        if (tile_list.first) {
            qDebug() << "cache tile size: " << tile_list.first->size;
        }
        if (tile_list.last) {
            qDebug() << "cache tile size: " << tile_list.last->size;
        }
    }

    bool tile_cache_zorch_next()
    {
        Tile *tile = tile_list.first;

        if (NULL == tile) {
            qDebug() << "tile_cache_zorch_next tile == NULL";
            return false;
        }

        tile_cache_flush_internal(tile);

        if (PENDING_WRITE(tile)) {
            TileSwap::instance()->tile_swap_out(tile);
        }

        if (!tile->dirty) {
            delete []tile->data;
            tile->data = NULL;

            return true;
        }
        /* unable to swap out tile for some reason */
        qDebug() << "unable to swap out tile for some reason";
        return false;
    }

    void tile_cache_flush_internal(Tile *tile)
    {
        tile->cached = false;

        if (PENDING_WRITE(tile))
            cur_cache_dirty -= tile->size;

        cur_cache_size -= tile->size;

        if (tile->next) {
            tile->next->prev = tile->prev;
        } else {
            tile_list.last = tile->prev;
        }

        if (tile->prev) {
            tile->prev->next = tile->next;
        } else {
            tile_list.first = tile->next;
        }

        tile->next = tile->prev = NULL;
    }

}
