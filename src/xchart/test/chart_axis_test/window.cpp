
#include "window.h"
#include "chart_number_axis.h"

#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"


#include "chart.h"

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

    GraphLib::Box box1;
    box1.setCorner(GraphLib::Point(0, 0));
    box1.setWidth(300.0);
    box1.setHeight(300.0);
    box1.draw(m_data->renderer);

    GraphLib::Box box2;
    box2.setCorner(GraphLib::Point(0, 0));
    box2.setWidth(300.0);
    box2.setHeight(200.0);
    box2.draw(m_data->renderer);


    Cross::ChartNumberAxis axis;

    axis.draw(m_data->renderer, 200, 
            QRectF(0, 0, 300, 300), 
            QRectF(0, 0, 300, 200), Cross::BOTTOM);

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

void Window::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_Escape:
            close();
            break;
    }
    return QWidget::keyPressEvent(event);
}
