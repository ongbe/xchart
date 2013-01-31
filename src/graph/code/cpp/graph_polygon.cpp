#include "graph_polygon.h"
#include "graph_poly_shape.h"

#include "graph_renderer.h"
#include "graph_element.h"
#include "graph_doc.h"

#include <math.h>
#include <QDebug>
#include <QVector>
#include <QDomDocument>

using namespace  GraphLib;

struct Polygon::PrivateData
{
    Point clickedPoint;
    std::vector<Handle *> resizeHandles;

    Polygon::Parameters parameters;
    PolyShape polyShape;
};

Polygon::Polygon()
    : Element(3)
{
    m_data = new PrivateData;

    m_data->parameters.lineColor = Color(0, 0, 0);
    m_data->parameters.lineWidth = 0.3;
    m_data->parameters.lineStyle = Qt::SolidLine;
    m_data->parameters.backgroundColor = Color(255, 255, 255);
    m_data->parameters.showBackground = false;

}

Polygon::~Polygon()
{
    delete m_data;
}

void Polygon::setParameters(Parameters parameters)
{
    m_data->parameters = parameters;
}

Polygon::Parameters Polygon::parameters()
{
    return m_data->parameters;
}

PolyShape *Polygon::polyShape()
{
    return &m_data->polyShape;
}

void Polygon::draw(GraphRenderer *renderer)
{
    if (!renderer)
        return;

    QVector<Point> points = m_data->polyShape.points();
    Point *point = &points[0];
    int num = points.size();

    renderer->setLineWidth(m_data->parameters.lineWidth);
    renderer->setLineStyle(m_data->parameters.lineStyle);

    renderer->drawPolygon(point, num, &m_data->parameters.lineColor);

    if (m_data->parameters.showBackground)
        renderer->fillPolygon(point, num, &m_data->parameters.backgroundColor);
}

double Polygon::distanceFrom(Point *point)
{
    return m_data->polyShape.distanceFrom(point,
                                          m_data->parameters.lineWidth);
}

static void polyshapeSetPoints(PolyShape *poly, int num_points, Point *points)
{
    QVector<Point> vector;
    for (int i = 0; i < num_points; i++) {
        vector.append(points[i]);
    }

    poly->setPoints(vector);
}

static void polyShapeCopy(PolyShape *from, PolyShape *to)
{
    QVector<Point> points = from->points();
    Point *point = &points[0];
    int num = points.size();

    polyshapeSetPoints(to, num, point);
}

GraphLib::Object *Polygon::copy()
{
    Polygon *polygon = new Polygon;
    PolyShape *polyShape = polygon->polyShape();

    polygon->setParameters(parameters());

    polyShapeCopy(&m_data->polyShape, polyShape);

    return polygon;
}

void Polygon::selectf(Point *clickedPoint,
             GraphRenderer *interactiveRenderer)
{
    updateHandles();
}

void Polygon::updateHandles()
{
    m_data->polyShape.updateHandles();
}

std::vector<Handle *> Polygon::handles()
{
    return m_data->polyShape.handles();
}

void Polygon::setClickedPoint(Point *point)
{
    if (point)
        m_data->clickedPoint = *point;
}

GraphObjectChange *Polygon::move(Point *to)
{
    m_data->polyShape.move(&m_data->clickedPoint, to);

    updateData();

    m_data->clickedPoint = *to;

    return NULL;
}

GraphObjectChange *Polygon::moveHandle(Handle *handle,
                                    Point *to,
                                    HandleMoveReason reason)
{
    m_data->polyShape.moveHandle(handle, to, reason);

    updateData();
}

Rectangle Polygon::boundingBox()
{
    return m_data->polyShape.boundingBox();
}

Rectangle Polygon::enclosingBox()
{
    return m_data->polyShape.boundingBox();
}

void Polygon::addUpdates(IDoc *doc)
{
    if (doc) {
        Rectangle rect = enclosingBox();
        doc->addUpdate(&rect);
    }

    typedef std::vector<Handle *> Vector;
    typedef std::vector<Handle *>::iterator It;

    Vector vector = this->handles();
    It it;

    for (it = vector.begin(); it != vector.end(); it++) {
        Handle *h = *it;
        h->addUpdate(doc);
    }
}

void Polygon::updateData()
{
    PolyShape::ElementBBExtras extras;
    extras.borderTrans = m_data->parameters.lineWidth / 2.0;

    m_data->polyShape.setElementBBExtras(extras);

    updateHandles();
}

void Polygon::setData(QString &xml)
{
    m_data->polyShape.setData(xml);
}

void Polygon::setProperty(QString &xml)
{
    QDomDocument doc("xml");
    if (!doc.setContent(xml, false))
        return;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "property")
        return;

    QDomNode n = root.firstChild();
    while (!n.isNull()) {
        QDomElement e = n.toElement();
        QString tagName = e.tagName();

        if (tagName == "line_width")
            m_data->parameters.lineWidth = e.text().toDouble();

        if (tagName == "line_color")
            m_data->parameters.lineColor = Color(e.text());

        if (tagName == "line_style")
            m_data->parameters.lineStyle =
                    (GraphLib::LineStyle)e.text().toInt();

        if (tagName == "show_background")
            m_data->parameters.showBackground = (bool)e.text().toInt();

        if (tagName == "fill_color")
            m_data->parameters.backgroundColor = Color(e.text());

        n = n.nextSibling();
    }
}

void Polygon::addPoint(const Point &point)
{
    int segment =
            m_data->polyShape.closestSegment(m_data->parameters.lineWidth,
                                             point);
    m_data->polyShape.addPoint(segment, point);

    updateData();
}

void Polygon::removePoint(const Point &point)
{
    int pos = m_data->polyShape.closestHandle(point);

    m_data->polyShape.removePoint(pos);
}

int Polygon::pointSize()
{
    return m_data->polyShape.points().size();
}
