


#ifndef _GRAPH_POLYCONN_H_
#define _GRAPH_POLYCONN_H_

#include "graph_element.h"
#include "graph_color.h"
#include "graph_bounding_box.h"

#include <QRectF>
#include <QPointF>

namespace GraphLib
{
    class GraphRenderer;

    class Polyconn : public Element
    {
        public:
            Polyconn();
            ~Polyconn();

        public:
            void draw(GraphRenderer *renderer);
            Rectangle boundingBox();
            QVector<Point> points();
            void updateHandles();
            PolyBBExtras *extraSpacing();

            std::vector<Handle *> handles();
            GraphObjectChange *move(Point *from, Point *to);
            GraphObjectChange *moveHandle(Handle *, Point *pos, HandleMoveReason reason);
            void addPoint(const Point &p);
            void removePoint(const Point &p);
            void clearPoints();


            void setOPoints(const QVector<Point> &);
            QVector<Point> OPoints();

            void setCorner(Point);
            Point corner();

        private:
            int closestSegment(const Point &p, double lineWidth = 1.0);
            int closestHandle(const Point &p);

        private:
            struct PrivateData;
            PrivateData *m_data;
    };

}


#endif
