
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "window.h"
#include "graph_data.h"
#include "graph_box.h"
#include "graph_layer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    /*
    Window *w = new Window;
    w->move(300, 300);
    w->show();
    */

    qDebug() << "this is a demo.";

    GraphLib::GraphData d;
    qDebug() << "++ extents: " << d.extents();

    GraphLib::Box *box1 = new GraphLib::Box;
    GraphLib::Box &box = *box1;
    box.setCorner(GraphLib::Point(100.0, 200.0));
    box.setWidth(100.0);
    box.setHeight(100.0);
    box.setBorderWidth(10.0);
    box.setBorderColor(QColor(Qt::red));
    box.setFillColor(QColor(Qt::red));

    d.activeLayer()->addObject(&box);
    if (d.updateExtents()) {
        qDebug() << "++ layer extents: " << d.activeLayer()->extents();
        qDebug() << "++ data extents: " << d.extents();
    } 

    if (!d.updateExtents()) {
        qDebug() << "graph data no changed.";
    }

    return 0;
}
