
#include "graph_qt_renderer.h"
#include "graph_font.h"
#include "graph_font.h"
#include "graph_text.h"
#include "graph_text_line.h"

#include <QPainter>
#include <QPaintDevice>
#include <QPixmap>
#include <QImage>
#include <QDebug>

namespace GraphLib 
{
    struct GraphQtRenderer::PrivateData
    {
        QImage *pixmap;
        int width;
        int height;
        QRegion *clipRegion;

        GraphFont *font;
        float fontHeight;
        QPainter *painter;
        bool autoDelete;

        QPen *pen;
        double penLineWidth;
        Qt::PenStyle penLineStyle;
        QColor penLineColor;

        QBrush *brush;

        float lineWidth;
        int dashLength;
        int dotLength;

        Color *hightlightColor;

        IGraphInteractiveRenderer *interactiveRenderer;
    };

    GraphQtRenderer::GraphQtRenderer() :
        GraphRenderer()
    {
        m_data = new PrivateData;
        m_data->painter = NULL;
        m_data->pixmap = NULL;
        m_data->clipRegion = NULL;
        m_data->autoDelete = true;

        m_data->width = 0;
        m_data->height = 0;
        m_data->pen = new QPen();
        m_data->penLineWidth = m_data->pen->widthF();
        m_data->penLineStyle = m_data->pen->style();
        m_data->penLineColor = m_data->pen->color();

        m_data->brush = new QBrush();
        m_data->font = new GraphFont;
        m_data->fontHeight = 3.0;

        m_data->lineWidth = 1.0;
        m_data->dashLength = 10;
        m_data->dotLength = 2;

        m_data->interactiveRenderer = NULL;
    }

    GraphQtRenderer::~GraphQtRenderer()
    {
        delete m_data->font;

        if (m_data->painter)
            delete m_data->painter;
        if (m_data->autoDelete && m_data->pixmap)
            delete m_data->pixmap;
        if (m_data->clipRegion)
            delete m_data->clipRegion;

        delete m_data;
    }

    int GraphQtRenderer::widthPixels()
    {
        return m_data->width;
    }
    int GraphQtRenderer::heightPixels()
    {
        return m_data->height;
    }

    void GraphQtRenderer::beginRender()
    {
        m_data->penLineWidth = m_data->pen->widthF();
        m_data->penLineStyle = m_data->pen->style();
        m_data->penLineColor = m_data->pen->color();
    }

    void GraphQtRenderer::endRender()
    {
        m_data->pen->setWidthF(m_data->penLineWidth);
        m_data->pen->setStyle(m_data->penLineStyle);
        m_data->pen->setColor(m_data->penLineColor);
    }

    void GraphQtRenderer::setLineWidth(float width)
    {
        m_data->pen->setWidthF(width);
    }
    void GraphQtRenderer::setLineCaps(LineCaps mode)
    {
        m_data->pen->setCapStyle(mode);
    }
    void GraphQtRenderer::setLineJoin(LineJoin mode)
    {
        m_data->pen->setJoinStyle(mode);
    }
    void GraphQtRenderer::setLineStyle(LineStyle mode)
    {
        m_data->pen->setStyle(mode);
    }
    void GraphQtRenderer::setDashLength(float length)
    {
        m_data->pen->setDashOffset(length);
    }
    void GraphQtRenderer::setFillStyle(FillStyle mode)
    {
        m_data->brush->setStyle(mode);
    }


    void GraphQtRenderer::drawLine(Point *start, Point *end, Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();


        if (start->x() != end->x() && start->y() != end->y()) {
            if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
                m_data->painter->setRenderHint(QPainter::Antialiasing, true);
            }
        }

        QPen pen(*m_data->pen);
        pen.setColor(*color);
        m_data->painter->setBrush(Qt::NoBrush);
        m_data->painter->setPen(pen);

        m_data->painter->drawLine(*start, *end);

        m_data->painter->restore();
    }
        
    void GraphQtRenderer::drawRect(Point *ulCorner, Point *lrCorner, 
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        QPen pen(*m_data->pen);
        pen.setColor(*color);
        m_data->painter->setPen(pen);
        int x = ulCorner->x();
        int y = ulCorner->y();
        int width = lrCorner->x() - ulCorner->x();
        int height = lrCorner->y() - ulCorner->y();
        m_data->painter->drawRect(x, y, width, height);

        m_data->painter->restore();
    }

    void GraphQtRenderer::fillRect(Point *ulCorner, Point *lrCorner, 
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        QBrush brush(*m_data->brush);
        brush.setColor(*color);
        m_data->painter->setBrush(brush);

        int x = ulCorner->x();
        int y = ulCorner->y();
        int width = lrCorner->x() - ulCorner->x();
        int height = lrCorner->y() - ulCorner->y();
        m_data->painter->fillRect(x, y, width, height, *color);

        m_data->painter->restore();
    }

    void GraphQtRenderer::drawEllipse(Point *center, 
            float width, float height, 
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
            m_data->painter->setRenderHint(QPainter::Antialiasing, true);
        }

        QPen pen(*m_data->pen);
        pen.setColor(*color);
        m_data->painter->setPen(pen);
        m_data->painter->drawEllipse(center->x(),
                center->y(), width, height);

        m_data->painter->restore();

    }
    void GraphQtRenderer::fillEllipse(Point *center, 
            float width, float height, 
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
            m_data->painter->setRenderHint(QPainter::Antialiasing, true);
        }

        m_data->painter->setPen(Qt::NoPen);
        m_data->painter->setBrush(*color);

        m_data->painter->drawEllipse(center->x(),
                center->y(), width, height);

        m_data->painter->restore();
    }

    void GraphQtRenderer::drawPolyline(Point *points, int pointCount,
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        QPainter::RenderHints hints = m_data->painter->renderHints();
        if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
            m_data->painter->setRenderHint(QPainter::Antialiasing, true);
        }

        QPen pen(*m_data->pen);
        pen.setColor(*color);
        m_data->painter->setPen(pen);
        m_data->painter->drawPolyline(points, pointCount);

        hints = m_data->painter->renderHints();

        m_data->painter->restore();
    }

    void GraphQtRenderer::drawPolygon(Point *points, int pointCount,
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
            m_data->painter->setRenderHint(QPainter::Antialiasing, true);
        }

        QPen pen(*m_data->pen);
        pen.setColor(*color);
        m_data->painter->setPen(pen);
        
        m_data->painter->drawPolygon(points, pointCount);

        m_data->painter->restore();

    }
    void GraphQtRenderer::fillPolygon(Point *points, int pointCount,
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
            m_data->painter->setRenderHint(QPainter::Antialiasing, true);
        }

        m_data->painter->setPen(Qt::NoPen);
        m_data->painter->setBrush(*color);

        m_data->painter->drawPolygon(points, pointCount);

        m_data->painter->restore();

    }

    void GraphQtRenderer::fillPolygon(Point *points,
                                      int pointCount,
                                      QPixmap pixmap)
    {
        if (!m_data->painter)
            return;

        QPainterPath path;
        for (int i = 0; i < pointCount; i++) {
            Point p = *(points + i);

            if (i == 0)
                path.moveTo(p.x(), p.y());
            else
                path.lineTo(p.x(), p.y());
        }

        QRectF rect = path.boundingRect();

        m_data->painter->save();

        m_data->painter->setClipPath(path);

        if (!m_data->painter->testRenderHint(QPainter::Antialiasing)) {
            m_data->painter->setRenderHint(QPainter::Antialiasing, true);
        }

        m_data->painter->setPen(Qt::NoPen);

        m_data->painter->drawTiledPixmap(rect, pixmap);

        m_data->painter->restore();

    }

    void GraphQtRenderer::drawString(const QString &text, Point *pos,
            int, 
            Color *color)
    {
        if (!m_data->painter)
            return;

        QString str = text;
        GraphTextLine line(&str, m_data->font, m_data->fontHeight);
        drawTextLine(&line, pos, 0, color);
    }
    void GraphQtRenderer::drawImage(Point *point, float width, float height,
            GraphImage *image)
    {
        if (!m_data->painter)
            return;
        QRectF rect(point->x(), point->y(), width, height);
        m_data->painter->drawImage(rect, *image);
    }

    void GraphQtRenderer::drawImage(Rectangle *targetRect,
                                    GraphImage *image,
                                    Rectangle *sourceRect)
    {
        if (!m_data->painter)
            return;

        m_data->painter->drawImage(*targetRect, *image, *sourceRect);
    }

    GraphFont *GraphQtRenderer::font()
    {
        return m_data->font;
    }

    void GraphQtRenderer::setFont(GraphFont *font)
    {
        m_data->font = font;
    }

    QPainter *GraphQtRenderer::painter() 
    {
        return m_data->painter;
    }
    QImage *GraphQtRenderer::pixmap()
    {
        return m_data->pixmap;
    }
    void GraphQtRenderer::setPixmap(QImage *pixmap)
    {
        if (m_data->painter)
            delete m_data->painter;
        if (m_data->autoDelete && m_data->pixmap) {
            delete m_data->pixmap;
        }
        m_data->pixmap = pixmap;
        m_data->painter = new QPainter(m_data->pixmap);
        m_data->width = pixmap->width();
        m_data->height = pixmap->height();
    }
    void GraphQtRenderer::setClipRegion(QRegion *region)
    {
        if (m_data->clipRegion)
            delete m_data->clipRegion;
        m_data->clipRegion = region; 
    }

    QRegion *GraphQtRenderer::clipRegion()
    {
        if (!m_data->clipRegion)
            m_data->clipRegion = new QRegion;
        return m_data->clipRegion;
    }

    void GraphQtRenderer::drawText(GraphText *text)
    {
        if (text == NULL)
            return;
        text->draw(this);
    }
    void GraphQtRenderer::drawTextLine(GraphTextLine *line, 
            Point *pos, Alignment alignment, Color *color)
    {
        if (line == NULL || pos == NULL || color == NULL)
            return;

        QString text = *line->string();
        if (text.size() == 0)
            return;

        m_data->painter->save();

        m_data->painter->setPen(*color);

        GraphFont *font = line->font();
        if (font)
            m_data->painter->setFont(*font);

        m_data->painter->drawText(*pos, text);

        m_data->painter->restore();
    }
    void GraphQtRenderer::drawArc(Point *point,
            float width,
            float height,
            float startAngle,
            float spanAngle,
            Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        m_data->painter->setRenderHint(QPainter::Antialiasing);

        QPen pen;
        pen.setColor(*color);
        pen.setWidthF(m_data->pen->widthF());
        pen.setStyle(m_data->pen->style());
        m_data->painter->setPen(pen);

        m_data->painter->setBrush(Qt::NoBrush);

        m_data->painter->drawArc( QRectF(point->x(), point->y(), width, height),
                (int)(startAngle*16),
                (int)(spanAngle*16));

        m_data->painter->restore();

    }


    void GraphQtRenderer::drawPie(Point *point,
                                  float width,
                                  float height,
                                  float startAngle,
                                  float spanAngle,
                                  Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        m_data->painter->setRenderHint(QPainter::Antialiasing);

        QPen pen;
        pen.setColor(*color);
        pen.setWidthF(m_data->pen->widthF());
        pen.setStyle(m_data->pen->style());
        m_data->painter->setPen(pen);

        m_data->painter->setBrush(Qt::NoBrush);

        m_data->painter->drawPie( QRectF(point->x(), point->y(), width, height),
                                  (int)(startAngle*16),
                                  (int)(spanAngle*16));

        m_data->painter->restore();
    }

    void GraphQtRenderer::fillPie(Point *point,
                                  float width,
                                  float height,
                                  float startAngle,
                                  float spanAngle,
                                  Color *color)
    {
        if (!m_data->painter)
            return;

        m_data->painter->save();

        m_data->painter->setRenderHint(QPainter::Antialiasing);

        m_data->painter->setPen(Qt::NoPen);
        m_data->painter->setBrush(*color);

        m_data->painter->drawPie( QRectF(point->x(), point->y(), width, height),
                                  (int)(startAngle*16),
                                  (int)(spanAngle*16));

        m_data->painter->restore();
    }

    IGraphInteractiveRenderer *GraphQtRenderer::interactiveRenderer()
    {
        return m_data->interactiveRenderer;
    }
    void GraphQtRenderer::setInteractiveRenderer(IGraphInteractiveRenderer *r)
    {
        m_data->interactiveRenderer = r;
    }
}
