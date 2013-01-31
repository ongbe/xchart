
#ifndef _TILE_CACHE_H_
#define _TILE_CACHE_H_


#include "tile_types.h"

namespace GraphLib
{
    /**
     * @brief tile cache是起管理作用的一个类，本身并不占用tile的内存。
     * 是对tile的一个外部管理的方式。
     * 由tile cache 来调用 tile seap，判断在什么情况下，一个tile
     * 应该被交换到硬盘上。
     *
     *  TileCache 会更新
     *   1 tile->cached
     *   2 tile->next
     *   3 tile->prev
     *   会依赖tile->dirty和tile->swap_offset字段。
     *
     *   将TileCache 实现成单例模式。
     *  
     * */
    class TileCache
    {
        public:
            virtual ~TileCache(){}
            static TileCache *instance();

            void init(long cache_size);
            void exit();

            // 改变cache的内存大小
            void set_size(long cache_size);
            void suspend_idle_swapper(void);

            // cache一个tile
            void insert(Tile *);
            // 移除一个tile
            void flush(Tile *);

            void dump();

        protected:
            TileCache(){}
    };

}
#endif
