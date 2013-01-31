
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"

#include "chart.h"
#include "chart_plot.h"
#include "chart_pie_plot.h"
#include "chart_pie_dataset.h"

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
    m_data->interactiveRenderer->setSize(NULL, 900, 600);

    m_data->renderer->beginRender();


    Cross::ChartPieDataset *dataset = new Cross::ChartPieDataset();
    dataset->insertValue("One", 43.2);
    dataset->insertValue("Two", 10.0);
    dataset->insertValue("Three", 27.5);
    dataset->insertValue("Four", 17.5);
    dataset->insertValue("Five", 11.0);
    dataset->insertValue("Six", 19.4);

    Cross::ChartPiePlot *plot = new Cross::ChartPiePlot(dataset);

    Cross::Chart chart(plot);

    //    chart.setPos(QPointF(10, 10));
    //    chart.setWidth(400);
    //    chart.setHeight(400);
    //    chart.draw(m_data->renderer);
    chart.draw(m_data->renderer, QRectF(10, 10, 400, 300));


    plot->setLabelStyle(Cross::ChartPiePlot::SIMPLE);
    plot->setShowStyle(Cross::ChartPiePlot::KEY);
    chart.draw(m_data->renderer, QRectF(420, 10, 400, 300));

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
