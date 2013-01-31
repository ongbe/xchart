
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Window w;// = &w1;
    w.move(300, 300);
    w.show();

    return app.exec();
}
