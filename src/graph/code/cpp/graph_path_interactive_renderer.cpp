
#include "graph_path_interactive_renderer.h"
#include "graph_path_renderer.h"
#include "graph_geometry.h"



#include <QWidget>
#include <QPainter>
#include <QPaintDevice>
#include <QPainterPath>
#include <QDebug>


namespace GraphLib
{

    struct GraphPathInteractiveRenderer::PrivateData
    {
        GraphPathRenderer *renderer;
    };

    GraphPathInteractiveRenderer::GraphPathInteractiveRenderer(GraphRenderer *r)
    {
        m_data = new PrivateData;

        GraphPathRenderer *renderer = static_cast<GraphPathRenderer *>(r);
        m_data->renderer = renderer;
        renderer->setInteractiveRenderer(this);
    }
    GraphPathInteractiveRenderer::~GraphPathInteractiveRenderer()
    {
        delete m_data;
    }

    void GraphPathInteractiveRenderer::copyToWindow(QPaintDevice *device, 
            int x, int y, int width, int height)
    {
        if (!device)
            return;

        QPainter painter(device);
        painter.translate(x, -1 * y);
        m_data->renderer->copyToWindow(&painter,
                x, y, width, height);

    }


    void GraphPathInteractiveRenderer::setSize(void *window, int width, int height)
    {
        if (width < 0 || height < 0)
            return;

        GraphPathRenderer *renderer = m_data->renderer;
        if (!renderer)
            return;
        renderer->setWidthPixels(width);
        renderer->setHeightPixels(height);
    }

    void GraphPathInteractiveRenderer::clipRegionClear()
    {
        GraphPathRenderer *renderer = m_data->renderer;
        QRegion *clip = new QRegion;
        renderer->setClipRegion(clip);
    }

    void GraphPathInteractiveRenderer::clipRegionAddRect(Rectangle *rect)
    {
        GraphPathRenderer *renderer = m_data->renderer;
        QRegion *clip = renderer->clipRegion();

        QRect clipRect;
        clipRect.setX(rect->x());
        clipRect.setY(rect->y());
        clipRect.setWidth(rect->width());
        clipRect.setHeight(rect->height());


        *clip = clip->united(clipRect);
    }

    void GraphPathInteractiveRenderer::drawPixelLine(int x1, int y1, 
            int x2, int y2, Color *color)
    {
        if (!m_data->renderer)
            return;

        Point start(x1, y1);
        Point end(x2, y2);
        m_data->renderer->drawLine(&start, &end, color);

    }
    void GraphPathInteractiveRenderer::drawPixelRect(int x, int y, 
            int width, int height, Color *color)
    {
        if (!m_data->renderer)
            return;

        Point ulCorner(x, y);
        Point lrCorner(x+width, y+height);
        m_data->renderer->drawRect(&ulCorner, &lrCorner, color);

    }
    void GraphPathInteractiveRenderer::fillPixelRect(int x, int y, 
            int width, int height, Color *color)
    {
        if (!m_data->renderer)
            return;

        Point ulCorner(x, y);
        Point lrCorner(x+width, y+height);
        m_data->renderer->fillRect(&ulCorner, &lrCorner, color);

    }

    void GraphPathInteractiveRenderer::copyToWindow(void *window, int x, int y, int width, int height)
    {
        QPaintDevice *device = static_cast<QPaintDevice *>(window);
        if (device) {
            copyToWindow(device, x, y, width, height);
        }
    }
}
