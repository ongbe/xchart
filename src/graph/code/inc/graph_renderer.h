
#ifndef _GRAPH_RENDERER_H_
#define _GRAPH_RENDERER_H_

#include "i_canvas_path_methods.h"
#include "graph_color.h"
#include "graph_image.h"
#include "graph_font.h"
#include "graph_geometry.h"

namespace GraphLib 
{
    class Object;
    class GraphTextLine;
    class GraphText;
    class IGraphInteractiveRenderer;
    /*!
     * \brief 
     * - GraphRenderer 类， 渲染图形的工具类。
     *
     */
    class GraphRenderer  : public ICanvasPathMethods
    {
        public:
            virtual ~GraphRenderer() {}

        public:
            virtual int widthPixels();
            virtual int heightPixels();

            virtual void beginRender() = 0;
            virtual void endRender() = 0; 

            /**
             * 设置line的属性，样式
             * */
            virtual void setLineWidth(float width);
            virtual void setLineCaps(LineCaps mode);
            virtual void setLineJoin(LineJoin mode);
            virtual void setLineStyle(LineStyle mode);
            virtual void setDashLength(float length);
            virtual void setFillStyle(FillStyle mode);

            virtual void drawObject(Object *);

            /*! Draw a line from start to end, using color and the current line style */
            virtual void drawLine(Point *start, Point *end, 
                    Color *color);

            /*! Draw a rectangle, given its upper-left and lower-right corners */
            virtual void drawRect(Point *ulCorner, Point *lrCorner, 
                    Color *color);
            /*! Fill a rectangle, given its upper-left and lower-right corners */
            virtual void fillRect(Point *ulCorner, Point *lrCorner, 
                    Color *color);

            /*! Draw an ellipse, given its center and the bounding box */
            virtual void drawEllipse(Point *center, 
                    float width, float height, 
                    Color *color);
            /*! Same a DrawEllipse, except the ellips is filled */
            virtual void fillEllipse(Point *center, 
                    float width, float height, 
                    Color *color);

            /*! Draw a line joining multiple points, using color and the current
              line style */
            virtual void drawPolyline(Point *points, int pointCount,
                    Color *color);

            /*! Draw a polygone, using the current line style
              The polygon is closed even if the first point is not the same as the
              last point */
            virtual void drawPolygon(Point *points, int pointCount,
                    Color *color);
            /*! the polygon is filled using the current fill type, no border is drawn */
            virtual void fillPolygon(Point *points, int pointCount,
                    Color *color);

            virtual void fillPolygon(Point *points,
                                     int pointCount,
                                     QPixmap pixmap);

            /*! Print a string at pos, using the current font 
             * 默认的显示字符串的位置是在字符的左下位置开始画的
             * */
            virtual void drawString(const QString &text, 
                    Point *pos,
                    int alignment, 
                    Color *color);
            /*! Draw an image, given its bounding box */
            virtual void drawImage(Point *point,
                    float width,
                    float height,
                    GraphImage *image);

            virtual void drawImage(Rectangle *targetRect,
                                   GraphImage *image,
                                   Rectangle *sourceRect);

            virtual GraphFont *font();
            virtual void setFont(GraphFont *);

            /*! Print a Text.  It holds its own information. */
            virtual void drawText(GraphText *text) 
            {
                Q_UNUSED(text)
            }
            /*! Print a TextLine.  It holds its own font/size information. */
            virtual void drawTextLine(GraphTextLine *line, 
                    Point *pos, 
                    Alignment alignment, 
                    Color *color) 
            {
                Q_UNUSED(line)
                Q_UNUSED(pos)
                Q_UNUSED(alignment)
                Q_UNUSED(color)
            }

            virtual void drawPie(Point *point,
                                 float width,
                                 float height,
                                 float startAngle,
                                 float spanAngle,
                                 Color *color) 
            {
                Q_UNUSED(point);
                Q_UNUSED(width);
                Q_UNUSED(height);
                Q_UNUSED(startAngle);
                Q_UNUSED(spanAngle);
                Q_UNUSED(color);
            }

            virtual void drawArc(Point *point,
                                 float width,
                                 float height,
                                 float startAngle,
                                 float spanAngle,
                                 Color *color) 
            {
                Q_UNUSED(point);
                Q_UNUSED(width);
                Q_UNUSED(height);
                Q_UNUSED(startAngle);
                Q_UNUSED(spanAngle);
                Q_UNUSED(color);
            }




            virtual void fillPie(Point *point,
                    float width,
                    float height,
                    float startAngle,
                    float spanAngle,
                    Color *color) 
            {
                Q_UNUSED(point);
                Q_UNUSED(width);
                Q_UNUSED(height);
                Q_UNUSED(startAngle);
                Q_UNUSED(spanAngle);
                Q_UNUSED(color);
            }


            virtual IGraphInteractiveRenderer *interactiveRenderer() = 0; //{ return NULL; }
            virtual void setInteractiveRenderer(IGraphInteractiveRenderer *) = 0; //{}

        protected:
            GraphRenderer() {}
    };
}
#endif
