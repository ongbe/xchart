#ifndef GRAPH_CLIPPER_H_
#define GRAPH_CLIPPER_H_

#include "graph_global.h"
#include "graph_array.h"
#include "graph_double_rect.h"
#include "graph_double_interval.h"
#include <QPolygon>
#include <QPolygonF>

class QRect;

namespace GraphLib
{
    /*!
      \brief Some clipping algos
     */

    class GraphClipper
    {
        public:
            static QPolygon clipPolygon(const QRect &, const QPolygon &);
            static QPolygonF clipPolygonF(
                    const GraphDoubleRect &, 
                    const QPolygonF &);

            static GraphArray<GraphDoubleInterval> clipCircle(
                    const GraphDoubleRect &, 
                    const GraphDoublePoint &, 
                    double radius);
    };

}
#endif

