


#ifndef _GRAPH_POLYLINE_H_
#define _GRAPH_POLYLINE_H_

#include "graph_element.h"
#include "graph_color.h"

namespace GraphLib
{
    class GraphRenderer;
    class Polyconn;
    class Polyline : public Element
    {
        public:
            Polyline();
            ~Polyline();

        public:
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
 
        public:
            void setData(QString &xml);
            void setProperty(QString &xml);
            Object *copy();
            GraphLib::Polyconn *polyConn();
        public:

            double distanceFrom(Point *point);
            Rectangle boundingBox();
            Rectangle enclosingBox();
            void addUpdates(IDoc *doc);

            GraphObjectChange *move(Point *to);
            GraphObjectChange *moveHandle(Handle *, Point *pos, HandleMoveReason reason);
            IDoc *doc();
            void setDoc(IDoc *doc);
            void updateHandles();
            std::vector<Handle *> handles();

            void addPoint(const Point &);
            void removePoint(const Point &);
            Point corner();
            void setCorner(Point );

        private:
            struct PrivateData;
            PrivateData *m_data;
    };
}


#endif
