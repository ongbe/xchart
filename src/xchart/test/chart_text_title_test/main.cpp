
#include <QApplication>
#include <QTextCodec>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Window w;// = new Window;
    w.move(300, 300);
    w.show();

    return app.exec();
}
