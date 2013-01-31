#include "graph_line.h"

#include "graph_renderer.h"
#include "graph_element.h"
#include "graph_doc.h"
#include "graph_geometry.h"
#include "graph_bounding_box.h"

#include <math.h>
#include <QDebug>
#include <QDomDocument>

using namespace  GraphLib;

const qreal Pi = 3.14;
struct Line::PrivateData
{
    Point clickedPoint;
    Handle resizeHandles[2];

    LineBBExtras extraSpacing;
    Line::Parameters parameters;

    bool arrowIsVisible;
};

Line::Line()
    : Element(2)
{
    m_data = new PrivateData;

    m_data->parameters.lineColor = Color(0, 0, 0);
    m_data->parameters.lineWidth = 0.3;
    m_data->parameters.lineStyle = Qt::SolidLine;
    m_data->parameters.lineCaps = Qt::FlatCap;
    m_data->parameters.lineJoin = Qt::MiterJoin;

    m_data->parameters.startPoint = Point(0,0);
    m_data->parameters.endPoint = Point(0,0);

    m_data->extraSpacing.endLong = 0;
    m_data->extraSpacing.endTrans = 0;
    m_data->extraSpacing.startLong = 0;
    m_data->extraSpacing.startTrans = 0;

   for (int i = 0; i < 2; i++) {
        m_data->resizeHandles[i].setType(HANDLE_MAJOR_CONTROL);
    }

    m_data->arrowIsVisible = true;;
}

Line::~Line()
{
    delete m_data;
}

void Line::setParameters(Parameters parameters)
{
    m_data->parameters = parameters;
}

Line::Parameters Line::parameters()
{
    return m_data->parameters;
}

void Line::setLineBBExtras(LineBBExtras extras)
{
    m_data->extraSpacing = extras;
}

LineBBExtras Line::lineBBExtras()
{
    return m_data->extraSpacing;
}

void Line::draw(GraphRenderer *renderer)
{
    if (renderer == NULL)
        return;

    renderer->setLineWidth(m_data->parameters.lineWidth);
    renderer->setLineStyle(m_data->parameters.lineStyle);
    renderer->setLineCaps(m_data->parameters.lineCaps);
    renderer->setLineJoin(m_data->parameters.lineJoin);

    renderer->drawLine(&m_data->parameters.startPoint,
                       &m_data->parameters.endPoint,
                       &m_data->parameters.lineColor);

    if (!m_data->arrowIsVisible)
        return;

    double x = m_data->parameters.endPoint.x() - m_data->parameters.startPoint.x();
    double y = m_data->parameters.endPoint.y() - m_data->parameters.startPoint.y();

    double lengthB = sqrt(x * x + y * y);

    double angle = ::acos(x / lengthB);
    if (y > 0)
        angle = (Pi * 2) - angle;
    QPointF arrowP1 = m_data->parameters.endPoint +
            QPoint(sin(angle - Pi / 3) *
                   m_data->parameters.lineWidth * 8,
                   cos(angle - Pi / 3) *
                   m_data->parameters.lineWidth * 8);

    QPointF arrowP2 = m_data->parameters.endPoint +
            QPoint(sin(angle - Pi + Pi / 3) *
                   m_data->parameters.lineWidth * 8,
                   cos(angle - Pi + Pi / 3) *
                   m_data->parameters.lineWidth * 8);

    QVector<QPointF> vector;
    vector << arrowP1 << m_data->parameters.endPoint << arrowP2;
    Point *point = &vector[0];

    renderer->drawPolyline(point, 3, &m_data->parameters.lineColor);
}

double Line::distanceFrom(Point *point)
{
    Point *line_start = &m_data->parameters.startPoint;
    Point *line_end = &m_data->parameters.endPoint;
    double line_width = m_data->parameters.lineWidth;

    return distance_line_point(line_start, line_end,
                               line_width, point);

}

void Line::selectf(Point *clickedPoint,
             GraphRenderer *interactiveRenderer)
{
    updateHandles();
}

