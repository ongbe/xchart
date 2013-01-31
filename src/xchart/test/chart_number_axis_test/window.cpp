
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
    m_data->interactiveRenderer->setSize(NULL, 800, 800);

    m_data->renderer->beginRender();
    Cross::RectangleEdge edge;
    QRectF rect(100, 100, 200, 200);

    edge = Cross::TOP;
    Cross::ChartNumberAxis axis;
    axis.setTickLabelsVisible(false);
    axis.draw(m_data->renderer, 100, QRectF(100, 0, 100, 100), rect, edge);


    Cross::ChartNumberAxis axis0;
    edge = Cross::BOTTOM;
    axis0.setAxisLabelVisible(true);
    axis0.draw(m_data->renderer, 300, QRectF(100, 300, 100, 100), rect, edge);



    Cross::ChartNumberAxis axis1;
    edge = Cross::RIGHT;
    axis1.draw(m_data->renderer, 300, QRectF(300, 100, 100, 100), rect, edge);

    Cross::ChartNumberAxis axis2;
    edge = Cross::LEFT;
    axis2.setMinorTickMarksVisible(false);
    axis2.draw(m_data->renderer, 100, QRectF(0, 100, 100, 100), rect, edge);

    QRectF rect1(400, 100, 200, 200);

    Cross::ChartNumberAxis axis3;
    edge = Cross::BOTTOM;
    axis3.setAxisLabelVisible(true);
    axis3.setMinorTickMarksVisible(false);
    axis3.draw(m_data->renderer, 300, QRectF(100, 300, 100, 100), rect1, edge);

    rect1 = QRectF(400, 100, 200, 50);
    Cross::ChartNumberAxis axis4;
    edge = Cross::BOTTOM;
    axis4.setAxisLabelVisible(true);
    axis4.draw(m_data->renderer, 150, QRectF(100, 150, 100, 100), rect1, edge);

    rect1 = QRectF(400, 100, 200, 100);
    Cross::ChartNumberAxis axis5;
    edge = Cross::BOTTOM;
    axis5.setAxisLabelVisible(true);
    axis5.setTickLabelsVisible(false);
    axis5.setMinorTickMarksVisible(false);
    axis5.setArrayVisible(true);
    axis5.draw(m_data->renderer, 200, QRectF(100, 200, 100, 100), rect1, edge);



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
