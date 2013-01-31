
#include <QApplication>
#include <QTextCodec>

#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    GraphLib::GraphRenderer *renderer;
    GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;


    renderer = new GraphLib::GraphQtRenderer();
    interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer(renderer);
    interactiveRenderer->setSize(NULL, 500, 500);

    renderer->beginRender();

    GraphLib::Box box;
    box.setCorner(GraphLib::Point(100.0, 200.0));
    box.setWidth(100.0);
    box.setHeight(100.0);
    box.setBorderWidth(10.0);
    box.setBorderColor(QColor(Qt::red));
    box.setFillColor(QColor(Qt::red));
    box.draw(renderer);
    box.updateHandles();

    GraphLib::Box box1;
    GraphLib::Point corner(200.0, 200.0);
    box1.setCorner(corner);
    box1.setBorderWidth(5.0);
    box1.draw(renderer);

    renderer->endRender();


    QImage image(QSize(500, 500), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    interactiveRenderer->copyToWindow(&image, 0, 0, 500, 500);
    image.save("demo.png");
    //image.save("demo.ppm");

    //return app.exec();
    return 0;
}