void Line::updateHandles()
{
    m_data->resizeHandles[0].setId(HANDLE_MOVE_STARTPOINT);
    m_data->resizeHandles[0].setPos(m_data->parameters.startPoint);

    m_data->resizeHandles[1].setId(HANDLE_MOVE_ENDPOINT);
    m_data->resizeHandles[1].setPos(m_data->parameters.endPoint);
}

std::vector<Handle *> Line::handles()
{
    std::vector<Handle *> handles;
    for (int i = 0; i < 2; ++i)
        handles.push_back(&m_data->resizeHandles[i]);

    return handles;
}

GraphLib::Object *Line::copy()
{
    Line *line = new Line;

    line->setParameters(parameters());
    line->setLineBBExtras(lineBBExtras());

    return line;
}

void Line::setClickedPoint(Point *point)
{
    if(point)
        m_data->clickedPoint = *point;
}

GraphObjectChange *Line::move(Point *to)
{
    Point start_to_end = *to;

    point_sub(&start_to_end, &m_data->clickedPoint);
    point_add(&m_data->parameters.endPoint, &start_to_end);
    point_add(&m_data->parameters.startPoint, &start_to_end);

    m_data->clickedPoint = *to;

    updateData();

    return NULL;
}

GraphObjectChange *Line::moveHandle(Handle *handle,
                                    Point *to,
                                    HandleMoveReason /*reason*/)
{
    int id = handle->id();

    switch(id) {
     case HANDLE_MOVE_STARTPOINT:
       m_data->parameters.startPoint = *to;
       break;
     case HANDLE_MOVE_ENDPOINT:
       m_data->parameters.endPoint = *to;
       break;
     default:
       break;
     }

    updateData();

    return NULL;
}

Rectangle Line::boundingBox()
{
    Rectangle rect;

    double lineWidth = m_data->parameters.lineWidth;
    m_data->extraSpacing.endLong = lineWidth / 2.0;

    line_bbox(&m_data->parameters.startPoint, &m_data->parameters.endPoint,
              &m_data->extraSpacing, &rect);

    return rect;
}

Rectangle Line::enclosingBox()
{
    Rectangle rect;

    double lineWidth = m_data->parameters.lineWidth;
    m_data->extraSpacing.endLong = lineWidth / 2.0;

    line_bbox(&m_data->parameters.startPoint, &m_data->parameters.endPoint,
              &m_data->extraSpacing, &rect);

    return rect;
}

void Line::addUpdates(IDoc *doc)
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

void Line::updateData()
{
    updateHandles();
}

void Line::setData(QString &xml)
{
    QDomDocument doc("xml");
    if (!doc.setContent(xml, false))
        return;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "data")
        return;

    QDomNodeList nodes = root.childNodes();
    for (int i = 0; i < nodes.count(); i++) {
        QDomNode node = nodes.at(i);

        QDomElement element = node.toElement();
        if (element.tagName() == "start_point") {
            double x = element.attribute("x").toDouble();
            double y = element.attribute("y").toDouble();

            m_data->parameters.startPoint.setX(x);
            m_data->parameters.startPoint.setY(y);

        }

        if (element.tagName() == "end_point") {
            double x = element.attribute("x").toDouble();
            double y = element.attribute("y").toDouble();

            m_data->parameters.endPoint.setX(x);
            m_data->parameters.endPoint.setY(y);
        }
    }
}

void Line::setProperty(QString &xml)
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

        if (tagName == "line_cap")
            m_data->parameters.lineCaps =
                    (GraphLib::LineCaps)e.text().toInt();

        if (tagName == "line_join")
            m_data->parameters.lineJoin =
                    (GraphLib::LineJoin)e.text().toInt();

        n = n.nextSibling();
    }
}

void Line::setStartPos(Point p)
{
    m_data->parameters.startPoint = p;
}
void Line::setEndPos(Point p)
{
    m_data->parameters.endPoint = p;
}

void Line::setArrowVisible(bool flag)
{
    m_data->arrowIsVisible = flag;
}
