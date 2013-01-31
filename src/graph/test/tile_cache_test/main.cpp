
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTime>
#include <QImage>


#include "tile_manager.h"
#include "tile.h"
#include "tile_swap.h"
#include "tile_cache.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    GraphLib::Tile *t = new GraphLib::Tile;
    t->alloc();
    t->is_valid();
    t->data[0] = 0;

    GraphLib::Tile *t1 = new GraphLib::Tile;
    t1->alloc();
    t1->is_valid();
    t1->data[0] = 1;

    GraphLib::Tile *t2 = new GraphLib::Tile;
    t2->alloc();
    t2->is_valid();
    t2->data[0] = 2;




    // tile cache的测试思路
    GraphLib::TileCache &cache = *GraphLib::TileCache::instance();
    // 初始化一个cache的内存大小
    cache.init(1024 * 1024);

    // 插入需要cache的tile
    cache.insert(t);
    cache.insert(t1);

    // 不缓存该tile
    cache.flush(t);

    cache.insert(t2);
    // 退出
    cache.exit();
    return 0;
    //return app.exec();
}
