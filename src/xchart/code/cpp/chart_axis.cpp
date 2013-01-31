
#include "graph_object.h"
#include "graph_color.h"
#include "chart_axis.h"

#include <vector>
#include <QMap>
#include <QList>
#include <QPointF>

namespace Cross
{
    class ChartAxis::PrivateData
    {
    };

    ChartAxis::ChartAxis()
    {
        init();
    }

    ChartAxis::~ChartAxis()
    {
        delete m_data;
    }

    void ChartAxis::init()
    {
        m_data = new PrivateData;
    }

    void ChartAxis::drawLabel(GraphLib::GraphRenderer *)
    {
    }
    void ChartAxis::drawAxisLine(GraphLib::GraphRenderer *renderer, double cursor, QRectF dataArea, int type)
    {
        if (renderer == NULL)
            return;
        QLineF line;
    }

}
