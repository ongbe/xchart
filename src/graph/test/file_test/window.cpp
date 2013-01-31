
#include "window.h"

#include <QPainter>
#include <QImage>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
};

Window::Window(QWidget *p):
    QWidget(p)
{
    init();
}

Window::~Window()
{
    delete m_data;
}

void Window::init()
{
    m_data = new PrivateData;

    resize(800, 600);
}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QString filePath = QString("../graph_data_transform_test/demo%1.png")
        .arg(2);
    QImage image;
    image.load(filePath);
    QRectF target(0, 0, image.width(), image.height());
    QRectF source(0, 0, image.width(), image.height());
    painter.save();
    painter.scale(0.8, 0.8);
    painter.drawImage(target, image, source);
    painter.restore();


    filePath = QString("../graph_data_transform_test/demo%1.png")
        .arg(3);
    image.load(filePath);
    target = QRectF(image.width(), 0, image.width(), image.height());
    painter.drawImage(target, image, source);

    filePath = QString("../graph_data_transform_test/demo%1.png")
        .arg(4);
    image.load(filePath);
    target = QRectF(0, image.height(), image.width(), image.height());
    painter.drawImage(target, image, source);

    filePath = QString("../graph_data_transform_test/demo%1.png")
        .arg(5);
    image.load(filePath);
    target = QRectF(image.width(), image.height(), image.width(), image.height());
    painter.drawImage(target, image, source);


    return QWidget::paintEvent(event);

}
