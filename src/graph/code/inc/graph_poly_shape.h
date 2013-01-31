#ifndef GRAPH_POLY_SHAPE_H
#define GRAPH_POLY_SHAPE_H

#include "graph_element.h"

namespace GraphLib
{

class Handle;

class PolyShape : public Element
{
public:
    struct ElementBBExtras
    {
        double borderTrans;
    };

    enum PointDirection{
        DIR_NONE  = 0,
        DIR_NORTH = (1<<0),
        DIR_EAST  = (1<<1),
        DIR_SOUTH = (1<<2),
        DIR_WEST  = (1<<3),

        /* Convenience directions */
        DIR_NORTHEAST = (DIR_NORTH|DIR_EAST),
        DIR_SOUTHEAST = (DIR_SOUTH|DIR_EAST),
        DIR_NORTHWEST = (DIR_NORTH|DIR_WEST),
        DIR_SOUTHWEST = (DIR_SOUTH|DIR_WEST),
        DIR_ALL       = (DIR_NORTH|DIR_SOUTH|DIR_EAST|DIR_WEST)
    };

public:
    PolyShape();
    ~PolyShape();

public:
    void setPoints(QVector<Point> vector);
    QVector<Point> points();

    void addPoint(int segment, const Point &point);
    void removePoint(int pos);

    int closestSegment(double lineWidth, const Point &point);

    void setElementBBExtras(ElementBBExtras extras);
    ElementBBExtras elementBBExtras();

public:
    void setData(QString &xml);

public:
    void draw(GraphRenderer *);

    Rectangle boundingBox();

    void updateHandles();
    void addHandle();
    void addHandle(int pos,
                   Point *point,
                   Handle *handle);

    void removeHandle(int pos);
    int closestHandle(const Point &point);


    std::vector<Handle *> handles();
    GraphObjectChange * move(Point *from, Point *to);
    GraphObjectChange *moveHandle(Handle *,
                                  Point *pos,
                                  HandleMoveReason reason);

    double distanceFrom(Point *point, float lineWidth);

private:
    struct PrivateData;
    PrivateData *m_data;
};

}

#endif // GRAPH_POLY_SHAPE_H
