
#ifndef CHART_CATEGORY_AXIS_H_
#define CHART_CATEGORY_AXIS_H_

#include "graph_renderer.h"
#include "chart_axis.h"

#include <QPolygonF>
#include <QLineF>

class GraphLib::GraphRenderer;

namespace Cross 
{
    struct CategoryTick
    {
        QString label;
        double value;
    };

    class ChartCategoryAxis : public ChartAxis
    {
        public:
            ChartCategoryAxis();
            ~ChartCategoryAxis();


        public:
            void setAxisLabel(const QString &);
            QString axisLabel() const;

            void setAxisLabelsVisible(bool);
            bool axisLabelsIsVisible();

            QColor axisColor();
            void setAxisColor(QColor color);


        public:
            void draw(GraphLib::GraphRenderer *, double cursor, 
                    QRectF plotArea, QRectF dataArea, RectangleEdge edge);

            void setCategoryTicks(QList<CategoryTick> );
        private:
            void drawTickMarksAndLabels(GraphLib::GraphRenderer *,
                    double curosr, QRectF plotArea, QRectF dataArea, 
                    RectangleEdge edge);

            void drawAxisLine(GraphLib::GraphRenderer *, double cursor, 
                    QRectF dataArea, RectangleEdge edge);

        private:
            void init();

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif
