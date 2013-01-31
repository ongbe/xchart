
#ifndef _GRAPH_ELLIPSE_H_
#define _GRAPH_ELLIPSE_H_

#include "graph_element.h"
#include "graph_renderer.h"

namespace GraphLib
{
    class Ellipse : public Element
    {
        public:
            enum AspectType
            {
                FREE_ASPECT,
                FIXED_ASPECT,
                CIRCLE_ASPECT
            };

        public:
            Ellipse();
            ~Ellipse();

        public:
            Point pos();
            void setPos(Point p);

        public:
            double borderWidth();
            void setBorderWidth(double width);

            Color borderColor();
            void setBorderColor(Color color);

            Color innerColor();
            void setInnerColor(Color color);

            bool showBackground();
            void setShowBackground(bool show);

            Ellipse::AspectType aspectType();
            void setAspectType(Ellipse::AspectType type);

            LineStyle lineStyle();
            void setLineStyle(LineStyle style);

            double dashLength();
            void setDashLength(double length);

        public:
            void setData(QString &xml);
            void setProperty(QString &xml);

        public:
            GraphLib::Object *copy();

        public:
            void updateHandles();
            std::vector<Handle *> handles();
            GraphObjectChange * move(Point *to);
            GraphObjectChange *moveHandle(Handle *handle,
                                          Point *to,
                                          HandleMoveReason reason);

        public:
            void draw(GraphRenderer *renderer);
            void selectf(Point *clickedPoint, GraphRenderer *interactiveRenderer);
            double distanceFrom(Point *point);

        private:
            void updateData();

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}

#endif
