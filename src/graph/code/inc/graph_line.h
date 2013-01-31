#ifndef GRAPH_LINE_H
#define GRAPH_LINE_H

#include "graph_element.h"
#include "graph_color.h"
#include "graph_bounding_box.h"

namespace GraphLib {
class GraphRenderer;
class Handle;
class Line : public Element
{
public:
    struct Parameters {
        Point startPoint;
        Point endPoint;

        Color lineColor;
        LineStyle lineStyle;
        LineJoin lineJoin;
        LineCaps lineCaps;

        float lineWidth;
    };

public:
    Line();
    ~Line();

public:
    void setParameters(Parameters parameters);
    Parameters parameters();

    void setLineBBExtras(LineBBExtras lineBBExtras);
    LineBBExtras lineBBExtras();

public:
    void setStartPos(Point );
    void setEndPos(Point );

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

    void setArrowVisible(bool);

private:
    void updateData();

private:
    struct PrivateData;
    PrivateData *m_data;
};

}

#endif // GRAPH_LINE_H
