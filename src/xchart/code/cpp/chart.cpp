
#include "chart.h"
#include "chart_plot.h"
#include "chart_text_title.h"

#include "graph_geometry.h"
#include "graph_color.h"
#include "graph_renderer.h"

#include <QDebug>


namespace Cross
{
    struct Chart::PrivateData
    {
        ChartPlot *plot;
        ChartTextTitle *title;


        bool boundingBoxIsVisible;

        GraphLib::Point pos;
        double width;
        double height;

        double margin;

        double titleHeight;
        bool titleIsVisible;
    };

    Chart::Chart(ChartPlot *plot)
    {
        m_data = new PrivateData;
        m_data->plot = plot;
        m_data->title = new ChartTextTitle;
        m_data->title->setText("demo plot");

        m_data->pos = QPointF(10, 10);
        m_data->width = 300;
        m_data->height = 300;
        m_data->margin = 20;
        m_data->titleHeight = 30;

        m_data->boundingBoxIsVisible = true;
        m_data->titleIsVisible = true;
    }

    Chart::~Chart()
    {
        delete m_data;
    }

    QString Chart::title()
    {
        return m_data->title->text();
    }

    void Chart::setTitle(QString text)
    {
        m_data->title->setText(text);
    }


    bool Chart::titleIsVisible()
    {
        return m_data->titleIsVisible;
    }

    void Chart::setTitleVisible(bool visible)
    {
        m_data->titleIsVisible = visible;
    }

    double Chart::titleHeight()
    {
        return m_data->titleHeight;
    }

    void Chart::setTitleHeight(double height)
    {
        m_data->titleHeight = height;
    }

    bool Chart::boundingBoxIsVisible()
    {
        return m_data->boundingBoxIsVisible;
    }

    void Chart::setBoundingBoxVisibleVisible(bool visible)
    {
        m_data->boundingBoxIsVisible = visible;
    }

    GraphLib::Point Chart::pos()
    {
        return m_data->pos;
    }

    void Chart::setPos(GraphLib::Point pos)
    {
        m_data->pos = pos;
    }

    double Chart::width()
    {
        return m_data->width;
    }

    void Chart::setWidth(double width)
    {
        m_data->width = width;
    }

    double Chart::height()
    {
        return m_data->height;
    }

    void Chart::setHeight(double height)
    {
        m_data->height = height;
    }

    GraphLib::Rectangle Chart::boundingBox()
    {
        return QRectF(m_data->pos,
                      QSizeF(m_data->width, m_data->height));
    }

    GraphLib::Rectangle Chart::enclosingBox()
    {
        return QRectF(m_data->pos,
                      QSizeF(m_data->width, m_data->height));
    }

    void Chart::draw(GraphLib::GraphRenderer *renderer)
    {
        draw(renderer, enclosingBox());
    }

    QRectF Chart::plotArea(QRectF chartArea)
    {
        double margin = m_data->margin;
        double chartLeft = chartArea.left();
        double chartRight = chartArea.right();
        double chartTop = chartArea.top();
        double chartBottom = chartArea.bottom();
        double chartWidth = chartArea.width();
        double chartHeight = chartArea.height();

        QRectF titleArea = QRectF(chartLeft + margin, 
                chartTop + margin,
                chartWidth - margin * 2,
                m_data->titleHeight);

        QRectF plotArea = QRectF(chartLeft + margin, 
                chartTop + margin + m_data->titleHeight,
                chartWidth - margin * 2,
                chartHeight - m_data->titleHeight - margin * 2);

        return plotArea;
    }
    void Chart::draw(GraphLib::GraphRenderer *renderer,
            GraphLib::Rectangle chartArea)
    {
        if (renderer == NULL)
            return;

        double margin = m_data->margin;
        double chartLeft = chartArea.left();
        double chartRight = chartArea.right();
        double chartTop = chartArea.top();
        double chartBottom = chartArea.bottom();
        double chartWidth = chartArea.width();
        double chartHeight = chartArea.height();

        QRectF titleArea = QRectF(chartLeft + margin, 
                chartTop + margin,
                chartWidth - margin * 2,
                m_data->titleHeight);

        QRectF plotArea =  this->plotArea(chartArea);

        if (chartHeight - margin - 
                m_data->titleHeight - margin - margin <= 0)
            return;
        if (chartWidth - margin - margin <= 0)
            return;

        if (m_data->boundingBoxIsVisible) {
            GraphLib::Color color;
            GraphLib::Point ulCorner = chartArea.topLeft();
            GraphLib::Point lrCorner = chartArea.bottomRight();
            color = QColor(230, 248, 255);
            renderer->fillRect(&ulCorner, &lrCorner, &color);
            color = QColor(Qt::gray);
            renderer->drawRect(&ulCorner, &lrCorner, &color);
        }

        if (m_data->titleIsVisible && m_data->title) {
            m_data->title->draw(renderer, titleArea);
        }

        if (m_data->plot) {
            m_data->plot->draw(renderer, plotArea, QPointF());
        }

    }

}

