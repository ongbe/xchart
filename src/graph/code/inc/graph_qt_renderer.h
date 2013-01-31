
#ifndef _GRAPH_QT_RENDERER_H_
#define _GRAPH_QT_RENDERER_H_

#include "graph_renderer.h"


class QPainter;
class QImage;

namespace GraphLib 
{
    class GraphTextLine;
    class GraphText;
    class GraphQtInteractiveRenderer;
    /*!
     * \brief 
     * - GraphQtRenderer 类， 渲染图形的工具类。
     *
     */
    class GraphQtRenderer : public GraphRenderer
    {
        public:
            GraphQtRenderer();
            virtual ~GraphQtRenderer();

        public:
            int widthPixels();
            int heightPixels();


            void beginRender();
            void endRender();

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

            void fillPolygon(Point *points,
                             int pointCount,
                             QPixmap pixmap);

            void drawString(const QString &text, Point *pos,
                    int alignment, 
                    Color *color);

            void drawImage(Point *point, float width, float height,
                    GraphImage *image);

            void drawImage(Rectangle *targetRect,
                                   GraphImage *image,
                                   Rectangle *sourceRect);

            GraphFont *font();
            void setFont(GraphFont *);
            void drawText(GraphText *text);
            void drawTextLine(GraphTextLine *line, 
                    Point *pos, Alignment alignment, Color *color);

            void drawArc(Point *point,
                         float width,
                         float height,
                         float startAngle,
                         float spanAngle,
                         Color *color);


            void drawPie(Point *point,
                         float width,
                         float height,
                         float startAngle,
                         float spanAngle,
                         Color *color);

            void fillPie(Point *point,
                         float width,
                         float height,
                         float startAngle,
                         float spanAngle,
                         Color *color);

        protected:
            void setClipRegion(QRegion *);
            QRegion *clipRegion();
            void setPixmap(QImage *pixmap);
            QImage *pixmap();
            QPainter *painter();


            IGraphInteractiveRenderer *interactiveRenderer();
            void setInteractiveRenderer(IGraphInteractiveRenderer *);
        protected:
            struct PrivateData;
            PrivateData *m_data;

            friend class GraphQtInteractiveRenderer;
    };

}
#endif
