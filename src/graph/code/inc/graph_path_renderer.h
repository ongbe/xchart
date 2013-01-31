
#ifndef _GRAPH_PATH_RENDERER_H_
#define _GRAPH_PATH_RENDERER_H_

#include "graph_renderer.h"


class QPainter;
class QImage;

namespace GraphLib 
{
    class GraphTextLine;
    class GraphText;
    class IGraphInteractiveRenderer;

    /*!
     * \brief 
     * - GraphPathRenderer 类， 渲染图形的工具类。
     *
     */
    class GraphPathRenderer : public GraphRenderer
    {
        public:
            GraphPathRenderer();
            virtual ~GraphPathRenderer();

        public:
            int widthPixels();
            int heightPixels();
            void setWidthPixels(int w);
            void setHeightPixels(int h);

            virtual void beginRender();
            virtual void endRender();

            void setLineWidth(float width);
            void setLineCaps(LineCaps mode);
            void setLineJoin(LineJoin mode);
            void setLineStyle(LineStyle mode);
            void setDashLength(float length);
            void setFillStyle(FillStyle mode);

            void drawLine(Point *start, Point *end, Color *color);
            void drawRect(Point *ulCorner, Point *lrCorner, 
                    Color *color);
            void fillRect(Point *ulCorner, Point *lrCorner, 
                    Color *color);

            void drawEllipse(Point *center, 
                    float width, float height, 
                    Color *color);
            void fillEllipse(Point *center, 
                    float width, float height, 
                    Color *color);

            void drawPolyline(Point *points, int pointCount,
                    Color *color);

            void drawPolygon(Point *points, int pointCount,
                    Color *color);
            void fillPolygon(Point *points, int pointCount,
                    Color *color);

            void drawString(const QString &text, Point *pos,
                    int alignment, 
                    Color *color);

            void drawImage(Point *point, float width, float height,
                    GraphImage *image);

            GraphFont *font();
            void setFont(GraphFont *);
            void drawText(GraphText *text);
            void drawTextLine(GraphTextLine *line, 
                    Point *pos, Alignment alignment, Color *color);

        public:
            void render(QImage *image, QRectF clip, QTransform &);
            IGraphInteractiveRenderer *interactiveRenderer();
            void setInteractiveRenderer(IGraphInteractiveRenderer *);

        protected:
            void setClipRegion(QRegion *);
            QRegion *clipRegion();

            void copyToWindow(QPainter *,
                    int, int, int, int);

            struct shape;
            void addShape(struct shape *);

            int needRedraw(QRectF tileRect);

        protected:
            struct PrivateData;
            PrivateData *m_data;

            friend class GraphPathInteractiveRenderer;
    };

}
#endif
