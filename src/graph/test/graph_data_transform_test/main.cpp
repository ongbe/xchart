
#include <QApplication>
#include <QTextCodec>
#include <QImage>
#include <QTime>
#include <QTransform>
#include <QLabel>
#include <QDebug>

#include "graph_data.h"
#include "graph_box.h"
#include "graph_layer.h"
#include "graph_renderer.h"
#include "graph_path_interactive_renderer.h"
#include "graph_path_renderer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qDebug() << "this is a demo.";

    GraphLib::GraphData dd;
    qDebug() << "++ extents: " << dd.extents();

    GraphLib::Box *box1 = new GraphLib::Box;
    GraphLib::Box &box = *box1;
    box.setCorner(GraphLib::Point(100.0, 200.0));
    box.setWidth(100.0);
    box.setHeight(100.0);
    box.setBorderWidth(3.0);
    box.setBorderColor(QColor(Qt::red));
    box.setFillColor(QColor(Qt::red));

    dd.activeLayer()->addObject(&box);
    if (dd.updateExtents()) {
        qDebug() << "++ layer extents: " << dd.activeLayer()->extents();
        qDebug() << "++ data extents: " << dd.extents();
    } 
    GraphLib::Rectangle extent = dd.extents();

    QImage *image = new QImage(QSize(300, 240), QImage::Format_ARGB32_Premultiplied);
    image->fill(Qt::white);

    GraphLib::GraphPathRenderer *renderer;
    GraphLib::GraphPathInteractiveRenderer *irenderer;
    renderer = new GraphLib::GraphPathRenderer;
    /*
     * 不同interactive的方式，也可以绘制出图形
    irenderer = new GraphLib::GraphPathInteractiveRenderer(renderer);
    irenderer->setSize(image, 300, 240);
    */

    QTime t;
    t.start();
    // 在render的过程中，形成path
    dd.render(renderer);
    qDebug("Time elapsed: %dd ms", t.elapsed());

    QTransform t1;
    QRectF r1;

    //graph 移动到自己的中心点
    t1.translate(-1*extent.center().x(), -1*extent.center().y());
    //移动到图像的中心点
    t1.translate(image->width()/2, image->height()/2);
    //设置存储介质和坐标转换器
    renderer->render(image, r1, t1);
    image->save("demo.png");

    QImage *image2 = new QImage(QSize(60, 60), QImage::Format_ARGB32_Premultiplied);
    image2->fill(Qt::white);
    QTransform t2;
    t2.translate(-1*extent.center().x(), -1*extent.center().y());
    t2.translate(extent.width()/2 + 10, extent.height()/2 + 10);
    renderer->render(image2, r1, t2);
    image2->save("demo2.png");

    QImage *image3 = new QImage(QSize(60, 60), QImage::Format_ARGB32_Premultiplied);
    image3->fill(Qt::white);
    QTransform t3;
    t3.translate(-1*extent.center().x(), -1*extent.center().y());
    t3.translate(0, extent.height()/2 + 10);
    //t3.translate(-1*extent.width()/2 + 10, extent.height()/2 + 10);
    renderer->render(image3, r1, t3);
    image3->save("demo3.png");

    QImage *image4 = new QImage(QSize(60, 60), QImage::Format_ARGB32_Premultiplied);
    image4->fill(Qt::white);
    QTransform t4;
    t4.translate(-1*extent.center().x(), -1*extent.center().y());
    t4.translate(extent.width()/2 + 10, 0);
    //t4.translate(-1*extent.width()/2 + 10, extent.height()/2 + 10);
    renderer->render(image4, r1, t4);
    image4->save("demo4.png");


    QImage *image5 = new QImage(QSize(60, 60), QImage::Format_ARGB32_Premultiplied);
    image5->fill(Qt::white);
    QTransform t5;
    //图形的中心移动到坐标的原点。
    t5.translate(-1*extent.center().x(), -1*extent.center().y());
    //t5.translate(extent.width()/2 + 10, 0);
    //t5.translate(-1*extent.width()/2 + 10, extent.height()/2 + 10);
    renderer->render(image5, r1, t5);
    image5->save("demo5.png");

    for (int i = 1; i <= 5; ++i) {
        qDebug() << QString("demo%1.png").arg(i);
    }

    qDebug() << "data extents: " << extent;

    return 0;
}
