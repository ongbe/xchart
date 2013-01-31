
#include "chart_xy_plot.h"
#include "graph_renderer.h"

#include "chart_label.h"

#include <QTransform>
#include <QVector>
#include <QPointF>

#include <QDebug>

namespace Cross
{
    struct ChartXYPlot::PrivateData
    {
        ChartXYDataset *dataset;
        ChartNumberAxis *xAxis;
        ChartNumberAxis *yAxis;

        double yAxisWidth;
        double xAxisHeight;

        bool xGridLineVisible;
        bool yGridLineVisible;

        QColor backgroundColor;
        QColor outlineColor;
        double outlineWidth;

        QColor gridLineColor;
        double gridLineWidth;

        bool xAxisIsVisible;
        bool yAxisIsVisible;
        bool gridIsVisible;

        QColor pointColor;
        double pointSize;

        bool outlineIsVisible;
    };

    ChartXYPlot::ChartXYPlot(ChartXYDataset *dataset) :
        ChartPlot(dataset)
    {
        m_data = new PrivateData;
        m_data->dataset = dataset;
        m_data->xAxis = new ChartNumberAxis;
        m_data->yAxis = new ChartNumberAxis;

        m_data->yAxisWidth = 60;
        m_data->xAxisHeight = 45;

        m_data->xGridLineVisible = true;
        m_data->yGridLineVisible = true;

        m_data->backgroundColor = Qt::white;
        m_data->outlineColor = Qt::gray;
        m_data->outlineWidth = 1.0;

        m_data->gridLineColor = Qt::red;
        m_data->gridLineWidth = 0.3;


        m_data->xAxisIsVisible = true;
        m_data->yAxisIsVisible = true;
        m_data->gridIsVisible = true;

        m_data->pointColor = Qt::red;
        m_data->pointSize = 10.0;

        m_data->outlineIsVisible = true;
    }

    ChartXYPlot::~ChartXYPlot()
    {
        delete m_data;
    }

    ChartXYDataset *ChartXYPlot::dataset()
    {
        return m_data->dataset;
    }

    void ChartXYPlot::setDataset(ChartXYDataset *dataset)
    {
        m_data->dataset = dataset;
    }

    void ChartXYPlot::draw(GraphLib::GraphRenderer *renderer,
            QRectF plotArea,
            QPointF anchor)
    {
        drawBackground(renderer, plotArea);
        drawOutline(renderer, plotArea);
        drawPoints(renderer, plotArea);

        if (m_data->gridIsVisible) {
            drawXGridLines(renderer, plotArea);
            drawYGridLines(renderer, plotArea);
        }

        drawAxis(renderer, plotArea);
    }

    void ChartXYPlot::drawBackground(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = m_data->backgroundColor;
        renderer->fillRect(&point1, &point2, &color);
    }

    void ChartXYPlot::drawOutline(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (!m_data->outlineIsVisible)
            return;

        GraphLib::Point point1 = rect.topLeft();
        GraphLib::Point point2 = rect.bottomRight();
        GraphLib::Color color = m_data->outlineColor;

        renderer->setLineWidth(m_data->outlineWidth);
        renderer->drawRect(&point1, &point2, &color);
        renderer->setLineWidth(1);
    }

    void ChartXYPlot::drawPoints(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        QStringList keys = m_data->dataset->keys();
        QStringList::iterator it = keys.begin();
        for (it; it != keys.end(); ++it) {
            QVector<QPointF> points = m_data->dataset->values(*it);
            drawPoint(renderer, rect, points);
        }
    }

    void ChartXYPlot::drawPoint(GraphLib::GraphRenderer *renderer, 
            QRectF rect, QVector<QPointF> &point)
    {
        QVector<QPointF>::iterator it = point.begin();
        for (it; it != point.end(); ++it) {
            QPointF center = *it;
            GraphLib::Color color = m_data->pointColor;
            double size = m_data->pointSize;

            QRectF dataRect = calculateDataArea(renderer, rect);
            double x = center.x();
            double y = center.y();
            double xx = m_data->xAxis->valueToChart2D(x, dataRect, BOTTOM);
            double yy = m_data->yAxis->valueToChart2D(y, dataRect, LEFT);

            QPointF c(xx, yy);

            if (dataRect.contains(c))
                renderer->fillEllipse(&c, size, size, &color);
        }
    }

    void ChartXYPlot::drawAxis(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        if (m_data->xAxis != NULL && m_data->xAxisIsVisible) {
            QRectF dataRect = calculateDataArea(renderer, rect);
            double cursor = dataRect.bottomRight().y();
            m_data->xAxis->draw(renderer,
                    cursor,
                    rect,
                    dataRect,
                    Cross::BOTTOM);
        }
        if (m_data->yAxis != NULL && m_data->yAxisIsVisible) {
            QRectF dataRect = calculateDataArea(renderer, rect);
            double cursor = dataRect.topLeft().x();
            m_data->yAxis->draw(renderer,
                    cursor,
                    rect,
                    dataRect,
                    Cross::LEFT);
        }
    }

    void ChartXYPlot::setXAxis(ChartNumberAxis *x)
    {
        if (x == NULL)
            return;
        m_data->xAxis = x;
    }
    void ChartXYPlot::setYAxis(ChartNumberAxis *y)
    {
        if (y == NULL)
            return;
        m_data->yAxis = y;
    }
    void ChartXYPlot::setXAxisRange(double lower, double upper)
    {
        m_data->xAxis->setRange(lower, upper);
    }
    void ChartXYPlot::setYAxisRange(double lower, double upper)
    {
        m_data->yAxis->setRange(lower, upper);
    }

