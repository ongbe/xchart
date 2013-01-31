#include "graph_polyconn.h"
#include "graph_renderer.h"
#include "graph_element.h"
#include "igraph_doc.h"

#include "graph_geometry.h"

#include <QVector>
#include <QDebug>
#include <QTransform>
#include <math.h>

namespace GraphLib
{
    struct Polyconn::PrivateData
    {
        int numpoints;
        QVector<Point> points;
        QVector<Point> oringalpoints;
        PolyBBExtras extraSpacing;
        QVector<Handle *> handles;
        Point corner;
    };

    Polyconn::Polyconn() : Element(3)
    {
        m_data = new PrivateData;
        m_data->points << Point(50, 250);
        m_data->points << Point(100, 300);
        m_data->points << Point(100, 500);
        m_data->oringalpoints << Point(50, 250);
        m_data->oringalpoints << Point(100, 300);
        m_data->oringalpoints << Point(100, 500);
        m_data->extraSpacing.startLong = 1.0;
        m_data->extraSpacing.middleTrans = 1.0;
        m_data->extraSpacing.endLong = 1.0;
        m_data->extraSpacing.endTrans = 1.0;
        for (int i = 0; i < 3; i++) {
            Handle *handle = new Handle;
            handle->setType(HANDLE_MAJOR_CONTROL);
            m_data->handles.push_back(handle);
        }

    }

    Polyconn::~Polyconn()
    {
        delete m_data;
    }
    void Polyconn::draw(GraphRenderer *renderer)
    {
        return;
    }

    Rectangle Polyconn::boundingBox()
    {
        Rectangle rect;
        polyline_bbox(&m_data->points.first(),
                m_data->points.size(),
                &m_data->extraSpacing,
                false,
                &rect);

        rect.adjust(-5.0, -5.0, 10.0, 10.0);

        return rect;
    }

    QVector<Point> Polyconn::points()
    {
        //return m_data->oringalpoints;
        return m_data->points;
    }
    PolyBBExtras *Polyconn::extraSpacing()
    {
        return &m_data->extraSpacing;
    }
    void Polyconn::updateHandles()
    {
        int size = m_data->handles.size();
        for (int i = 0; i < size; ++i) {
            Point p = m_data->points.at(i);
            m_data->handles.at(i)->setPos(p);
        }
    }

    GraphObjectChange *Polyconn::move(Point *, Point *)
    {
                return NULL;
    }
    GraphObjectChange *Polyconn::moveHandle(Handle *h, Point *pos, 
            HandleMoveReason reason)
    {
        if (!m_data->handles.contains(h))
            return NULL;
        int index = m_data->handles.indexOf(h);
        m_data->points.replace(index, *pos);
        return NULL;
    }

    void Polyconn::clearPoints()
    {
        m_data->points.clear();
        m_data->handles.clear();
    }
    void Polyconn::removePoint(const Point &p)
    {
        int pos = closestHandle(p);
        m_data->points.remove(pos);
        m_data->handles.remove(pos);
    }
    void Polyconn::addPoint(const Point &p)
    {
        Handle *newHandle = new Handle;
        newHandle->setType(HANDLE_MAJOR_CONTROL);
        if (m_data->points.size() == 0) {
            m_data->points.insert(0, p);
            m_data->handles.insert(0, newHandle);
            return;
        }
        if (m_data->points.size() == 1) {
            m_data->points.insert(1, p);
            m_data->handles.insert(1, newHandle);
            return;
        }
        Point realPoint;
        int segment = closestSegment(p);
        Point p1 = m_data->points[segment];
        Point p2 = m_data->points[segment+1];
        realPoint.setX(p1.x() / 2 + p2.x() / 2);
        realPoint.setY(p1.y() / 2 + p2.y() / 2);
        m_data->points.insert(segment, p);
        m_data->handles.insert(segment, newHandle);
    }

    int Polyconn::closestHandle(const Point &p)
    {
        int pos = 0;
        Point firstPoint = m_data->points.first();
        double dist = distance_point_point(&p, &firstPoint);

        for (int i = 1; i < m_data->points.size() - 1; ++i) {
            double newDist;
            if (newDist < dist) {
                dist = newDist;
                pos = i;
            }
        }
        return pos; 
    }

    int Polyconn::closestSegment(const Point &p, double lineWidth)
    {
        int i;
        Point p1 = m_data->points.at(0);
        Point p2 = m_data->points.at(1);
        double dist = distance_line_point(&p1, &p2, lineWidth, &p);
        int closest = 0;
        for (int i = 1; i < m_data->points.size() - 1; i++) {
            Point p3 = m_data->points.at(i);
            Point p4 = m_data->points.at(i+1);
            double newDist = distance_line_point(&p3, &p4, lineWidth, &p);
            if (newDist < dist) {
                dist = newDist;
                closest = i;
            }
        }
        return closest;
    }

    std::vector<Handle *> Polyconn::handles()
    {
        std::vector<Handle *> handles;

        QVector<Handle *>::iterator it = m_data->handles.begin();
        for (it; it != m_data->handles.end(); ++it) {
            handles.push_back(*it);
        }
        return handles;
    }
    void Polyconn::setCorner(Point p)
    {
        m_data->corner = p;
        Point corner = this->corner();
        QTransform transform;

        transform.translate(corner.x(), corner.y());
        QVector<Point> points;
        int num = m_data->oringalpoints.size();
        for (int i = 0; i < num; ++i) {
            Point p = transform.map(m_data->oringalpoints.at(i));
            points << p;
            m_data->handles.at(i)->setPos(p);
        }

        m_data->points = points;

    }
    Point Polyconn::corner()
    {
        return m_data->corner;
    }
    void Polyconn::setOPoints(const QVector<Point> &v)
    {
        m_data->oringalpoints.clear();
        m_data->oringalpoints = v;
        for (int i = 0; i < 3; i++) {
            Handle *handle = new Handle;
            handle->setType(HANDLE_MAJOR_CONTROL);
            m_data->handles.push_back(handle);
        }
    }
    QVector<Point> Polyconn::OPoints()
    {
        return m_data->oringalpoints;
    }
}
