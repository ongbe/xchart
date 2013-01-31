
#include "window.h"
#include "graph_renderer.h"
#include "graph_geometry.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphQtRenderer *renderer;
        GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;

        QScrollBar *bar;
};

Window::Window(QWidget *p):
    QWidget(p)
{
    init();
}

Window::~Window()
{
    delete m_data->interactiveRenderer;
    delete m_data->renderer;
    delete m_data;
}

void Window::init()
{
    m_data = new PrivateData;

    m_data->bar = new QScrollBar(Qt::Vertical, this);
    connect(m_data->bar,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(update()));
    m_data->bar->setEnabled(true);
    m_data->bar->setRange(0, 32678 - 600);
    m_data->bar->setValue(0);
    m_data->bar->move(width() - 30, 0);
    m_data->bar->resize(30, height());




    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer((GraphLib::GraphRenderer *)m_data->renderer);
    //
    //m_data->interactiveRenderer->setSize(NULL, 500, 500);
    //

    // 
    QImage *image = new QImage(1000, 32678, QImage::Format_ARGB32);
    image->fill(Qt::darkYellow);
    m_data->interactiveRenderer->setSize((void *)image, 
            image->width(), image->height());

    m_data->renderer->beginRender();

    m_data->interactiveRenderer->fillPixelRect(200, 50, 30, 30, new QColor(Qt::red));

    GraphLib::Rectangle rect1(0, 0, 300, 300);
    m_data->interactiveRenderer->clipRegionAddRect(&rect1);


    resize(800, 600);

    m_data->renderer->beginRender();

    m_data->interactiveRenderer->fillPixelRect(200, 50, 30, 30, new QColor(Qt::red));

    m_data->renderer->setLineWidth(0.3);
    m_data->renderer->drawLine(new GraphLib::Point(10, 10), 
            new GraphLib::Point(30, 50), 
            new QColor(1, 1, 0));

    m_data->renderer->drawRect(new GraphLib::Point(50, 50), 
            new GraphLib::Point(70, 70), new QColor(1, 1, 0));

    m_data->renderer->fillRect(new GraphLib::Point(80, 50), 
            new GraphLib::Point(100, 70), new QColor(Qt::cyan));

    m_data->renderer->drawEllipse(new GraphLib::Point(50, 100), 
            20, 20, new QColor(0, 1, 0));

    m_data->renderer->fillEllipse(new GraphLib::Point(100, 100), 
            20, 20, new QColor(1, 1, 0));

    GraphLib::Point polyline[] = {QPoint(110, 210), QPoint(120,210), QPoint(130,220)};
    m_data->renderer->drawPolyline(polyline, 
            3, new QColor(1, 1, 0));

    GraphLib::Point polygon[] = {QPoint(110, 310), QPoint(120,310), QPoint(130,320)};
    m_data->renderer->drawPolygon(polygon, 
            3, new QColor(1, 1, 0));

    GraphLib::Point polygon1[] = {QPoint(110, 410), QPoint(120,410), QPoint(130,420)};
    m_data->renderer->fillPolygon(polygon1, 
            3, new QColor(1, 1, 0));

    for (int i = 100; i < 99900; i+= 20) {
        QString str = QString("line %1").arg(i);
        m_data->renderer->drawString(str, 
                new GraphLib::Point(20, i), 0, new QColor(1, 1, 0));
    }


    m_data->renderer->endRender();

}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    m_data->interactiveRenderer->copyToWindow(this, 0, 
            m_data->bar->value(), width(), height());
}
