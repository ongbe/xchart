
#ifndef _CHART_TITLE_H_
#define _CHART_TITLE_H_

#include "graph_object.h"

#include <QPointF>

namespace Cross
{
    class ChartTitle
    {
        public:
            virtual ~ChartTitle();

        public:
            virtual void draw(GraphLib::GraphRenderer *,
                    QRectF titleArea) = 0;

        protected:
            ChartTitle();

    };
}


#endif
