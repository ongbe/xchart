
#include "graph_qt_interactive_renderer.h"
#include "graph_qt_renderer.h"
#include "graph_geometry.h"



#include <QWidget>
#include <QPainter>
#include <QPaintDevice>
#include <QPainterPath>
#include <QDebug>

#define QIMAGE_LIMIT 32768

namespace GraphLib
{

    struct GraphQtInteractiveRenderer::PrivateData
    {
        GraphQtRenderer *renderer;
    };

    GraphQtInteractiveRenderer::GraphQtInteractiveRenderer(GraphRenderer *r)
    {
        m_data = new PrivateData;

        GraphQtRenderer *renderer = static_cast<GraphQtRenderer *>(r);
        m_data->renderer = renderer;
        renderer->setInteractiveRenderer(this);
    }
    GraphQtInteractiveRenderer::~GraphQtInteractiveRenderer()
    {
        delete m_data;
    }

    void GraphQtInteractiveRenderer::copyToWindow(QPaintDevice *device, 
            int x, int y, int width, int height)
    {
        if (!device)
            return;

        QImage *pixmap = m_data->renderer->pixmap();
        if (!pixmap)
            return;

        QPainter painter(device);
        QPixmap dst = QPixmap::fromImage(pixmap->copy(x, y, width, height));
        painter.drawPixmap(0, 0, width, height, dst);
    }

    void GraphQtInteractiveRenderer::setSize(void *window, int width, int height)
    {
        if (width < 0 || height < 0)
            return;
        if (width >= QIMAGE_LIMIT)
            width = QIMAGE_LIMIT - 1;
        if (height >= QIMAGE_LIMIT)
            height = QIMAGE_LIMIT - 1;


        QImage *pixmap = static_cast<QImage *>(window);
        if (!pixmap) {
            pixmap = new QImage(width, height, QImage::Format_ARGB32_Premultiplied);
            pixmap->fill(0);
        }
        m_data->renderer->setPixmap(pixmap);
    }

    void GraphQtInteractiveRenderer::clipRegionClear()
    {
        GraphQtRenderer *renderer = m_data->renderer;
        QRegion *clip = new QRegion;
        renderer->setClipRegion(clip);
    }
    void GraphQtInteractiveRenderer::clipRegionAddRect(Rectangle *rect)
    {
        GraphQtRenderer *renderer = m_data->renderer;
        QRegion *clip = renderer->clipRegion();

        QRect clipRect;
        clipRect.setX(rect->x());
        clipRect.setY(rect->y());
        clipRect.setWidth(rect->width());
        clipRect.setHeight(rect->height());


        *clip = clip->united(clipRect);


        QPainterPath path;
        path.addRegion(*clip);
        QPainter *painter = m_data->renderer->painter();
        painter->setClipping(true);
        painter->setClipPath(path);
    }
    void GraphQtInteractiveRenderer::drawPixelLine(int x1, int y1, int x2, int y2, Color *color)
    {
        if (!m_data->renderer)
            return;

        int targetWidth = m_data->renderer->widthPixels();
        int targetHeight = m_data->renderer->heightPixels();
        if ((x1 < 0 && x2 < 0) 
                || (y1 < 0 && y2 < 0) 
                || (x1 > targetWidth && x2 > targetWidth) 
                || (y1 > targetHeight && y2 > targetHeight))
            return;


        QPainter *painter = m_data->renderer->painter();
        painter->save();
        QPen pen(*color);
        painter->setPen(pen);
        painter->drawLine(x1, y1, x2, y2);
        painter->restore();

    }
    void GraphQtInteractiveRenderer::drawPixelRect(int x, int y, int width, int height, Color *color)
    {
        if (!m_data->renderer)
            return;

        int targetWidth = m_data->renderer->widthPixels();
        int targetHeight = m_data->renderer->heightPixels();

        if (x + width < 0 || y + height < 0 || 
                x > targetWidth || y > targetHeight)
            return;

        QRect rect(x, y, width, height);
        QPen pen(*color);
        QPainter *painter = m_data->renderer->painter();
        painter->save();
        painter->setPen(pen);
        painter->drawRect(rect);
        painter->restore();
    }
    void GraphQtInteractiveRenderer::fillPixelRect(int x, int y, 
            int width, int height, Color *color)
    {
        if (!m_data->renderer)
            return;

        int targetWidth = m_data->renderer->widthPixels();
        int targetHeight = m_data->renderer->heightPixels();

        if (x + width < 0 || y + height < 0 || 
                x > targetWidth || y > targetHeight)
            return;

        QPainter *painter = m_data->renderer->painter();
        painter->save();
        painter->fillRect(x, y, width, height, *color);
        painter->restore();
    }

    void GraphQtInteractiveRenderer::copyToWindow(void *window, int x, int y, int width, int height)
    {
        QPaintDevice *device = static_cast<QPaintDevice *>(window);
        if (device)
            copyToWindow(device, x, y, width, height);
    }
}
