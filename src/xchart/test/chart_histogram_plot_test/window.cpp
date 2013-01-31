
#include "window.h"
#include "graph_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_qt_interactive_renderer.h"
#include "graph_box.h"
#include "graph_handle.h"

#include "chart.h"
#include "chart_plot.h"
#include "chart_histogram_plot.h"
#include "chart_histogram_dataset.h"

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
    m_data->interactiveRenderer->setSize(NULL, 1200, 1200);

    m_data->renderer->beginRender();

    Cross::ChartHistogramDataset *dataset = new Cross::ChartHistogramDataset();
    dataset->insertValue("d1", 43.2);
    dataset->insertValue("d2", 10.0);
    dataset->insertValue("d3", 27.5);
    dataset->insertValue("d4", 17.5);
    dataset->insertValue("d5", 11.0);
    dataset->insertValue("d6", 19.4);
    dataset->insertValue("d7", 37.5);
    dataset->insertValue("d8", 47.5);
    dataset->insertValue("d9", 15.0);

    Cross::ChartHistogramPlot *plot = new Cross::ChartHistogramPlot(dataset);
    plot->setRectangleEdge(Cross::LEFT);
    plot->setValueAxisVisible(true);
    plot->setCategoryAxisVisible(true);
    plot->setValueShowStyle(Cross::ChartHistogramPlot::VALUE_SHOW_STYLE_TOP);
    plot->setMargin(0);
    plot->setPadding(0);
    plot->setBarsPadding(0);
    Cross::Chart chart(plot);
    chart.draw(m_data->renderer, QRectF(10, 10, 400, 300));

    Cross::ChartHistogramPlot *plot2 = new Cross::ChartHistogramPlot(dataset);
    plot2->setRectangleEdge(Cross::TOP);
    plot2->setValueAxisVisible(true);
    plot2->setCategoryAxisVisible(true);
    plot2->setValueShowStyle(Cross::ChartHistogramPlot::VALUE_SHOW_STYLE_LEFT);
    plot2->setMargin(0);
    plot2->setPadding(0);
    plot2->setBarsPadding(0);
    Cross::Chart chart2(plot2);
    chart2.draw(m_data->renderer, QRectF(430, 10, 400, 300));

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
