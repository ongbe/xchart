#include "graph_poly_shape.h"
#include "graph_color.h"
#include "graph_renderer.h"

#include "graph_geometry.h"
#include "graph_bounding_box.h"

#include <QVector>
#include <math.h>
#include <QDomDocument>

using namespace GraphLib;

struct PolyShape::PrivateData
{
    int numPoint;                           //顶点数 >= 3
    QVector<Point> points;

    QVector<Handle *>  handles;
    PolyShape::ElementBBExtras extraSpacing;
};

PolyShape::PolyShape()
    : Element(3)
{
    m_data = new PrivateData;
    m_data->numPoint = 3;

    m_data->points.append(Point(100, 100));
    m_data->points.append(Point(100, 200));
    m_data->points.append(Point(200, 200));

    for (int i = 0; i < 3; ++i) {
        Handle *handle = new Handle;
        handle->setType(HANDLE_MAJOR_CONTROL);

        m_data->handles.push_back(handle);
    }
}

PolyShape::~PolyShape()
{
    delete m_data;
}

void PolyShape::setPoints(QVector<Point> vector)
{
    m_data->numPoint = vector.size();
    m_data->points.clear();
    m_data->handles.clear();

    foreach (Point point, vector) {
        m_data->points.append(point);
        addHandle();
    }

    updateHandles();
}

QVector<Point> PolyShape::points()
{
    return m_data->points;
}

void PolyShape::draw(GraphRenderer *renderer)
{
    int num = m_data->points.size();
    Color color(0, 0, 0);
    Point *point = &m_data->points[0];

    renderer->drawPolygon(point, num, &color);
}

void PolyShape::addPoint(int segment, const Point &point)
{
    /*
      *popint
    if (point == NULL) {
        realpoint.x = (poly->points[segment].x+poly->points[segment+1].x)/2;
        realpoint.y = (poly->points[segment].y+poly->points[segment+1].y)/2;
      } else {
        realpoint = *point;
      }
    */

    Handle *newHandle = new Handle;
    newHandle->setId(HANDLE_CUSTOM1);
    newHandle->setType(HANDLE_MAJOR_CONTROL);

    Point p = point;
    addHandle(segment, &p, newHandle);
}

void PolyShape::removePoint(int pos)
{
    m_data->points.remove(pos);
    m_data->numPoint = m_data->points.size();

    removeHandle(pos);
}

int PolyShape::closestSegment(double lineWidth, const Point &point)
{
    int num = m_data->numPoint;
    int closest = num - 1;

    double dist =
            distance_line_point( &m_data->points[num - 1],
                                &m_data->points[0],
                                lineWidth, &point);

    for (int i = 0; i < num - 1; ++i) {
        double new_dist =
                distance_line_point( &m_data->points[i],
                                     &m_data->points[i+1],
                                     lineWidth,
                                     &point);
        if (new_dist < dist) {
            dist = new_dist;
            closest = i;
        }
    }

    return closest + 1;
}

void PolyShape::setElementBBExtras(ElementBBExtras extras)
{
    m_data->extraSpacing.borderTrans = extras.borderTrans;
}

PolyShape::ElementBBExtras PolyShape::elementBBExtras()
{
    return m_data->extraSpacing;
}

Rectangle PolyShape::boundingBox()
{
    Rectangle rect;
    PolyBBExtras pextra;

    pextra.startTrans = pextra.endTrans =
            pextra.startLong = pextra.endLong = 0;

    pextra.middleTrans = m_data->extraSpacing.borderTrans;

    polyline_bbox(&m_data->points[0],
                  m_data->numPoint,
                  &pextra, true,
                  &rect);
    return rect;
}

double PolyShape::distanceFrom(Point *point,
                               float lineWidth)
{
    Point *poly = &m_data->points[0];
    int npoints = m_data->points.size();

    return distance_polygon_point(poly, npoints,
                                  lineWidth, point);
}

void PolyShape::updateHandles()
{
    for (int i = 0; i < m_data->numPoint; ++i) {
        Point point = m_data->points.at(i);

        Handle *handle = m_data->handles.at(i);
        handle->setPos(point);
        handle->setId(HANDLE_CUSTOM1);
    }
}

std::vector<Handle *> PolyShape::handles()
{
    std::vector<Handle *> vector;
    foreach (Handle *handle, m_data->handles)
        vector.push_back(handle);

    return vector;
}

