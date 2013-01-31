
#ifndef CHART_LOG_AXIS_H_
#define CHART_LOG_AXIS_H_

#include "graph_renderer.h"
#include "chart_axis.h"

#include <QPolygonF>
#include <QLineF>

class GraphLib::GraphRenderer;

namespace Cross 
{
    /*
    struct Range
    {
        double lower;
        double upper;
    };

    enum TickType {
        MINOR,
        MAJOR,
    };

    struct NumberTick
    {
        double value;
        TickType type;
    };

    struct TickUnit
    {
        double size;
        int minorTickCount;
    };
    */


    class ChartLogAxis : public ChartAxis
    {
        public:
            ChartLogAxis();
            ~ChartLogAxis();

        public:
            void setRange(Range range);
            void setRange(double lower, double upper);

            double lower();
            void setLower(double lower);

            double upper();
            void setUpper(double upper);

            void setTickUnit(TickUnit unit);

            double tickUnitSize();
            void setTickUnitSize(double size);

            int minorTickCount();
            void setMinorTickCount(int count);

            QList<NumberTick> ticks( GraphLib::GraphRenderer *, QRectF dataArea, RectangleEdge edge);
            void setArrayVisible(bool);
            void setAxisLabel(const QString &);
            QString axisLabel() const;

            void setTickLabelsVisible(bool);
            bool isTickLabelsVislble() const;

            void setMinorTickMarksVisible(bool);
            bool isMinorTickMarksVislble() const;

            void setMajorTickMarksVisible(bool);
            bool isMajorTickMarksVislble() const;

            void setAxisLabelVisible(bool);
            bool isAxisLabelVisible() const;

            QColor axisColor();
            void setAxisColor(QColor color);

            double axisWidth();
            void setAxisWidth(double width);

        public:
            void draw(GraphLib::GraphRenderer *, double cursor, 
                    QRectF plotArea, QRectF dataArea, RectangleEdge edge);

            void drawTickMarksAndLabels(GraphLib::GraphRenderer *,
                    double curosr, QRectF plotArea, QRectF dataArea, 
                    RectangleEdge edge);

            void drawLabel(QString label, GraphLib::GraphRenderer *, 
                    QRectF plotArea, QRectF dataArea, 
                    RectangleEdge edge);

            void drawAxisLine(GraphLib::GraphRenderer *, double cursor, 
                    QRectF dataArea, RectangleEdge edge);
            void drawArrays(GraphLib::GraphRenderer *, RectangleEdge edge);
            double chart2DToValue(double value, QRectF dataArea, RectangleEdge edge);
            double valueToChart2D(double value, QRectF dataArea, RectangleEdge edge);

        private:
            void init();

            QList<NumberTick> refreshTicks(GraphLib::GraphRenderer *, 
                    QRectF dataArea, RectangleEdge edge);

            QList<NumberTick> refreshTickHorizontal(GraphLib::GraphRenderer *, 
                    QRectF dataArea, RectangleEdge edge);

            QList<NumberTick> refreshTickVertical(GraphLib::GraphRenderer *, 
                    QRectF dataArea, RectangleEdge edge);

            int calculateVisibleTickCount();
            double calculateLowestVisibleTickValue();

            double calculateLog(double value);
            double calculateValue(double value);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif
