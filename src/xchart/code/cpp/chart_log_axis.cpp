
#include "graph_object.h"
#include "graph_color.h"

#include "chart_log_axis.h"

#include <vector>
#include <math.h>

#include <QMap>
#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QTransform>
#include <QDebug>

namespace Cross
{
    struct ChartLogAxis::PrivateData
    {
        PrivateData() : base(10.0), baseLog(log(10.0))
        {
        }
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

        double base;
        double baseLog;
    };

    ChartLogAxis::ChartLogAxis() : ChartAxis()
    {
        init();
    }

    ChartLogAxis::~ChartLogAxis()
    {
        delete m_data;
    }

    void ChartLogAxis::init()
    {
        m_data = new PrivateData;
        m_data->tickUnit.size = 0.0;
        m_data->tickUnit.minorTickCount = 8;
        m_data->range.lower = 0.01;
        m_data->range.upper = 100.0;
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

    void ChartLogAxis::draw(GraphLib::GraphRenderer *renderer, 
            double cursor, 
            QRectF plotArea, 
            QRectF dataArea,
            RectangleEdge edge)
    {
        drawAxisLine(renderer, cursor, dataArea, edge);
        drawTickMarksAndLabels(renderer, cursor, plotArea, dataArea, edge);
        drawLabel(m_data->axisLabel, renderer, plotArea, dataArea, edge);
    }

    void ChartLogAxis::drawTickMarksAndLabels(GraphLib::GraphRenderer *renderer,
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
            QString str = QString("%1").arg(tick.value, 0, 'f', 2);
            if (tick.type == MAJOR && m_data->tickLabelsVisible) 
                renderer->drawString(str, &pos, 1, &color);

        }

    }

    void ChartLogAxis::drawAxisLine(GraphLib::GraphRenderer *renderer, 
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

    QList<NumberTick> ChartLogAxis::refreshTicks(
            GraphLib::GraphRenderer *renderer,
            QRectF dataArea, RectangleEdge edge)
    {
        if (edge == TOP || edge == BOTTOM) {
            return refreshTickHorizontal(renderer, dataArea, edge);
        } else {
            return refreshTickVertical(renderer, dataArea, edge);
        }
    }

    int ChartLogAxis::calculateVisibleTickCount()
    {
        double unit = m_data->tickUnit.size;
        int count = (m_data->range.upper - m_data->range.lower) / unit + 1;
        return count;
    }

    double ChartLogAxis::calculateLowestVisibleTickValue()
    {
        double unit = m_data->tickUnit.size;
        double index = m_data->range.lower / unit;
        return unit * index;
    }

    QList<NumberTick> ChartLogAxis::refreshTickHorizontal(GraphLib::GraphRenderer *, 
            QRectF dataArea, RectangleEdge edge)
    {
        QList<NumberTick> ticks;

        double upper = m_data->range.upper;
        double lower = m_data->range.lower;
        double stepSize = 2;

        double width = log10(upper) - log10(lower);
        int numberTicks = width / stepSize + 1;

        double lxmin = log(lower);
        double lxmax = log(upper);
        numberTicks = 5;
        double lstep = (lxmax - lxmin)/double(numberTicks - 1);

        NumberTick firstTick;
        firstTick.value = 0.01;
        firstTick.type = MAJOR;
        //firstTick.label = QString("%1").arg(0.01);
        ticks << firstTick;

        for (int i = 0; i < numberTicks - 1; ++i) {
            double value = exp(lxmin + double(i) * lstep);
            NumberTick tick;
            tick.value = value;
            tick.type = MAJOR;
            ticks << tick;
            int minorTickCount = m_data->tickUnit.minorTickCount;
            for (int j = 1; j < minorTickCount + 1; j++) {
                NumberTick minTick;
                //minTick.value = pow(10, i) + pow(10,i)*j;
                minTick.value = value * j;
                minTick.type = MINOR;
                ticks << minTick;
            }

        }

        NumberTick lastTick;
        lastTick.value = upper;
        lastTick.type = MAJOR;
        //lastTick.label= QString("%1").arg(100);

        ticks << lastTick;


        return ticks;
        /*
        QList<NumberTick> ticks;
        double start = calculateLog(m_data->range.lower);
        double end = calculateLog(m_data->range.upper);

        double current = start;

        while (current <= end) {
            double v = calculateValue(current);
            NumberTick tick;
            tick.value = v;
            tick.type = MAJOR;
            double next = pow(m_data->base, current + m_data->tickUnit.size);
            int minorTickCount = m_data->tickUnit.minorTickCount;
            for (int j = 1; j < minorTickCount + 1; j++) {
                NumberTick minTick;
                minTick.value = next;
                minTick.type = MINOR;
                ticks << minTick;
            }
            current = current + m_data->tickUnit.size;
            ticks << tick;
        }

        return ticks;
        */
    }

    QList<NumberTick> ChartLogAxis::refreshTickVertical(GraphLib::GraphRenderer *, 
            QRectF dataArea, RectangleEdge  edge)
    {
        QList<NumberTick> ticks;
        double start = calculateLog(m_data->range.lower);
        double end = calculateLog(m_data->range.upper);
        double current = start;

        while (current <= end) {
            double v = calculateValue(current);
            NumberTick tick;
            tick.value = v;
            tick.type = MAJOR;
            double next = pow(m_data->base, current + m_data->tickUnit.size);
            /*
               int minorTickCount = m_data->tickUnit.minorTickCount;
               for (int j = 1; j < minorTickCount + 1; j++) {
               NumberTick minTick;
               minTick.value = next;
               minTick.type = MINOR;
               ticks << minTick;
               }
               */
            current = current + m_data->tickUnit.size;
            ticks << tick;
        }

        return ticks;

    }


    void ChartLogAxis::drawLabel(QString label, 
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

    double ChartLogAxis::valueToChart2D(double value, QRectF dataArea, RectangleEdge edge)
    {
        /*
        double axisMin = m_data->range.lower;
        double axisMax = m_data->range.upper;
        */
        double axisMin = log(m_data->range.lower);
        double axisMax = log(m_data->range.upper);

        double min = 0.0;
        double max = 0.0;
        if (edge == TOP || edge == BOTTOM) {
            min = dataArea.topLeft().x();
            max = dataArea.topRight().x();
        } else {
            min = dataArea.topLeft().y();
            max = dataArea.bottomRight().y();
        }

        value = log(value);
        double result = 0.0;
        if (edge == TOP || edge == BOTTOM) {
            result = min + ((value - axisMin) / (axisMax - axisMin)) * (max - min);
        } else {
            result = max - ((value - axisMin) / (axisMax - axisMin)) * (max - min);
        }
        return result;
    }

    double ChartLogAxis::chart2DToValue(double value, QRectF dataArea, RectangleEdge edge)
    {
        double axisMin = log(m_data->range.lower);
        double axisMax = log(m_data->range.upper);

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
        return pow(10.0, result);

    }

    void ChartLogAxis::setRange(Range range)
    {
        m_data->range.lower = range.lower;
        m_data->range.upper = range.upper;
    }

    void ChartLogAxis::setRange(double lower, double upper)
    {
        m_data->range.lower = lower;
        m_data->range.upper = upper;
    }

    double ChartLogAxis::lower()
    {
        return m_data->range.lower;
    }

    void ChartLogAxis::setLower(double lower)
    {
        m_data->range.lower = lower;
    }

    double ChartLogAxis::upper()
    {
        return m_data->range.upper;
    }

    void ChartLogAxis::setUpper(double upper)
    {
        m_data->range.upper = upper;
    }

    void ChartLogAxis::setTickUnit(TickUnit unit)
    {
        m_data->tickUnit.size = unit.size;
        m_data->tickUnit.minorTickCount = unit.minorTickCount;
    }

    double ChartLogAxis::tickUnitSize()
    {
        return m_data->tickUnit.size;
    }

    void ChartLogAxis::setTickUnitSize(double size)
    {
        m_data->tickUnit.size = size;
    }

    int ChartLogAxis::minorTickCount()
    {
        return m_data->tickUnit.minorTickCount;
    }

    void ChartLogAxis::setMinorTickCount(int count)
    {
        m_data->tickUnit.minorTickCount = count;
    }

    QList<NumberTick> ChartLogAxis::ticks(
            GraphLib::GraphRenderer *renderer, 
            QRectF dataArea, 
            RectangleEdge edge)
    {
        return refreshTicks(renderer, dataArea, edge);
    }

    void ChartLogAxis::setArrayVisible(bool flag)
    {
        m_data->isArrayVisible = flag;
    }

    void ChartLogAxis::setAxisLabel(const QString &label)
    {
        m_data->axisLabel = label;
    }
    QString ChartLogAxis::axisLabel() const
    {
        return m_data->axisLabel;
    }

    void ChartLogAxis::setTickLabelsVisible(bool flag)
    {
        m_data->tickLabelsVisible = flag;
    }
    bool ChartLogAxis::isTickLabelsVislble() const
    {
        return m_data->tickLabelsVisible;
    }

    void ChartLogAxis::setMinorTickMarksVisible(bool flag)
    {
        m_data->minorTickMarksVisible = flag;
    }
    bool ChartLogAxis::isMinorTickMarksVislble() const
    {
        return m_data->minorTickMarksVisible;
    }

    void ChartLogAxis::setMajorTickMarksVisible(bool flag)
    {
        m_data->majorTickMarksVisible = flag;
    }
    bool ChartLogAxis::isMajorTickMarksVislble() const
    {
        return m_data->majorTickMarksVisible;
    }

    void ChartLogAxis::setAxisLabelVisible(bool flag)
    {
        m_data->axisLabelVisible = flag;
    }
    bool ChartLogAxis::isAxisLabelVisible() const
    {
        return m_data->axisLabelVisible;
    }

    QColor ChartLogAxis::axisColor()
    {
        return m_data->axisColor;
    }

    void ChartLogAxis::setAxisColor(QColor color)
    {
        m_data->axisColor = color;
    }

    double ChartLogAxis::axisWidth()
    {
        return m_data->axisWidth;
    }

    void ChartLogAxis::setAxisWidth(double width)
    {
        m_data->axisWidth = width;
    }

    double ChartLogAxis::calculateLog(double value)
    {
        if (value == 0)
            value = 0.0001;
        return log(value) / m_data->baseLog;
    }

    double ChartLogAxis::calculateValue(double log)
    {
        return pow(m_data->base, log);
    }

}
