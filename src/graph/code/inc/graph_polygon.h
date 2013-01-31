#ifndef GRAPH_POLYGON_H
#define GRAPH_POLYGON_H

#include "graph_element.h"
#include "graph_color.h"
#include "graph_poly_shape.h"

namespace GraphLib {

class GraphRenderer;
class Handle;

class Polygon : public Element
{
public:
    struct Parameters {
        Color lineColor;
        LineStyle lineStyle;

        Color backgroundColor;
        bool showBackground;

        float lineWidth;
    };

public:
    Polygon();
    ~Polygon();

public:
    void setParameters(Parameters parameters);
    Parameters parameters();

    PolyShape *polyShape();

public:
    void draw(GraphRenderer *);

    double distanceFrom(Point *point);
    void selectf(Point *clickedPoint,
                 GraphRenderer *interactiveRenderer);

public:
    GraphLib::Object *copy();

public:
    void setData(QString &xml);
    void setProperty(QString &xml);

    void setClickedPoint(Point *point);

public:
    void updateHandles();
    std::vector<Handle *> handles();

    GraphObjectChange *move(Point *to);
    GraphObjectChange *moveHandle(Handle *handle,
                                  Point *to,
                                  HandleMoveReason reason);
    Rectangle boundingBox();
    Rectangle enclosingBox();

    void addUpdates(IDoc *doc);

    void addPoint(const Point &point);
    void removePoint(const Point &point);

    int pointSize();

private:
    void updateData();

private:
    struct PrivateData;
    PrivateData *m_data;
};

}

#endif // GRAPH_LINE_H
