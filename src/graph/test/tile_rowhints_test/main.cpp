
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTime>
#include <QImage>



#include "tile_manager.h"
#include "tile.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    GraphLib::Tile *t = new GraphLib::Tile;
    t->alloc();
    qDebug() << "tile size :" << t->size;

    // 将tile转换成qimage，可以显示在窗口中
    QImage image(t->data, t->ewidth, t->eheight,
            QImage::Format_ARGB32_Premultiplied);
    image.save("demo.png");


    // ture 和 false 与整数值直接的转换。
    qDebug() << "true " << int(true);
    qDebug() << "false " << int(false);
    qDebug() << "size of unsigned char: " << sizeof(unsigned char);
    qDebug() << "size of unsigned short: " << sizeof(unsigned short);
    qDebug() << "size of int: " << sizeof(int);
    qDebug() << "size of long: " << sizeof(long);


    return 0;
    //return app.exec();
}
