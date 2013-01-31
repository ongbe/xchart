
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTime>
#include <QImage>

#include "graph_data.h"
#include "graph_box.h"
#include "graph_layer.h"
#include "graph_renderer.h"
#include "graph_path_interactive_renderer.h"
#include "graph_path_renderer.h"



#include "tile_manager.h"
#include "tile.h"
#include "tile_swap.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    GraphLib::GraphData dd;
    //qDebug() << "++ extents: " << dd.extents();

    GraphLib::Box *box1 = new GraphLib::Box;
    GraphLib::Box &box = *box1;
    box.setCorner(GraphLib::Point(10.0, 20.0));
    box.setWidth(10.0);
    box.setHeight(10.0);
    box.setBorderWidth(3.0);
    box.setBorderColor(QColor(Qt::red));
    box.setFillColor(QColor(Qt::red));

    dd.activeLayer()->addObject(&box);
    if (dd.updateExtents()) {
        //qDebug() << "++ layer extents: " << dd.activeLayer()->extents();
        //qDebug() << "++ data extents: " << dd.extents();
    } 
    GraphLib::Rectangle extent = dd.extents();

    GraphLib::GraphPathRenderer *renderer;
    //GraphLib::GraphPathInteractiveRenderer *irenderer;
    renderer = new GraphLib::GraphPathRenderer;

    // 在render的过程中，形成path
    dd.render(renderer);

 
    //qDebug() << "hello.";

    GraphLib::Tile *t = new GraphLib::Tile;
    t->alloc();
    t->is_valid();
    //qDebug() << "tile size :" << t->size;

    // 将tile转换成qimage，可以显示在窗口中
    QImage image(t->data, t->ewidth, t->eheight,
            QImage::Format_ARGB32_Premultiplied);


    QTransform t1;
    QRectF r1;

    //graph 移动到自己的中心点
    //t1.translate(-1*extent.center().x(), -1*extent.center().y());
    //移动到图像的中心点
    //t1.translate(image.width()/2, image.height()/2);
    //设置存储介质和坐标转换器
    renderer->render(&image, r1, t1);
    image.save("demo.jpg");


    GraphLib::TileSwap &swap = *GraphLib::TileSwap::instance();
    swap.init("./");

    swap.tile_swap_out(t);

    GraphLib::Tile *t2 = new GraphLib::Tile;
    t2->swap_offset = 0;
    swap.tile_swap_in(t2);

    //qDebug() << "t2->ewidth " << t2->ewidth;
    QImage image1(t2->data, t2->ewidth, t2->eheight,
            QImage::Format_ARGB32_Premultiplied);
    image1.save("00.png");

    qDebug() << "save 00.png";

    swap.exit();

    return 0;
    //return app.exec();
}
