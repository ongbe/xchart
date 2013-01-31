
#ifndef _GRAPH_AGG_RENDERER_H_
#define _GRAPH_AGG_RENDERER_H_

#include "graph_renderer.h"
#include "agg_path_attributes.h"


class QPainter;
class QImage;
namespace GraphLib
{
    class GraphFont;
    class GraphAggRenderer : public GraphRenderer
    {
        public:
            GraphAggRenderer();
            ~GraphAggRenderer();

            IGraphInteractiveRenderer *interactiveRenderer();
            void setInteractiveRenderer(IGraphInteractiveRenderer *);

            void beginRender();
            void endRender(); 

            void copyToWindow(QPainter *d, int x, int y, int w, int h);
            //void render(QImage *image);
            void render(QImage *image, QRectF clip, agg::trans_affine *);

        public:
            GraphFont *font();
            void setFont(GraphFont *);
            void setClipRegion(QRegion *region);
            QRegion *clipRegion();

            int widthPixels();
            void setWidthPixels(int w);

            int heightPixels();
            void setHeightPixels(int h);

        public:
            virtual void drawLine(Point *start, Point *end, 
                    Color *color);

            virtual void drawRect(Point *ulCorner, Point *lrCorner, 
                    Color *color);

            virtual void fillRect(Point *ulCorner, Point *lrCorner, 
                    Color *color);

            virtual void drawEllipse(Point *center, 
                    float width, float height, 
                    Color *color);

            virtual void fillEllipse(Point *center, 
                    float width, float height, 
                    Color *color);

            virtual void drawPolyline(Point *points, int pointCount,
                    Color *color);

            virtual void drawPolygon(Point *points, int pointCount,
                    Color *color);

            virtual void fillPolygon(Point *points, int pointCount,
                    Color *color);

            virtual void drawString(const QString &text, 
                    Point *pos,
                    int alignment, 
                    Color *color);

        public:
            virtual void beginPath();
            virtual void endPath();

            virtual void closePath();
            virtual void moveTo(double x, double y);
            virtual void lineTo(double x, double y);

        public:
            virtual void setLineWidth(float width);

        private:
            void pop_attr();
            void push_attr();
            path_attributes& cur_attr();

            void updateBbox();
        private:
            int needRedraw(QRectF tileRect);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif
