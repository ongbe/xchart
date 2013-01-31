
#include "graph_object.h"
#include "graph_color.h"

#include "chart_number_axis.h"

#include <vector>
#include <QMap>
#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QTransform>
#include <QDebug>

namespace Cross
{
    struct ChartNumberAxis::PrivateData
    {
        TickUnit tickUnit;
        Range range;
        QPolygonF xArray;
        QPolygonF yArray;
        bool isArrayVisible;

        QString axisLabel;
        bool axisLabelVisible;

        bool tickLabelsVisible;
        bool minorTickMarksVisible;
        bool majorTickMarksVisible;

        QColor axisColor;
        double axisWidth;
    };

    ChartNumberAxis::ChartNumberAxis() : ChartAxis()
    {
        init();
    }

    ChartNumberAxis::~ChartNumberAxis()
    {
        delete m_data;
    }

    void ChartNumberAxis::init()
    {
        m_data = new PrivateData;
        m_data->tickUnit.size = 40.0;
        m_data->tickUnit.minorTickCount = 3;
        m_data->range.lower = 0;
        m_data->range.upper = 200;
        m_data->xArray << QPointF(0, 0) << QPointF(-2, -2) << QPointF(2, -2);
        m_data->isArrayVisible = false;
        m_data->axisLabel = "坐标轴";
        m_data->tickLabelsVisible = true;
        m_data->minorTickMarksVisible = true;
        m_data->majorTickMarksVisible = true;
        m_data->axisLabelVisible = true;

        m_data->axisColor = Qt::black;
        m_data->axisWidth = 1.0;
    }

    void ChartNumberAxis::draw(GraphLib::GraphRenderer *renderer, 
            double cursor, 
            QRectF plotArea, 
            QRectF dataArea,
            RectangleEdge edge)
    {
        drawAxisLine(renderer, cursor, dataArea, edge);
        drawTickMarksAndLabels(renderer, cursor, plotArea, dataArea, edge);
        drawLabel(m_data->axisLabel, renderer, plotArea, dataArea, edge);
    }

    void ChartNumberAxis::drawTickMarksAndLabels(GraphLib::GraphRenderer *renderer,
            double cursor, QRectF plotArea, QRectF dataArea, 
            RectangleEdge edge)
    {
        QList<NumberTick> list = refreshTicks(renderer, dataArea, edge);
        QList<NumberTick>::iterator it = list.begin();
        for (it; it != list.end(); ++it) {
            NumberTick tick = *it;
            double ol = 0.0;
            double il = 0.0;

            double minTickLength = 2.50;
            double tickLength = 5.0;

            if (tick.type == MAJOR) {
                ol = tickLength;
                il = 0;
            } else {
                ol = minTickLength;
                il = 0;
            }
            double xx = valueToChart2D(tick.value, dataArea, edge);
            QLineF mark;
            QPointF pos;
            if (edge == TOP) {
                mark = QLineF(xx, cursor - ol, xx, cursor + il);
                pos = QPointF(xx, cursor - ol - 5.0);
            }
            if (edge == BOTTOM) {
                mark = QLineF(xx, cursor + ol, xx, cursor + il);
                pos = QPointF(xx - 5.0, cursor + ol + 15.0);
            }
            if (edge == LEFT) {
                mark = QLineF(cursor - ol, xx, cursor + il, xx);
                pos = QPointF(cursor - ol - 30.0, xx + 5.0);
            }
            if (edge == RIGHT) {
                mark = QLineF(cursor + ol, xx, cursor + il, xx);
                pos = QPointF(cursor + ol + 5.0, xx);
            }
            QPointF start = mark.p1();
            QPointF end = mark.p2();
            QColor color = m_data->axisColor;
            if (!m_data->minorTickMarksVisible && tick.type == MINOR) {
                continue;
            }
            if (!m_data->majorTickMarksVisible && tick.type == MAJOR) {
                continue;
            }

            renderer->setLineWidth(m_data->axisWidth);
            renderer->drawLine(&start, &end, &color);
            renderer->setLineWidth(1.0);


            //QString str = QString("%1").arg(tick.value);
            QString str = QString("%1").arg(tick.value, 0, 'f', 1);
            if (tick.type == MAJOR && m_data->tickLabelsVisible) 
                renderer->drawString(str, &pos, 1, &color);

        }

    }

    void ChartNumberAxis::drawAxisLine(GraphLib::GraphRenderer *renderer, 
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
        renderer->setLineWidth(m_data->axisWidth);
        renderer->drawLine(&p1, &p2, &color);
        renderer->setLineWidth(1.0);

        if (m_data->isArrayVisible) {
            if (edge == TOP) {
            }
            if (edge == BOTTOM) {
                QPointF end(p2.x() + 10, p2.y());
                QLineF line(p2, end);
                QPointF p11 = line.p1();
                QPointF p22 = line.p2();
                renderer->setLineWidth(m_data->axisWidth);
                renderer->drawLine(&p11, &p22, &color);
                renderer->setLineWidth(1.0);

                QTransform transform;
                transform = transform.translate(p22.x(), p22.y());

                QColor color = m_data->axisColor;
                QVector<QPointF> xArray;
                xArray << transform.map(QPointF(0, 0)) 
                    << transform.map(QPointF(-3, -3)) 
                    << transform.map(QPointF(-3, 3));

                renderer->setLineWidth(m_data->axisWidth);
                renderer->drawPolygon(xArray.data(), xArray.size(), &color);
                renderer->setLineWidth(1.0);
            }
            if (edge == LEFT) {
                QPointF end(p1.x(), p1.y() - 10);
                QLineF line(p1, end);
                QPointF p11 = line.p1();
                QPointF p22 = line.p2();

                renderer->setLineWidth(m_data->axisWidth);
                renderer->drawLine(&p11, &p22, &color);
                renderer->setLineWidth(1.0);

                QTransform transform;
                transform = transform.translate(p22.x(), p22.y());

                QColor color = m_data->axisColor;
                QVector<QPointF> xArray;
                xArray << transform.map(QPointF(0, 0)) 
                    << transform.map(QPointF(-3, 3)) 
                    << transform.map(QPointF(3, 3));

                renderer->setLineWidth(m_data->axisWidth);
                renderer->drawPolygon(xArray.data(), xArray.size(), &color);
                renderer->setLineWidth(1.0);
            }
            if (edge == RIGHT) {
            }
        }

    }

    QList<NumberTick> ChartNumberAxis::refreshTicks(
            GraphLib::GraphRenderer *renderer,
            QRectF dataArea, RectangleEdge edge)
    {
        if (edge == TOP || edge == BOTTOM) {
            return refreshTickHorizontal(renderer, dataArea, edge);
        } else {
            return refreshTickVertical(renderer, dataArea, edge);
        }
    }

    int ChartNumberAxis::calculateVisibleTickCount()
    {
        double unit = m_data->tickUnit.size;
        // 计算n是小数的情况
        // 保证n只要是有小数，就加一
        double n = (m_data->range.upper - m_data->range.lower) / unit;
        int m = int((n * 10)) % 10;
        if (m > 0) {
            n += 1;
        }
        int count = n + 1;
        return count;
    }

    double ChartNumberAxis::calculateLowestVisibleTickValue()
    {
        double unit = m_data->tickUnit.size;
        double index = m_data->range.lower / unit;
        return unit * index;
    }

    QList<NumberTick> ChartNumberAxis::refreshTickHorizontal(GraphLib::GraphRenderer *, 
            QRectF dataArea, RectangleEdge edge)
    {
        double unit = m_data->tickUnit.size;
        QList<NumberTick> list;
        double lowestValue = calculateLowestVisibleTickValue();
        double size = m_data->tickUnit.size;
        int count = calculateVisibleTickCount();
        int minorTickCount = m_data->tickUnit.minorTickCount;
        double minUnit = unit / (minorTickCount + 1);
        for (int i = 0; i < count - 1; i++) {
            double value = lowestValue * i;
            NumberTick tick;
            tick.value = i * unit + lowestValue;
            tick.type = MAJOR;
            list << tick;
            for (int j = 1; j < minorTickCount + 1; j++) {
                NumberTick minTick;
                minTick.value = tick.value + minUnit * j;
                minTick.type = MINOR;
                list << minTick;
            }
        }
        NumberTick tick;
        tick.value = (count - 1)* unit + lowestValue;
        tick.type = MAJOR;
        list << tick;
        return list;
    }

    QList<NumberTick> ChartNumberAxis::refreshTickVertical(GraphLib::GraphRenderer *, 
            QRectF dataArea, RectangleEdge  edge)
    {
        double unit = m_data->tickUnit.size;
        QList<NumberTick> list;
        double lowestValue = calculateLowestVisibleTickValue();
        double size = m_data->tickUnit.size;
        int count = calculateVisibleTickCount();
        int minorTickCount = m_data->tickUnit.minorTickCount;
        double minUnit = unit / (minorTickCount + 1);
        for (int i = 0; i < count - 1; i++) {
            double value = lowestValue * i;
            NumberTick tick;
            tick.value = i * unit + lowestValue;
            tick.type = MAJOR;
            list << tick;
            for (int j = 1; j < minorTickCount + 1; j++) {
                NumberTick minTick;
                minTick.value = tick.value + minUnit * j;
                minTick.type = MINOR;
                list << minTick;
            }
        }
        NumberTick tick;
        tick.value = (count - 1)* unit + lowestValue;
        tick.type = MAJOR;
        list << tick;
        return list;

    }

    void ChartNumberAxis::drawLabel(QString label, 
            GraphLib::GraphRenderer *renderer,
            QRectF plotArea, QRectF dataArea, 
            RectangleEdge edge)
    {
        if (!m_data->axisLabelVisible)
            return;
        QColor color(0, 0, 0);
        QPointF pos;
        if (edge == TOP) {
        }
        if (edge == BOTTOM) {
            double x = dataArea.bottomLeft().x() + dataArea.width() / 2.0 - 20;
            double y = dataArea.bottomRight().y() + 35;
            pos = QPointF(x, y);
        }
        if (edge == LEFT) {
            double x = dataArea.topLeft().x() - 35;
            double y = dataArea.topLeft().y() + dataArea.height() / 2.0 - 20;;
            pos = QPointF(x, y);
        }
        if (edge == RIGHT) {
        }

        renderer->drawString(label, &pos, 1, &color);
    }

    double ChartNumberAxis::valueToChart2D(double value, QRectF dataArea, RectangleEdge edge)
    {
        double axisMin = m_data->range.lower;
        double axisMax = m_data->range.upper;

        double min = 0.0;
        double max = 0.0;
        if (edge == TOP || edge == BOTTOM) {
            min = dataArea.topLeft().x();
            max = dataArea.topRight().x();
        } else {
            min = dataArea.topLeft().y();
            max = dataArea.bottomRight().y();
        }

        double result = 0.0;
        if (edge == TOP || edge == BOTTOM) {
            result = min + ((value - axisMin) / (axisMax - axisMin)) * (max - min);
        } else {
            result = max - ((value - axisMin) / (axisMax - axisMin)) * (max - min);
        }
        return result;
    }

