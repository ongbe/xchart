
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

    Window w;
    w.move(300, 300);
    w.show();


    return app.exec();
}
