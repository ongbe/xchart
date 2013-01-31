
#ifndef CHART_AXIS_H_
#define CHART_AXIS_H_

#include "graph_renderer.h"

class GraphLib::GraphRenderer;

namespace Cross 
{
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
        QString label;
        double value;
        TickType type;
    };

    struct TickUnit
    {
        double size;
        int minorTickCount;
    };



    enum RectangleEdge
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
    };

    class ChartAxis
    {
        public:
            ChartAxis();
            ~ChartAxis();

        public:

            virtual void draw(GraphLib::GraphRenderer *, double cursor, 
                    QRectF plotArea, QRectF dataArea, RectangleEdge edge) = 0;

        protected:
            void drawLabel(GraphLib::GraphRenderer *);
            virtual void drawAxisLine(GraphLib::GraphRenderer *, double cursor, QRectF dataArea, int type);

        private:
            void init();

        private:
            class PrivateData;
            PrivateData *m_data;
    };
}
#endif
