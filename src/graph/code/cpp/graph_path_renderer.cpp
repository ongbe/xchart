
#include "graph_path_renderer.h"
#include "graph_font.h"
#include "graph_text.h"
#include "graph_text_line.h"
#include "igraph_interactive_renderer.h"

#include "tile_types.h"
#include "tile_manager.h"
#include "tile.h"

#include <QPainter>
#include <QVector>
#include <QPainterPath>
#include <QPaintDevice>
#include <QPixmap>
#include <QImage>
#include <QTime>
#include <QDebug>

namespace GraphLib 
{
    struct GraphPathRenderer::shape
    {
        shape() : type(PATH)
        {
        }
        enum Type {IMAGE, TEXT, PATH};
        int type;
        QPen pen;
        QBrush brush;
        QPainterPath path;
    };

    static QRectF calculate_extent(QRectF r)
    {
        // 确保image的大小，从左上角的(0,0)位置位起点。
        return r.united(QRectF(0, 0, 1, 1));
        // 测试的范围
        //return QRectF(0, 0, 1000, 10000);
    }

    struct GraphPathRenderer::PrivateData
    {
        IGraphInteractiveRenderer *interactiveRenderer;

        TileManager *tiles; 
        unsigned char *redrawFlags;
        QRectF bbox;
        QVector<struct GraphPathRenderer::shape> paths;

        int width;
        int height;
        QRegion *clipRegion;

        GraphFont *font;
        float fontHeight;
        QPen *pen;
        QBrush *brush;
        float lineWidth;
        int dashLength;
        int dotLength;
    };

    GraphPathRenderer::GraphPathRenderer() :
        GraphRenderer()
    {
        m_data = new PrivateData;
        m_data->tiles = 0;
        m_data->bbox = QRectF(0, 0, 0, 0);
        m_data->redrawFlags = 0;
        m_data->clipRegion = NULL;
        m_data->width = 0;
        m_data->height = 0;

        m_data->pen = new QPen();
        m_data->brush = new QBrush();
        m_data->font = new GraphFont;
        m_data->fontHeight = 3.0;

        m_data->lineWidth = 1.0;
        m_data->dashLength = 10;
        m_data->dotLength = 2;

        m_data->interactiveRenderer = 0;
    }

    GraphPathRenderer::~GraphPathRenderer()
    {
        delete m_data->font;

        if (m_data->clipRegion)
            delete m_data->clipRegion;

        delete m_data;
    }

    int GraphPathRenderer::widthPixels()
    {
        return m_data->width;
    }
    int GraphPathRenderer::heightPixels()
    {
        return m_data->height;
    }
    void GraphPathRenderer::setWidthPixels(int w)
    {
        m_data->width = w;
    }
    void GraphPathRenderer::setHeightPixels(int h)
    {
        m_data->height = h;
    }


    void GraphPathRenderer::beginRender()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        m_data->paths.clear();
    }

    void GraphPathRenderer::endRender()
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);
        QRectF extent = calculate_extent(m_data->bbox);

        // 范围改变，重新分配tile 
        if (!m_data->tiles || m_data->tiles->width < extent.width() ||
                m_data->tiles->height < extent.height()) {
            if (m_data->tiles)
                delete m_data->tiles;
            m_data->tiles = new TileManager(extent.width(), 
                    extent.height(), 4);

            TileManager *tm = m_data->tiles;
            int ntiles = tm->ntile_rows * tm->ntile_cols;
            if (m_data->redrawFlags) {
                delete []m_data->redrawFlags;
            }
            m_data->redrawFlags = new unsigned char[ntiles];
            memset(m_data->redrawFlags, 1, ntiles);

            //qDebug() << "new tiles manager: " << extent;
            //qDebug() << "GRAPH DATA extent: " << extent;
        }

        //qDebug() << "path count: " << m_data->paths.size();

        if (m_data->clipRegion) {
            TileManager *tm = m_data->tiles;
            int ntiles = tm->ntile_rows * tm->ntile_cols;
            QRectF bbox = m_data->clipRegion->boundingRect();
            //qDebug() << "clip box" << bbox;
            int x = bbox.x(); 
            int y = bbox.y(); 
            int w = bbox.width();
            int h = bbox.height();
            int i, j;
            for (i = y; i < (y + h); i += (TILE_HEIGHT - (i % TILE_HEIGHT)))  {
                for (j = x; j < (x + w); j += (TILE_WIDTH - (j % TILE_WIDTH))) {
                    int index = tm->get_tile_num(j, i);
                    if (-1 != index) {
                        m_data->redrawFlags[index] = 1;
                    }
                    //qDebug() << "----" << index;
                }
            }
        }
    }

    void GraphPathRenderer::setLineWidth(float width)
    {
        m_data->pen->setWidthF(width);
    }
    void GraphPathRenderer::setLineCaps(LineCaps mode)
    {
        m_data->pen->setCapStyle(mode);
    }
    void GraphPathRenderer::setLineJoin(LineJoin mode)
    {
        m_data->pen->setJoinStyle(mode);
    }
    void GraphPathRenderer::setLineStyle(LineStyle mode)
    {
        m_data->pen->setStyle(mode);
    }
    void GraphPathRenderer::setDashLength(float length)
    {
        m_data->pen->setDashOffset(length);
    }
    void GraphPathRenderer::setFillStyle(FillStyle mode)
    {
        //qDebug() << "setFillStyle";
        m_data->brush->setStyle(mode);
    }

    void GraphPathRenderer::drawLine(Point *start, Point *end, Color *color)
    {
        struct shape item;
        QPainterPath &path = item.path; 

        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPen &pen = item.pen;
        pen.setColor(*color);

        QBrush &brush = item.brush;
        brush.setStyle(Qt::NoBrush);

        path.moveTo(*start);
        path.lineTo(*end);
        addShape(&item);
    }

    void GraphPathRenderer::drawRect(Point *ulCorner, Point *lrCorner, 
            Color *color)
    {
        struct shape item;
        QPainterPath &path = item.path; 

        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPen &pen = item.pen;
        pen.setColor(*color);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::NoBrush);

        qreal x = ulCorner->x();
        qreal y = ulCorner->y();
        qreal width = lrCorner->x() - ulCorner->x();
        qreal height = lrCorner->y() - ulCorner->y();

        path.addRect(QRectF(x, y, width, height));

        addShape(&item);
    }

    void GraphPathRenderer::fillRect(Point *ulCorner, Point *lrCorner, 
            Color *color)
    {
        struct shape item;
        QPainterPath &path = item.path; 

        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPen &pen = item.pen;
        pen.setStyle(Qt::NoPen);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(*color);

        qreal x = ulCorner->x();
        qreal y = ulCorner->y();
        qreal width = lrCorner->x() - ulCorner->x();
        qreal height = lrCorner->y() - ulCorner->y();

        path.addRect(QRectF(x, y, width, height));

        addShape(&item);
    }

    void GraphPathRenderer::drawEllipse(Point *center, 
            float width, float height, 
            Color *color)
    {
        struct shape item;
        QPainterPath &path = item.path; 

        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPen &pen = item.pen;
        pen.setStyle(Qt::SolidLine);
        pen.setColor(*color);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::NoBrush);

        QRectF bbox = QRectF(0.0, 0.0, width, height);
        bbox.moveCenter(*center);
        path.addEllipse(bbox);

        addShape(&item);
    }
    void GraphPathRenderer::fillEllipse(Point *center, 
            float width, float height, 
            Color *color)
    {
        struct shape item;
        QPainterPath &path = item.path; 

        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPen &pen = item.pen;
        pen.setStyle(Qt::NoPen);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(*color);

        QRectF bbox = QRectF(0.0, 0.0, width, height);
        bbox.moveCenter(*center);
        path.addEllipse(bbox);

        addShape(&item);
    }

    void GraphPathRenderer::drawPolyline(Point *points, int pointCount,
            Color *color)
    {

        struct shape item;
        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPainterPath &path = item.path; 

        QPen &pen = item.pen;
        pen.setStyle(Qt::SolidLine);
        pen.setColor(*color);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::NoBrush);

        path.moveTo(points[0]);
        for (int i = 1; i < pointCount; ++i) {
            path.lineTo(points[i]);
        }

        addShape(&item);
    }

    void GraphPathRenderer::drawPolygon(Point *points, int pointCount,
            Color *color)
    {
        struct shape item;
        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPainterPath &path = item.path; 
        QPen &pen = item.pen;
        pen.setStyle(Qt::SolidLine);
        pen.setColor(*color);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::NoBrush);

        path.moveTo(points[0]);
        for (int i = 1; i < pointCount; ++i) {
            path.lineTo(points[i]);
        }
        path.lineTo(points[0]);

        addShape(&item);
    }
    void GraphPathRenderer::fillPolygon(Point *points, int pointCount,
            Color *color)
    {
        struct shape item;
        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        QPainterPath &path = item.path; 
        QPen &pen = item.pen;
        pen.setStyle(Qt::NoPen);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(*color);

        QPolygonF gonf;
        for (int i = 0; i < pointCount; ++i)
            gonf << points[i];
        path.addPolygon(gonf);

        addShape(&item);
    }

    void GraphPathRenderer::drawString(const QString &text, Point *pos,
            int, 
            Color *color)
    {
        struct shape item;
        item.pen = *m_data->pen;
        item.brush = *m_data->brush;

        item.type = shape::TEXT;
        QPainterPath &path = item.path; 
        QPen &pen = item.pen;
        pen.setStyle(Qt::NoPen);
        QBrush &brush = item.brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(*color);

        QFont myFont;
        path.addText(*pos, myFont, text);

        addShape(&item);
    }
    void GraphPathRenderer::drawImage(Point *point, float width, float height,
            GraphImage *image)
    {
        /*
           QRectF rect(point->x(), point->y(), width, height);
           */
    }

    GraphFont *GraphPathRenderer::font()
    {
        return m_data->font;
    }

    void GraphPathRenderer::setFont(GraphFont *)
    {
        //qDebug() << "WARING not implement"; 
    }

    void GraphPathRenderer::setClipRegion(QRegion *region)
    {
        //qDebug() << "setClipRegion";
        if (m_data->clipRegion)
            delete m_data->clipRegion;
        m_data->clipRegion = region; 
    }

    QRegion *GraphPathRenderer::clipRegion()
    {
        if (!m_data->clipRegion)
            m_data->clipRegion = new QRegion;
        return m_data->clipRegion;
    }

    void GraphPathRenderer::drawText(GraphText *text)
    {
        if (text == NULL)
            return;
        text->draw(this);
    }
    void GraphPathRenderer::drawTextLine(GraphTextLine *line, 
            Point *pos, Alignment alignment, Color *color)
    {
        /*
           if (line == NULL || pos == NULL || color == NULL)
           return;

           QString text = *line->string();
           if (text.size() == 0)
           return;

           m_data->painter->save();

           m_data->painter->setPen(*color);
           */
        /*
           QFont *font = line->font();
           if (font)
           m_data->painter->setFont(*font);
           */
        /*
           m_data->painter->drawText(*pos, text);

           m_data->painter->restore();
           */
    }

    void GraphPathRenderer::copyToWindow(QPainter *d,
            int x, int y, int w, int h)
    {
        if (NULL == d || m_data->tiles == 0) {
            return;
        }
        //qDebug() << x << "," << y << "," << w << "," << h;
        QTime time;
        time.start();

        GraphLib::TileManager *tm = m_data->tiles;

        QTransform t;
        t.translate(-1 * x, -1 * y);

        QPainter &painter = *d;
        painter.setTransform(t);

        int i, j;
        for (i = y; i < (y + h); i += (TILE_HEIGHT - (i % TILE_HEIGHT)))  {
            for (j = x; j < (x + w); j += (TILE_WIDTH - (j % TILE_WIDTH))) {
                GraphLib::Tile *t = tm->get_tile(j, i, true, false);
                if (t == NULL)
                    continue;
                Tile::release(t, false);

                if (1 == needRedraw(QRectF(j, i, 1, 1))) {
                    t = tm->get_tile(j, i, true, true);

                    int tile_x, tile_y;
                    tm->get_tile_coordinates(t, &tile_x, &tile_y);
                    QImage image(t->data, t->ewidth, t->eheight, 
                            QImage::Format_ARGB32_Premultiplied);

                    memset(t->data, 0, t->size);
                    QTransform t1;
                    QRectF extent = calculate_extent(m_data->bbox);

                    t1.translate(-1*extent.center().x(), -1*extent.center().y());
                    t1.translate(tm->width/2, tm->height/2);
                    t1.translate(-1 * tile_x, -1 * tile_y);
                    render(&image, QRectF(), t1);
                    Tile::release(t, true);

                    int index = tm->get_tile_num(tile_x, tile_y);
                    m_data->redrawFlags[index] = 0;
                    //qDebug() << "copy to window : " << index;
                }

                t = tm->get_tile(j, i, true, false);
                int tile_x, tile_y;
                tm->get_tile_coordinates(t, &tile_x, &tile_y);

                QImage image(t->data, t->ewidth, t->eheight, 
                        QImage::Format_ARGB32_Premultiplied);

                int w = t->ewidth;
                int h = t->eheight;

                QRectF targetRect(tile_x, tile_y, w, h);
                QRectF sourceRect(0, 0, w, h);

                painter.drawImage(targetRect, image, sourceRect);
                Tile::release(t, false);


#ifdef TILE_PROFILING
                // 显示tile
                painter.setPen(Qt::red);
                int index = tm->get_tile_num(tile_x, tile_y);
                //image.save(QString("%1.png").arg(index));
                //qDebug() << "++++" << index;
                painter.drawText(QRectF(tile_x, tile_y, w, h),
                        Qt::AlignCenter, QString("%1").arg(index));
                painter.drawRect(QRectF(tile_x, tile_y, w, h));
#endif
            } // j循环
        } // i循环
        //qDebug() << "Time elapsed: " << time.elapsed() << " md.";
    }

    void GraphPathRenderer::render(QImage *image, QRectF clip, 
            QTransform &t)
    {
        //qDebug() << QString("%1").arg(__PRETTY_FUNCTION__);

        QPainter p(image);
        p.setTransform(t);
        //p.setClipRect(QRect(0, 0, image->width(), image->height())); //显示的图形会有空白的clip 矩形

        QPainter *d = &p;

        //d->setRenderHint(QPainter::Antialiasing, true);
        //d->setRenderHint(QPainter::TextAntialiasing, true);

        QVector<struct GraphPathRenderer::shape>::const_iterator it;
        for (it = m_data->paths.begin(); it != m_data->paths.end(); 
                ++it) {
            d->save();
            const struct GraphPathRenderer::shape &item = *it;
            d->setPen(item.pen);
            d->setBrush(item.brush);
            d->drawPath(item.path);
            d->restore();
        }
    }

    void GraphPathRenderer::addShape(struct shape *item)
    {
        QRectF rect = item->path.boundingRect();
        m_data->bbox = m_data->bbox.united(rect);
        m_data->paths.append(*item);
    }

    IGraphInteractiveRenderer *GraphPathRenderer::interactiveRenderer()
    {
        return m_data->interactiveRenderer;
    }
    void GraphPathRenderer::setInteractiveRenderer(IGraphInteractiveRenderer *r)
    {
        m_data->interactiveRenderer = r;
    }

    int GraphPathRenderer::needRedraw(QRectF tileRect)
    {
        TileManager *tm = m_data->tiles;
        int index = tm->get_tile_num(tileRect.x(), tileRect.y());
        return m_data->redrawFlags[index];
    }
}
