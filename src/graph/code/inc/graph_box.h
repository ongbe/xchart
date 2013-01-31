


#ifndef _GRAPH_BOX_H_
#define _GRAPH_BOX_H_

#include "graph_element.h"
#include "graph_color.h"

namespace GraphLib
{
    class GraphRenderer;
    class Box : public Element
    {
        public:
            Box();
            virtual ~Box();

        public:
            // 将pos和corner统一起来
            void setCorner(Point pos);
            Point pos();
            void setPos(Point pos);

            Color fillColor();
            void setFillColor(Color color);

            double borderWidth();
            void setBorderWidth(double width);

            bool backgroundVisible() const;
            void setBackgroundVisible(bool visible);

            void setBorderStyle(LineStyle mode);
            LineStyle borderStyle();

            void setBorderColor(Color color);
            Color borderColor();

        public:
            void draw(GraphRenderer *renderer);

            void selectf(Point *clickedPoint, 
                    GraphRenderer *interactiveRenderer);
 
            double distanceFrom(Point *point);

        public:
            GraphLib::Object *copy();

        public:
            void setData(QString &xml);
            void setProperty(QString &xml);
            
        public:
            void updateHandles();
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
