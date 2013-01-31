
#include "tile_swap.h"
#include "tile.h"

#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>


namespace GraphLib
{
    typedef enum
    {
        SWAP_IN = 1,
        SWAP_OUT,
        SWAP_DELETE
    } SwapCommand;

#define MAX_OPEN_SWAP_FILES 16

    typedef struct _SwapFile SwapFile;
    typedef struct _SwapFileGap SwapFileGap;


    // 交换文件，采用文件句柄保存
    struct _SwapFile 
    {
        std::string filename;
        FILE *fd;
        std::list<SwapFileGap *> gaps;
        size_t swap_file_end;
        size_t cur_position;
    };

    //文件内容之间的间隔,记录每次内容增长的范围
    struct _SwapFileGap 
    {
        size_t start;
        size_t end;
    };


    //单例方式
    static SwapFile *g_swap_file = NULL;
    static TileSwap *g_instance = NULL;

    // swap file的每次的增量。
    static const size_t swap_file_grow = 1024 * TILE_WIDTH * TILE_HEIGHT * 4;


    static void tile_swap_command(Tile *, int command);

    static void tile_swap_open(SwapFile *);
    static void tile_swap_resize(SwapFile *, size_t newsize);

    static void tile_swap_default_in(SwapFile *, Tile *);

    static void tile_swap_default_out(SwapFile *, Tile *);

    static void tile_swap_default_delete(SwapFile *, Tile *);

    static size_t tile_swap_find_offset(SwapFile *, int bytes); 

    static SwapFileGap *tile_swap_gap_new(size_t start, size_t end);

    static void tile_swap_gap_destroy(SwapFileGap *);


    TileSwap *TileSwap::instance()
    {
        static TileSwap instance;
        if (g_instance == NULL) {
            g_instance = &instance;
            g_instance->init(NULL);
        }

        return g_instance;
    }

    void TileSwap::init(const char *path)
    {
        g_swap_file = new  SwapFile;
        g_swap_file->filename = "./a.bin";
        g_swap_file->gaps.clear();
        g_swap_file->swap_file_end = 0;
        g_swap_file->cur_position = 0;
        g_swap_file->fd = 0;
    }

    void TileSwap::exit()
    {
        if (g_swap_file->fd) {
            fclose(g_swap_file->fd);
        }
    }

    void TileSwap::tile_swap_in(Tile *tile)
    {
        if (tile->swap_offset == -1) {
            tile->alloc();
            return;
        }
        tile_swap_command(tile, SWAP_IN);
    }

    void TileSwap::tile_swap_out(Tile *tile)
    {
        tile_swap_command(tile, SWAP_OUT);
    }

    void TileSwap::tile_swap_delete(Tile *tile)
    {
        tile_swap_command(tile, SWAP_DELETE);
    }

    void tile_swap_open(SwapFile *swap_file)
    {
        if (swap_file->fd)
            return;

        swap_file->fd = fopen(swap_file->filename.c_str(), "w+");
        if (swap_file->fd == NULL)
            return;
        rewind(swap_file->fd);
    }

    void tile_swap_command(Tile *tile, int command)
    {
        if (g_swap_file->fd == NULL) {
            tile_swap_open(g_swap_file);

            if (g_swap_file->fd == NULL) {
                return;
            }
        }

        switch(command) {
            case SWAP_IN:
                tile_swap_default_in(g_swap_file, tile);
                break;
            case SWAP_OUT:
                tile_swap_default_out(g_swap_file, tile);
                break;
            case SWAP_DELETE:
                tile_swap_default_delete(g_swap_file, tile);
                break;
            default:
                break;
        };
    }

    /** the actual swap file code. The swap file consists of tiles
     * which have been moved out to disk in order to conserve memory
     * the swap file format is free form. Any tile in memory may end
     * up anywhere on disk.
     * An actual tile in the swap file consists only of the tile data.
     * The offset of the tile on disk is tored in the tile data structure
     * in memory.
     *
     * */
    void tile_swap_default_in(SwapFile *swap_file, Tile *tile)
    {
        int nleft;
        int offset;

        if (tile->data)
            return;

        if (swap_file->cur_position != tile->swap_offset) {
            swap_file->cur_position = tile->swap_offset;

            offset = fseek(swap_file->fd, tile->swap_offset, SEEK_SET);
            if (offset != 0) {
                qDebug() << "fseek error.";
                return;
            }
        }

        tile->alloc();

        nleft = tile->size;
        while (nleft > 0) {
            int err = 0;
            do {
                // c的接口在ubuntn上有个技巧，连续的读写文件，要
                // 中间调用一次fseek.
                fseek(swap_file->fd, tile->swap_offset, SEEK_SET);
                err = fread(tile->data + tile->size - nleft,
                        sizeof(unsigned char), nleft, swap_file->fd);
            } while(err != tile->size);
            if (err < 0) {
                qDebug() << "fread error.";
                return;
            }

            nleft -= err;
        }

        swap_file->cur_position += tile->size;
    }