void PolyShape::addHandle()
{
    Handle *handle = new Handle;
    handle->setType(HANDLE_MAJOR_CONTROL);

    m_data->handles.push_back(handle);
}

void PolyShape::addHandle(int pos, Point *point, Handle *handle)
{
    m_data->numPoint++;
    m_data->points.insert(pos, *point);
    m_data->handles.insert(pos, handle);
}

void PolyShape::removeHandle(int pos)
{
    Handle *handle = m_data->handles.at(pos);
    m_data->handles.remove(pos);

    delete handle;
}

int PolyShape::closestHandle(const Point &point)
{
    int pos = 0;
    double dist = distance_point_point(&point,
                                       &m_data->points[0]);

    for (int i = 0; i < m_data->numPoint; ++i) {
        double newDist = distance_point_point(&point,
                                              &m_data->points.at(i));
        if (dist > newDist) {
            dist = newDist;
            pos = i;
        }
    }

    return pos;
}

GraphObjectChange *PolyShape::move(Point *from, Point *to)
{
    Point p = *to;

    point_sub(&p, from);
    for (int i = 0; i < m_data->numPoint; ++i) {
        Point point = m_data->points.at(i);

        point_add(&point, &p);

        m_data->points.replace(i, point);
    }

    return NULL;
}

GraphObjectChange *PolyShape::moveHandle(Handle *handle,
                              Point *pos,
                              HandleMoveReason reason)
{
    int index = m_data->handles.indexOf(handle);
    m_data->points.replace(index, *pos);

    return NULL;
}

void PolyShape::setData(QString &xml)
{
    m_data->points.clear();
    m_data->handles.clear();

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
        if (element.tagName() == "point") {
            double x = element.attribute("x").toDouble();
            double y = element.attribute("y").toDouble();

            m_data->points.append(Point(x, y));
            addHandle();
        }
    }

    m_data->numPoint = m_data->points.size();
    updateHandles();
}

static int first_direction(int dirs) {
  switch (dirs) {
  case PolyShape::DIR_NORTHEAST:
      return PolyShape::DIR_NORTH;
  case PolyShape::DIR_SOUTHEAST:
      return PolyShape::DIR_EAST;
  case PolyShape::DIR_NORTHWEST:
      return PolyShape::DIR_WEST;
  case PolyShape::DIR_SOUTHWEST:
      return PolyShape::DIR_SOUTH;
  default:
      return dirs;
  }
}

static int last_direction(int dirs) {
  switch (dirs) {
  case PolyShape::DIR_NORTHEAST:
      return PolyShape::DIR_EAST;
  case PolyShape::DIR_SOUTHEAST:
      return PolyShape::DIR_SOUTH;
  case PolyShape::DIR_NORTHWEST:
      return PolyShape::DIR_NORTH;
  case PolyShape::DIR_SOUTHWEST:
      return PolyShape::DIR_WEST;
  default:
      return dirs;
  }
}

static int find_slope_directions(Point from, Point to)
{
  int dirs;
  int slope;

  if (fabs(from.y() - to.y()) < 0.0000001)
    return (from.x() > to.x() ? PolyShape::DIR_SOUTH : PolyShape::DIR_NORTH);
  if (fabs(from.x() - to.x()) < 0.0000001)
        return (from.y() > to.y() ? PolyShape::DIR_WEST : PolyShape::DIR_EAST);

  point_sub(&to, &from);
  slope = fabs(to.y() / to.x());

  dirs = 0;
  if (slope < 2) { /* Flat enough to allow north-south */
    if (to.x() > 0) { /* The outside is north, not south */
      dirs |= PolyShape::DIR_NORTH;
    } else {
      dirs |= PolyShape::DIR_SOUTH;
    }
  }
  if (slope > .5) { /* Steep enough to allow east-west */
    if (to.y() > 0) {  /* The outside is east, not west */
      dirs |= PolyShape::DIR_EAST;
    } else {
      dirs |= PolyShape::DIR_WEST;
    }
  }
  return dirs;
}

static int find_tip_directions(Point prev, Point current, Point next)
{
  int startdirs = find_slope_directions(prev, current);
  int enddirs = find_slope_directions(current, next);
  int firstdir = first_direction(startdirs);
  int lastdir = last_direction(enddirs);
  int dir, dirs = 0;

  dir = firstdir;
  while (dir != lastdir) {
    dirs |= dir;
    dir = dir * 2;
    if (dir == 16) dir = 1;
  }
  dirs |= dir;

  return dirs;
}
