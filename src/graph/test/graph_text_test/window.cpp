
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_color.h"
#include "graph_text_line.h"
#include "graph_text.h"

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
    delete m_data->interactiveRenderer;
    delete m_data->renderer;
    delete m_data;
}

void Window::init()
{
    m_data = new PrivateData;
    m_data->renderer = new GraphLib::GraphQtRenderer();
    m_data->interactiveRenderer = new GraphLib::GraphQtInteractiveRenderer(m_data->renderer);
    m_data->interactiveRenderer->setSize(NULL, 500, 500);

    m_data->renderer->beginRender();

    QString text = "test";
    GraphLib::Point pos(100, 100);
    GraphLib::Color color(Qt::red);
    m_data->renderer->drawString(text, &pos, 0, &color);

    pos.setY(200);
    GraphLib::GraphTextLine *line = new GraphLib::GraphTextLine;
    line->setString(new QString("this is a demo."));
    m_data->renderer->drawTextLine(line,
            &pos, 0, &color);


    pos.setY(400);
    text = "this is \n a demo";
    GraphLib::GraphText *graphText = new GraphLib::GraphText(
            &text, NULL, 15.0, &pos, &color, 0);
    //graphText->draw(m_data->renderer);

    m_data->renderer->drawText(graphText);

    m_data->renderer->endRender();

}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(width(), height());
    pixmap.fill();
    m_data->interactiveRenderer->copyToWindow(&pixmap, 0, 0, width(), height());

    painter.drawPixmap(0, 0, pixmap);

}
