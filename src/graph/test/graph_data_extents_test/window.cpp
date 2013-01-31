
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class Window::PrivateData
{
    public:
        GraphLib::GraphRenderer *renderer;
        GraphLib::GraphQtInteractiveRenderer *interactiveRenderer;
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
    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 500, 500);


    resize(800, 600);
    qDebug() << "init";
}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawText(rect(), Qt::AlignCenter, "Qt");

    m_data->renderer->beginRender();

    m_data->renderer->drawLine(new GraphLib::Point(10, 10), 
            new GraphLib::Point(30, 50), 
            new QColor(1, 1, 0));

    m_data->renderer->drawRect(new GraphLib::Point(50, 50), 
            new GraphLib::Point(70, 70), new QColor(1, 1, 0));

    m_data->renderer->fillRect(new GraphLib::Point(80, 50), 
            new GraphLib::Point(100, 70), new QColor(1, 1, 0));

    m_data->renderer->drawEllipse(new GraphLib::Point(50, 100), 
            20, 20, new QColor(1, 1, 0));

    m_data->renderer->fillEllipse(new GraphLib::Point(100, 100), 
            20, 20, new QColor(1, 1, 0));

    GraphLib::Point polyline[] = {GraphLib::Point(110, 210), GraphLib::Point(120,210), GraphLib::Point(130,220)};
    m_data->renderer->drawPolyline(polyline, 
            3, new QColor(1, 1, 0));

    GraphLib::Point polygon[] = {GraphLib::Point(110, 310), GraphLib::Point(120,310), GraphLib::Point(130,320)};
    m_data->renderer->drawPolygon(polygon, 
            3, new QColor(1, 1, 0));

    GraphLib::Point polygon1[] = {GraphLib::Point(110, 410), GraphLib::Point(120,410), GraphLib::Point(130,420)};
    m_data->renderer->fillPolygon(polygon1, 
            3, new QColor(1, 1, 0));

    m_data->renderer->drawString("this is a demo", 
            new GraphLib::Point(20, 20), 0, new QColor(1, 1, 0));


    m_data->renderer->endRender();


    m_data->interactiveRenderer->copyToWindow(this, 0, 0, width(), height());

}
