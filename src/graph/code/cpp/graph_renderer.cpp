
#include "graph_renderer.h"
#include "graph_object.h"

#include <QPixmap>

namespace GraphLib 
{

    void GraphRenderer::setLineWidth(float ) 
    {
    }
    void GraphRenderer::setLineCaps(LineCaps )
    {
    }
    void GraphRenderer::setLineJoin(LineJoin ) 
    {
    }
    void GraphRenderer::setLineStyle(LineStyle ) 
    {
    }
    void GraphRenderer::setDashLength(float ) 
    {
    }
    void GraphRenderer::setFillStyle(FillStyle ) 
    {
    }

    int GraphRenderer::widthPixels()
    {
        return 0;
    }
    int GraphRenderer::heightPixels()
    {
        return 0;
    }

    void GraphRenderer::drawObject(Object *object)
    {
        if (object) {
            object->draw(this);
        }
    }

    void GraphRenderer::drawLine(Point *, Point *, Color *)
    {
    }

    void GraphRenderer::drawRect(Point *, Point *, 
            Color *)
    {
    }
    void GraphRenderer::fillRect(Point *, Point *, 
            Color *)
    {
    }

    void GraphRenderer::drawEllipse(Point *, 
            float , float , 
            Color *)
    {
    }
    void GraphRenderer::fillEllipse(Point *, 
            float , float , 
            Color *)
    {
    }

    void GraphRenderer::drawPolyline(Point *, int ,
            Color *)
    {
    }

    void GraphRenderer::drawPolygon(Point *, int ,
            Color *)
    {
    }
    void GraphRenderer::fillPolygon(Point *, int ,
            Color *)
    {
    }

    void GraphRenderer::fillPolygon(Point *points,
                             int pointCount,
                             QPixmap pixmap)
    {

    }

    void GraphRenderer::drawString(const QString &, Point *,
            int , 
            Color *)
    {
    }
    void GraphRenderer::drawImage(Point *point, float width, float height,
            GraphImage *image)
    {
        Q_UNUSED(point);
        Q_UNUSED(width);
        Q_UNUSED(height);
        Q_UNUSED(image);
    }

    void GraphRenderer::drawImage(Rectangle *targetRect,
                                  GraphImage *image,
                                  Rectangle *sourceRect)
    {
    }

    GraphFont *GraphRenderer::font()
    {
        return 0;
    }
    void GraphRenderer::setFont(GraphFont *)
    {
    }
}

