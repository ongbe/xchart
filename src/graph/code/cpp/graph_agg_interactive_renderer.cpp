
#include "graph_agg_interactive_renderer.h"
#include "graph_agg_renderer.h"

#include <QWidget>
#include <QPainter>
#include <QPaintDevice>
#include <QDebug>

namespace GraphLib 
{
    struct GraphAggInteractiveRenderer::PrivateData
    {
        GraphAggRenderer *renderer;
    };


    GraphAggInteractiveRenderer::GraphAggInteractiveRenderer(
            GraphRenderer *r)
    {
        m_data = new PrivateData;

        GraphAggRenderer *renderer = static_cast<GraphAggRenderer *>(r);
        m_data->renderer = renderer;
        renderer->setInteractiveRenderer(this);
    }
    GraphAggInteractiveRenderer::~GraphAggInteractiveRenderer()
    {
        delete m_data;
    }

    void GraphAggInteractiveRenderer::copyToWindow(QPaintDevice *device, 
            int x, int y, int width, int height)
    {
        if (!device)
            return;

        QPainter painter(device);
        painter.translate(x, -1 * y);
        m_data->renderer->copyToWindow(&painter,
                x, y, width, height);

    }


    void GraphAggInteractiveRenderer::copyToWindow(void *window, int x, int y, int width, int height)
    {
        QPaintDevice *device = static_cast<QPaintDevice *>(window);
        if (device) {
            copyToWindow(device, x, y, width, height);
        }
    }
    void GraphAggInteractiveRenderer::setSize(void *window, 
            int width, int height)
    {
        if (width < 0 || height < 0)
            return;

        GraphAggRenderer *renderer = m_data->renderer;
        if (!renderer)
            return;
        renderer->setWidthPixels(width);
        renderer->setHeightPixels(height);

    }
    void GraphAggInteractiveRenderer::clipRegionClear()
    {
        GraphAggRenderer *renderer = m_data->renderer;
        QRegion *clip = new QRegion;
        renderer->setClipRegion(clip);
    }
    void GraphAggInteractiveRenderer::clipRegionAddRect(Rectangle *rect)
    {
        GraphAggRenderer *renderer = m_data->renderer;
        QRegion *clip = renderer->clipRegion();

        QRect clipRect;
        clipRect.setX(rect->x());
        clipRect.setY(rect->y());
        clipRect.setWidth(rect->width());
        clipRect.setHeight(rect->height());

        *clip = clip->united(clipRect);
    }
    void GraphAggInteractiveRenderer::drawPixelLine(int x1, int y1, int x2, int y2, Color *color)
    {
    }
    void GraphAggInteractiveRenderer::drawPixelRect(int x, int y, int width, int height, Color *color)
    {
    }
    void GraphAggInteractiveRenderer::fillPixelRect(int x, int y, int width, int height, Color *color)
    {
    }
}
