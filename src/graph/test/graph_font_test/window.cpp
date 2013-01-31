
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_font.h"

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
    delete m_data->renderer;
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

    GraphLib::GraphFont *font = m_data->renderer->font();
    qDebug() << font->height();
    QString tt = "this is a demo";
    qDebug() << font->stringWidth(tt);

    m_data->renderer->beginRender();

    m_data->renderer->drawRect(
            new GraphLib::Point(50, 50), 
            new GraphLib::Point(50 + font->stringWidth(tt), 50 + font->height()), 
            new QColor(1, 1, 0));

    m_data->renderer->drawString(tt,
            new GraphLib::Point(0, 0+font->height()),
            0,
            new QColor(255, 0, 0));


    m_data->renderer->drawString(tt,
            new GraphLib::Point(50, 50+font->height()),
            0,
            new QColor(0, 255, 0));

    m_data->renderer->drawString(tt,
            //new GraphLib::Point(50, 50 + font->height()/2.0), 
            new GraphLib::Point(0, 100),
            0,
            new QColor(122, 122, 122));





   m_data->renderer->endRender();


}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(width(), height());
    pixmap.fill();
    m_data->interactiveRenderer->copyToWindow(&pixmap, 0, 0, width(), height());

    painter.drawPixmap(0, 0, pixmap);

    painter.drawText(rect(), Qt::AlignCenter, "Qt");
}
