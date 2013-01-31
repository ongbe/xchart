
#ifndef _GRAPH_IMAGE_H_
#define _GRAPH_IMAGE_H_

#include "graph_element.h"
#include "graph_color.h"
#include <QImage>

namespace GraphLib 
{
    typedef QImage GraphImage;
}

namespace GraphLib
{
    class GraphRenderer;
    class Image : public Element
    {
        public:
            Image();
            ~Image();

        public:
            Color fillColor();
            void setFillColor(Color color);

            double borderWidth();
            void setBorderWidth(double width);

            bool backgroundVisible() const;
            void setBackgroundVisible(bool visible);

            void setBorderStyle(LineStyle mode);
            LineStyle borderStyle();


            void setImage(GraphImage &image);

            void setBorderColor(Color color);
            Color borderColor();
            Rectangle boundingBox();
            Rectangle enclosingBox();

            Object *copy();

            void setPos(Point p);
            Point pos();
        public:
            void addUpdates(IDoc *doc);

        public:
            /*
             * <data>
             * <geometry x="" y= "" width="" height="" />
             * <image>xxx.jpg</image>
             * </data>
             * */
            void setData(QString &xml);
            /*
             * <data>
             * <border>1.0</border>
             * </data>
             * */
            void setProperty(QString &xml);

        public:
            void draw(GraphRenderer *renderer);

            void selectf(Point *clickedPoint, 
                    GraphRenderer *interactiveRenderer);
 
            double distanceFrom(Point *point);

            std::vector<Handle *> handles();
            GraphObjectChange *moveHandle(Handle *handle,
                                          Point *to,
                                          HandleMoveReason reason);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif
