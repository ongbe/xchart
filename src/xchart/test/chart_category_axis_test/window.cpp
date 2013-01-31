
#include "window.h"
#include "chart_category_axis.h"

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
    Cross::RectangleEdge edge;
    QRectF rect(100, 100, 200, 200);

    edge = Cross::BOTTOM;
    Cross::ChartCategoryAxis axis;
    QList<Cross::CategoryTick> list;
    
    Cross::CategoryTick tick1;
    tick1.value = 100;
    tick1.label = "X";

    Cross::CategoryTick tick2;
    tick2.value = 120;
    tick2.label = "Y";

    Cross::CategoryTick tick3;
    tick3.value = 150;
    tick3.label = "Z";

    Cross::CategoryTick tick4;
    tick4.value = 200;
    tick4.label = "A";


    list << tick1 << tick2 << tick3 << tick4;
    axis.setCategoryTicks(list);

    axis.draw(m_data->renderer, 100, rect, QRectF(100, 0, 100, 100), edge);

    edge = Cross::LEFT;
 
    tick1.value = 0;
    tick1.label = "X";

    tick2.value = 20;
    tick2.label = "Y";

    tick3.value = 50;
    tick3.label = "Z";

    tick4.value = 100;
    tick4.label = "A";

    list.clear();
    list << tick1 << tick2 << tick3 << tick4;
    axis.setCategoryTicks(list);


    axis.draw(m_data->renderer, 400, rect, QRectF(100, 0, 100, 100), edge);


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
