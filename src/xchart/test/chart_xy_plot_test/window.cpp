
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"

#include "chart.h"
#include "chart_plot.h"
#include "chart_number_axis.h"
#include "chart_xy_plot.h"
#include "chart_xy_dataset.h"

#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include <QPointF>
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
    m_data->interactiveRenderer->setSize(NULL, 600, 600);

    m_data->renderer->beginRender();


    Cross::ChartXYDataset *dataset = new Cross::ChartXYDataset();

    QVector<QPointF> v;
    v.push_back(QPointF(100, 100));
    v.push_back(QPointF(105, 105));
    v.push_back(QPointF(110, 110));

    dataset->insertValues("test1", v);
    Cross::ChartXYPlot *plot = new Cross::ChartXYPlot(dataset);
    plot->setYAxisRange(100, 300);

    Cross::Chart chart(plot);

    chart.draw(m_data->renderer, QRectF(40, 40, 400, 300));

    /*
    Cross::ChartNumberAxis axisBottom;
    Cross::RectangleEdge edge = Cross::BOTTOM;
    axisBottom.draw(m_data->renderer,
            300, 
            QRectF(40, 40, 400, 300),
            QRectF(80, 300, 300, 300),
            edge);

    Cross::ChartNumberAxis axisLeft;
    edge = Cross::LEFT;
    axisBottom.draw(m_data->renderer,
            80, 
            QRectF(40, 40, 400, 300),
            QRectF(12000, 80, 50, 220),
            edge);
            */




    m_data->renderer->endRender();

    delete plot;
    delete dataset;
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
