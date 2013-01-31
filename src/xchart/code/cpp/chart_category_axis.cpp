
#include "graph_object.h"
#include "graph_color.h"

#include "chart_category_axis.h"

#include <QList>
#include <QPointF>
#include <QDebug>

namespace Cross
{
    struct ChartCategoryAxis::PrivateData
    {
        QList<CategoryTick> ticks;

        QString axisLabel;
        bool axisLabelsIsVisible;
        QColor axisColor;
    };

    ChartCategoryAxis::ChartCategoryAxis() : ChartAxis()
    {
        init();
    }

    ChartCategoryAxis::~ChartCategoryAxis()
    {
    }

    void ChartCategoryAxis::init()
    {
        m_data = new PrivateData;

        m_data->axisLabelsIsVisible = true;
        m_data->axisColor = Qt::black;
    }

    void ChartCategoryAxis::draw(GraphLib::GraphRenderer *renderer, 
            double cursor, 
            QRectF plotArea, 
            QRectF dataArea,
            RectangleEdge edge)
    {
        drawAxisLine(renderer, cursor, dataArea, edge);
        drawTickMarksAndLabels(renderer, cursor, plotArea, dataArea, edge);
    }

    void ChartCategoryAxis::drawTickMarksAndLabels(GraphLib::GraphRenderer *renderer,
            double cursor, QRectF plotArea, QRectF dataArea, 
            RectangleEdge edge)
    {
        QList<CategoryTick>::iterator it = m_data->ticks.begin();
        for (it; it != m_data->ticks.end(); ++it) {
            QLineF mark;
            QPointF pos;
            if (edge == TOP ) {
            } else if (edge == BOTTOM) {
                double x = (*it).value;
                double y = cursor;
                mark = QLineF(x, y, x, y + 5.0);
                pos = QPointF(mark.p2().x() - 3.0, mark.p2().y() + 15.0);
            } else if (edge == LEFT) {
                double x = cursor;
                double y = (*it).value;
                mark = QLineF(x, y, x - 5.0, y);
                pos = QPointF(mark.p2().x() - 15.0, mark.p2().y());
            } else if (edge == RIGHT) {
            }

            QColor color = m_data->axisColor;
            QPointF p1 = mark.p1();
            QPointF p2 = mark.p2();


            if (m_data->axisLabelsIsVisible) {
                renderer->drawLine(&p1, &p2, &color);
                renderer->drawString((*it).label, &pos, 1, &color);
            }
        }
    }

    void ChartCategoryAxis::drawAxisLine(GraphLib::GraphRenderer *renderer, 
            double cursor, QRectF dataArea, RectangleEdge edge)
    {
        QLineF axisLine;
        if (edge == TOP || edge == BOTTOM) {
            axisLine = QLineF(dataArea.x(), cursor, dataArea.topRight().x(), cursor);
        } else {
            axisLine = QLineF(cursor, dataArea.topRight().y(), cursor, dataArea.bottomRight().y());
        }
        QColor color = m_data->axisColor;

        QPointF p1 = axisLine.p1();
        QPointF p2 = axisLine.p2();
        renderer->drawLine(&p1, &p2, &color);
    }

    void ChartCategoryAxis::setCategoryTicks(QList<CategoryTick> list)
    {
        m_data->ticks = list;
    }

    void ChartCategoryAxis::setAxisLabelsVisible(bool visible)
    {
        m_data->axisLabelsIsVisible = visible;
    }

    bool ChartCategoryAxis::axisLabelsIsVisible()
    {
        return m_data->axisLabelsIsVisible;
    }

    QColor ChartCategoryAxis::axisColor()
    {
        return m_data->axisColor;
    }

    void ChartCategoryAxis::setAxisColor(QColor color)
    {
        m_data->axisColor = color;
    }

    void ChartCategoryAxis::setAxisLabel(const QString &label)
    {
        m_data->axisLabel = label;
    }

    QString ChartCategoryAxis::axisLabel() const
    {
        return m_data->axisLabel;
    }
}