    double ChartNumberAxis::chart2DToValue(double value, QRectF dataArea, RectangleEdge edge)
    {
        double axisMin = m_data->range.lower;
        double axisMax = m_data->range.upper;

        double min = 0.0;
        double max = 0.0;
        if (edge == TOP || edge == BOTTOM) {
            min = dataArea.topLeft().x();
            max = dataArea.topRight().x();
        } else {
            min = dataArea.topRight().y();
            max = dataArea.topLeft().y();
        }

        double result = 0.0;
        if (edge == TOP || edge == BOTTOM) {
            result = axisMin - (value - min) / (max - min) * (axisMax - axisMin);
        } else {
            result = axisMin + (value - min) / (max - min) * (axisMax - axisMin);
        }
        return result;

    }

    void ChartNumberAxis::setRange(Range range)
    {
        m_data->range.lower = range.lower;
        m_data->range.upper = range.upper;
    }

    void ChartNumberAxis::setRange(double lower, double upper)
    {
        m_data->range.lower = lower;
        m_data->range.upper = upper;
    }

    double ChartNumberAxis::lower()
    {
        return m_data->range.lower;
    }

    void ChartNumberAxis::setLower(double lower)
    {
        m_data->range.lower = lower;
    }

    double ChartNumberAxis::upper()
    {
        return m_data->range.upper;
    }

    void ChartNumberAxis::setUpper(double upper)
    {
        m_data->range.upper = upper;
    }

    void ChartNumberAxis::setTickUnit(TickUnit unit)
    {
        m_data->tickUnit.size = unit.size;
        m_data->tickUnit.minorTickCount = unit.minorTickCount;
    }

    double ChartNumberAxis::tickUnitSize()
    {
        return m_data->tickUnit.size;
    }

    void ChartNumberAxis::setTickUnitSize(double size)
    {
        m_data->tickUnit.size = size;
    }

    int ChartNumberAxis::minorTickCount()
    {
        return m_data->tickUnit.minorTickCount;
    }

    void ChartNumberAxis::setMinorTickCount(int count)
    {
        m_data->tickUnit.minorTickCount = count;
    }

    QList<NumberTick> ChartNumberAxis::ticks(
            GraphLib::GraphRenderer *renderer, 
            QRectF dataArea, 
            RectangleEdge edge)
    {
        return refreshTicks(renderer, dataArea, edge);
    }

    void ChartNumberAxis::setArrayVisible(bool flag)
    {
        m_data->isArrayVisible = flag;
    }

    void ChartNumberAxis::setAxisLabel(const QString &label)
    {
        m_data->axisLabel = label;
    }
    QString ChartNumberAxis::axisLabel() const
    {
        return m_data->axisLabel;
    }

    void ChartNumberAxis::setTickLabelsVisible(bool flag)
    {
        m_data->tickLabelsVisible = flag;
    }
    bool ChartNumberAxis::isTickLabelsVislble() const
    {
        return m_data->tickLabelsVisible;
    }

    void ChartNumberAxis::setMinorTickMarksVisible(bool flag)
    {
        m_data->minorTickMarksVisible = flag;
    }
    bool ChartNumberAxis::isMinorTickMarksVislble() const
    {
        return m_data->minorTickMarksVisible;
    }

    void ChartNumberAxis::setMajorTickMarksVisible(bool flag)
    {
        m_data->majorTickMarksVisible = flag;
    }
    bool ChartNumberAxis::isMajorTickMarksVislble() const
    {
        return m_data->majorTickMarksVisible;
    }

    void ChartNumberAxis::setAxisLabelVisible(bool flag)
    {
        m_data->axisLabelVisible = flag;
    }
    bool ChartNumberAxis::isAxisLabelVisible() const
    {
        return m_data->axisLabelVisible;
    }

    QColor ChartNumberAxis::axisColor()
    {
        return m_data->axisColor;
    }

    void ChartNumberAxis::setAxisColor(QColor color)
    {
        m_data->axisColor = color;
    }

    double ChartNumberAxis::axisWidth()
    {
        return m_data->axisWidth;
    }

    void ChartNumberAxis::setAxisWidth(double width)
    {
        m_data->axisWidth = width;
    }

}