    void ChartXYPlot::drawXGridLines(GraphLib::GraphRenderer *renderer, QRectF plotArea)
    {
        if (!m_data->xGridLineVisible)
            return;

        QRectF dataArea = calculateDataArea(renderer, plotArea);
        double x = dataArea.x();

        QList<NumberTick> list = m_data->yAxis->ticks(renderer, dataArea, Cross::LEFT);
        QList<NumberTick>::iterator it = list.begin();
        for (it; it != list.end(); ++it) {
            NumberTick tick = *it;
            if (tick.type == MAJOR) {
                double y = tick.value;
                double yy = m_data->yAxis->valueToChart2D(y, dataArea, LEFT);
                QLineF line(x, yy, dataArea.x() + dataArea.width(), yy);

                QPointF start = line.p1();
                QPointF end = line.p2();
                QColor color = m_data->gridLineColor;

                renderer->setLineWidth(m_data->gridLineWidth);
                renderer->drawLine(&start, &end, &color);
                renderer->setLineWidth(1.0);
            }
        }
    }

    void ChartXYPlot::drawYGridLines(GraphLib::GraphRenderer *renderer, QRectF plotArea)
    {
        if (!m_data->yGridLineVisible)
            return;

        QRectF dataArea = calculateDataArea(renderer, plotArea);
        double y = dataArea.y();

        QList<NumberTick> list = m_data->xAxis->ticks(renderer, dataArea, Cross::BOTTOM);
        QList<NumberTick>::iterator it = list.begin();
        for (it; it != list.end(); ++it) {
            NumberTick tick = *it;
            if (tick.type == MAJOR) {
                double x = tick.value;
                double xx = m_data->xAxis->valueToChart2D(x,
                        dataArea, BOTTOM);
                QLineF line(xx, y, xx, y + dataArea.height());

                QPointF start = line.p1();
                QPointF end = line.p2();
                QColor color = m_data->gridLineColor;

                renderer->setLineWidth(m_data->gridLineWidth);
                renderer->drawLine(&start, &end, &color);
                renderer->setLineWidth(1.0);
            }
        }

    }

    QRectF ChartXYPlot::calculateDataArea(GraphLib::GraphRenderer *renderer, QRectF rect)
    {
        return QRectF(rect.left() + m_data->yAxisWidth,
                      rect.top() + 10,
                      rect.width() - m_data->yAxisWidth - 10,
                      rect.height() - m_data->xAxisHeight - 10);
    }

    ChartNumberAxis* ChartXYPlot::XAxis()
    {
        return m_data->xAxis;
    }
    ChartNumberAxis* ChartXYPlot::YAxis()
    {
        return m_data->yAxis;
    }

    void ChartXYPlot::setXGridLineVisible(bool flag)
    {
        m_data->xGridLineVisible = flag;
    }
    void ChartXYPlot::setYGridLineVisible(bool flag)
    {
        m_data->yGridLineVisible = flag;
    }

    QColor ChartXYPlot::backgroundColor()
    {
        return m_data->backgroundColor;
    }

    void ChartXYPlot::setBackgroundColor(QColor color)
    {
        m_data->backgroundColor = color;
    }

    bool ChartXYPlot::outlineIsVisible()
    {
        return m_data->outlineIsVisible;
    }

    void ChartXYPlot::setOutlineVisible(bool visible)
    {
        m_data->outlineIsVisible = visible;
    }

    QColor ChartXYPlot::outlineColor()
    {
        return m_data->outlineColor;
    }

    void ChartXYPlot::setOutlineColor(QColor color)
    {
        m_data->outlineColor = color;
    }

    double ChartXYPlot::outlineWidth()
    {
        return m_data->outlineWidth;
    }

    void ChartXYPlot::setOutlineWidth(double width)
    {
        m_data->outlineWidth = width;
    }


    QColor ChartXYPlot::gridLineColor()
    {
        return m_data->gridLineColor;
    }

    void ChartXYPlot::setGridLineColor(QColor color)
    {
        m_data->gridLineColor = color;
    }

    double ChartXYPlot::gridLineWidth()
    {
        return m_data->gridLineWidth;
    }

    void ChartXYPlot::setGridLineWidth(double width)
    {
        m_data->gridLineWidth = width;
    }


    bool ChartXYPlot::xAxisIsVisible()
    {
        return m_data->xAxisIsVisible;
    }

    void ChartXYPlot::setXAxisVisible(bool visible)
    {
        m_data->xAxisIsVisible = visible;
    }

    bool ChartXYPlot::yAxisIsVisible()
    {
        return m_data->yAxisIsVisible;
    }

    void ChartXYPlot::setYAxisVisible(bool visible)
    {
        m_data->yAxisIsVisible = visible;
    }

    bool ChartXYPlot::gridIsVisible()
    {
        return m_data->gridIsVisible;
    }

    void ChartXYPlot::setGridVisible(bool visible)
    {
        m_data->gridIsVisible = visible;
    }


    QColor ChartXYPlot::pointColor()
    {
        return m_data->pointColor;
    }

    void ChartXYPlot::setPointColor(QColor color)
    {
        m_data->pointColor = color;
    }

    double ChartXYPlot::pointSize()
    {
        return m_data->pointSize;
    }

    void ChartXYPlot::setPointSize(double size)
    {
        m_data->pointSize = size;
    }

}
