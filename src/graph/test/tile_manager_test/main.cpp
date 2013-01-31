
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QImage>

//#include "window.h"
#include "tile_manager.h"
#include "tile.h"
#include "tile_cache.h"
#include "tile_swap.h"

void test_tile_manager(int w, int h);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //test_tile_manager(100, 100);
    test_tile_manager(1000, 1000);
    //test_tile_manager(10000, 10000);
    //return app.exec();
    return 0;

}

void test_tile_manager(int w, int h)
{
    //生成一个tile manager
    //初始化每个tile为自己的tile num
    //一次访问每一个tile，查看tile的内容
    //重新生成一个更大的tile manager 
    //

    GraphLib::TileManager *tm = 
        new GraphLib::TileManager(w, h, 4);

    int ntiles = tm->ntile_rows * tm->ntile_cols;
    for (int i = 0; i < ntiles; ++i) {
        GraphLib::Tile *t = tm->get(i, true, true);
        //qDebug() << i << ":" << t->size;

        char buffer[1024];
        sprintf(buffer, "%d", i);
        strcpy((char *)t->data, buffer);

        GraphLib::Tile::release(t, true);
    }


    for (int i = 0; i < ntiles; ++i) {
        GraphLib::Tile *t = tm->get(i, true, false);

        qDebug() << "tile swap_offset: " << t->swap_offset;
        qDebug() << "       tile size: " << t->size;

        char *buffer = (char *)t->data;
        int n = atoi(buffer);
        if (i != n) {
            qDebug() << "error +++++++++++" << i << "----" << n;
        }
        GraphLib::Tile::release(t, false);

    }

    tm->invalidate_area(0, 0, 100, 100);

    // 一共多少个tile
    qDebug() << "tile sum: " << ntiles;
    delete tm;

    // 查看cache的状态
    //GraphLib::TileCache::instance()->dump();

    GraphLib::TileSwap::instance()->dump();
}
