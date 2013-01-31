
#ifndef _CHART_PLOT_H_
#define _CHART_PLOT_H_

#include "graph_object.h"

namespace Cross
{
    class GraphLib::GraphRenderer;
    class ChartDataset;
    class ChartPlot
    {
        public: 
            virtual ~ChartPlot() {}

            virtual void setDataset(ChartDataset *) {}

            virtual void draw(GraphLib::GraphRenderer *,
                    QRectF plotArea, QPointF anchor) = 0;

        protected:
            ChartPlot(ChartDataset *) {}
    };

};


#endif