    void tile_swap_default_out(SwapFile *swap_file, Tile *tile)
    {
        int bytes;
        int nleft;
        size_t offset;
        size_t newpos;

        bytes = TILE_WIDTH * TILE_HEIGHT * tile->bpp;

        if (tile->swap_offset == -1) {
            newpos = tile_swap_find_offset(swap_file, bytes);
        } else {
            newpos = tile->swap_offset;
        }


        if (swap_file->cur_position != newpos) {
            offset = fseek(swap_file->fd, newpos, SEEK_SET);
            if (offset != 0) {
                qDebug() << "fseek error.";
                return;
            }
            swap_file->cur_position = newpos;
        }

        nleft = tile->size;
        while (nleft > 0) {

            int err = fwrite(tile->data + tile->size - nleft,
                    sizeof(unsigned char), nleft, swap_file->fd);
            int ret = fflush(swap_file->fd);

            if (err <= 0) {
                qDebug() << "fwrite error.";
                return;
            }
            nleft -= err;
        };

        swap_file->cur_position += tile->size;
        tile->dirty = false;
        tile->swap_offset = newpos;
    }


    // 从文件中查找合适的偏移量
    size_t tile_swap_find_offset(SwapFile *swap_file, int bytes)
    {
        SwapFileGap *gap;
        size_t offset;

        // 从gap中寻找合适的空间
        std::list<SwapFileGap *> &tmp = swap_file->gaps;
        std::list<SwapFileGap *>::iterator it;
        for (it = tmp.begin(); it != tmp.end(); ++it) {
            gap = *it;
            if ((gap->end - gap->start) >= bytes) {
                offset = gap->start;
                gap->start += bytes;
            }

            if (gap->start == gap->end) {
                delete gap;
                tmp.erase(it);
            }
            return offset;
        }

        // 追加到文件的末尾
        offset = swap_file->swap_file_end;
        // 文件每次增加一个较大的幅度
        tile_swap_resize(swap_file, swap_file->swap_file_end +
                swap_file_grow);
        if ((offset + bytes) < (swap_file->swap_file_end)) {
            // 记录新的gap的位置
            gap = tile_swap_gap_new(offset + bytes,
                    swap_file->swap_file_end);
            swap_file->gaps.push_back(gap);
        }

        return offset;
    }

    void tile_swap_resize(SwapFile *swap_file, size_t new_size)
    {
        if (swap_file->swap_file_end > new_size) {
            //qDebug() << "tile_swap_resize error";
            //截断文件
            return;
        }
        swap_file->swap_file_end = new_size;
    }

    void tile_swap_default_delete(SwapFile *swap_file, Tile *tile)
    {
        //static int n = 0;
        //qDebug() << ++n << " tile_swap_default_delete";

        if (tile->swap_offset == -1)
            return;

        SwapFileGap *gap;
        SwapFileGap *gap2;
        size_t start;
        size_t end;

        // 释放tile对应的文件空间
        start = tile->swap_offset;
        end = start + TILE_WIDTH * TILE_HEIGHT * tile->bpp;
        tile->swap_offset = -1;

        //qDebug() << "," << start << "," << end;

        // 修改gap列表,合并gap 
        std::list<SwapFileGap *> &tmp = swap_file->gaps;
        std::list<SwapFileGap *>::iterator it;
        bool ok = false;
        for (it = tmp.begin(); it != tmp.end(); ++it) {
            gap = *it;
            if (end == gap->start) {
                gap->start = start;
                if (it != tmp.begin()) { //该节点有前置节点
                    it--;
                    gap2 = *it;
                    if (gap->start == gap2->end) {
                        gap2->end = gap->end;
                        tile_swap_gap_destroy(gap);
                        swap_file->gaps.erase(++it);
                    }
                }
                ok = true;
                break;
            } else if (start == gap->end) {
                gap->end = end;
                ++it;
                if (it != swap_file->gaps.end()) {
                    gap2 = *it;
                    if (gap->end == gap2->start) {
                        gap2->start = gap->start;
                        tile_swap_gap_destroy(gap);
                        swap_file->gaps.erase(--it);
                    }
                }
                ok = true;
                break;
            } else if (end < gap->start) {
                gap = tile_swap_gap_new(start, end);
                swap_file->gaps.insert(it, gap);
                ok = true;
                break;
            }
        }
        if (!ok && it == swap_file->gaps.end()) {
            gap = tile_swap_gap_new(start ,end);
            swap_file->gaps.push_back(gap);
        }

        //判断，是否与文件结尾重合
        std::list<SwapFileGap *>::reverse_iterator lastOne= swap_file->gaps.rbegin();
        gap = *lastOne;//swap_file->gaps.back();
        if (gap->end == swap_file->swap_file_end) {
            tile_swap_resize(swap_file, gap->start);
            tile_swap_gap_destroy(gap);
            swap_file->gaps.erase(it);
        }
    }

    SwapFileGap *tile_swap_gap_new(size_t start, size_t end)
    {
        SwapFileGap *gap = new SwapFileGap;
        gap->start = start;
        gap->end = end;
        return gap;
    }

    void tile_swap_gap_destroy(SwapFileGap *gap)
    {
        delete gap;
    }

    void TileSwap::dump()
    {
        qDebug() << "+++";
        qDebug() << "TileSwap::dump";
        qDebug() << "swap_file_end : " << g_swap_file->swap_file_end;
        qDebug() << "cur_position : " << g_swap_file->cur_position;

        std::list<SwapFileGap *> &gaps = g_swap_file->gaps;
        qDebug() << "gaps size: " << gaps.size();
        std::list<SwapFileGap *>::iterator it;
        for (it = gaps.begin(); it != gaps.end(); ++it) {
            SwapFileGap *gap = *it;
            qDebug() << gap->start << "," << gap->end;
        }

        qDebug() << "---";
    }

}

