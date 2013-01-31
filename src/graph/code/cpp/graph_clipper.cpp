
#include <qrect.h>
#include "graph_math.h"
#include "graph_clipper.h"

using namespace GraphLib;

static inline GraphDoubleRect boundingRect(const QPolygonF &polygon)
{
    return polygon.boundingRect();
}

enum Edge 
{ 
    Left, 
    Top, 
    Right, 
    Bottom, 
    NEdges 
};

class GraphPolygonClipper: public QRect
{
public:
    GraphPolygonClipper(const QRect &r);

    QPolygon clipPolygon(const QPolygon &) const;

private:
    void clipEdge(Edge, const QPolygon &, QPolygon &) const;
    bool insideEdge(const QPoint &, Edge edge) const;
    QPoint intersectEdge(const QPoint &p1,
        const QPoint &p2, Edge edge) const;

    void addPoint(QPolygon &, uint pos, const QPoint &point) const;
};

class GraphPolygonClipperF: public GraphDoubleRect
{
public:
    GraphPolygonClipperF(const GraphDoubleRect &r);
    QPolygonF clipPolygon(const QPolygonF &) const;

private:
    void clipEdge(Edge, const QPolygonF &, QPolygonF &) const;
    bool insideEdge(const GraphDoublePoint &, Edge edge) const;
    GraphDoublePoint intersectEdge(const GraphDoublePoint &p1,
        const GraphDoublePoint &p2, Edge edge) const;

    void addPoint(QPolygonF &, uint pos, const GraphDoublePoint &point) const;
};

class GraphCircleClipper: public GraphDoubleRect
{
public:
    GraphCircleClipper(const GraphDoubleRect &r);
    GraphArray<GraphDoubleInterval> clipCircle(
        const GraphDoublePoint &, double radius) const;

private:
    QList<GraphDoublePoint> cuttingPoints(
        Edge, const GraphDoublePoint &pos, double radius) const;
    double toAngle(const GraphDoublePoint &, const GraphDoublePoint &) const;
};

GraphPolygonClipper::GraphPolygonClipper(const QRect &r): 
    QRect(r) 
{
}

inline void GraphPolygonClipper::addPoint(
    QPolygon &pa, uint pos, const QPoint &point) const
{
    if ( uint(pa.size()) <= pos ) 
        pa.resize(pos + 5);

    pa.setPoint(pos, point);
}

//! Sutherland-Hodgman polygon clipping
QPolygon GraphPolygonClipper::clipPolygon(const QPolygon &pa) const
{
    if ( contains( pa.boundingRect() ) )
        return pa;

    QPolygon cpa(pa.size());

    clipEdge((Edge)0, pa, cpa);

    for ( uint edge = 1; edge < NEdges; edge++ ) 
    {
        const QPolygon rpa = cpa;
        clipEdge((Edge)edge, rpa, cpa);
    }

    return cpa;
}

bool GraphPolygonClipper::insideEdge(const QPoint &p, Edge edge) const
{
    switch(edge) 
    {
        case Left:
            return p.x() > left();
        case Top:
            return p.y() > top();
        case Right:
            return p.x() < right();
        case Bottom:
            return p.y() < bottom();
        default:
            break;
    }

    return false;
}

QPoint GraphPolygonClipper::intersectEdge(const QPoint &p1, 
    const QPoint &p2, Edge edge ) const
{
    int x=0, y=0;
    double m = 0;

    const double dy = p2.y() - p1.y();
    const double dx = p2.x() - p1.x();

    switch ( edge ) 
    {
        case Left:
            x = left();
            m = double(qAbs(p1.x() - x)) / qAbs(dx);
            y = p1.y() + int(dy * m);
            break;
        case Top:
            y = top();
            m = double(qAbs(p1.y() - y)) / qAbs(dy);
            x = p1.x() + int(dx * m);
            break;
        case Right:
            x = right();
            m = double(qAbs(p1.x() - x)) / qAbs(dx);
            y = p1.y() + int(dy * m);
            break;
        case Bottom:
            y = bottom();
            m = double(qAbs(p1.y() - y)) / qAbs(dy);
            x = p1.x() + int(dx * m);
            break;
        default:
            break;
    }

    return QPoint(x,y);
}

void GraphPolygonClipper::clipEdge(Edge edge, 
    const QPolygon &pa, QPolygon &cpa) const
{
    if ( pa.count() == 0 )
    {
        cpa.resize(0);
        return;
    }

    unsigned int count = 0;

    QPoint p1 = pa.point(0);
    if ( insideEdge(p1, edge) )
        addPoint(cpa, count++, p1);

    const uint nPoints = pa.size();
    for ( uint i = 1; i < nPoints; i++ )
    {
        const QPoint p2 = pa.point(i);
        if ( insideEdge(p2, edge) )
        {
            if ( insideEdge(p1, edge) )
                addPoint(cpa, count++, p2);
            else
            {
                addPoint(cpa, count++, intersectEdge(p1, p2, edge));
                addPoint(cpa, count++, p2);
            }
        }
        else
        {
            if ( insideEdge(p1, edge) )
                addPoint(cpa, count++, intersectEdge(p1, p2, edge));
        }
        p1 = p2;
    }
    cpa.resize(count);
}

GraphPolygonClipperF::GraphPolygonClipperF(const GraphDoubleRect &r): 
    GraphDoubleRect(r) 
{
}

inline void GraphPolygonClipperF::addPoint(QPolygonF &pa, uint pos, const GraphDoublePoint &point) const
{
    if ( uint(pa.size()) <= pos ) 
        pa.resize(pos + 5);

    pa[(int)pos] = point;
}

//! Sutherland-Hodgman polygon clipping
QPolygonF GraphPolygonClipperF::clipPolygon(const QPolygonF &pa) const
{
    if ( contains( ::boundingRect(pa) ) )
        return pa;

    QPolygonF cpa(pa.size());

    clipEdge((Edge)0, pa, cpa);

    for ( uint edge = 1; edge < NEdges; edge++ ) 
    {
        const QPolygonF rpa = cpa;
        clipEdge((Edge)edge, rpa, cpa);
    }

    return cpa;
}

bool GraphPolygonClipperF::insideEdge(const GraphDoublePoint &p, Edge edge) const
{
    switch(edge) 
    {
        case Left:
            return p.x() > left();
        case Top:
            return p.y() > top();
        case Right:
            return p.x() < right();
        case Bottom:
            return p.y() < bottom();
        default:
            break;
    }

    return false;
}

GraphDoublePoint GraphPolygonClipperF::intersectEdge(const GraphDoublePoint &p1, 
    const GraphDoublePoint &p2, Edge edge ) const
{
    double x=0.0, y=0.0;
    double m = 0;

    const double dy = p2.y() - p1.y();
    const double dx = p2.x() - p1.x();

    switch ( edge ) 
    {
        case Left:
            x = left();
            m = double(qAbs(p1.x() - x)) / qAbs(dx);
            y = p1.y() + int(dy * m);
            break;
        case Top:
            y = top();
            m = double(qAbs(p1.y() - y)) / qAbs(dy);
            x = p1.x() + int(dx * m);
            break;
        case Right:
            x = right();
            m = double(qAbs(p1.x() - x)) / qAbs(dx);
            y = p1.y() + int(dy * m);
            break;
        case Bottom:
            y = bottom();
            m = double(qAbs(p1.y() - y)) / qAbs(dy);
            x = p1.x() + int(dx * m);
            break;
        default:
            break;
    }

    return GraphDoublePoint(x,y);
}

void GraphPolygonClipperF::clipEdge(Edge edge, 
    const QPolygonF &pa, QPolygonF &cpa) const
{
    if ( pa.count() == 0 )
    {
        cpa.resize(0);
        return;
    }

    unsigned int count = 0;

    GraphDoublePoint p1 = pa[0];
    if ( insideEdge(p1, edge) )
        addPoint(cpa, count++, p1);

    const uint nPoints = pa.size();
    for ( uint i = 1; i < nPoints; i++ )
    {
        const GraphDoublePoint p2 = pa[(int)i];
        if ( insideEdge(p2, edge) )
        {
            if ( insideEdge(p1, edge) )
                addPoint(cpa, count++, p2);
            else
            {
                addPoint(cpa, count++, intersectEdge(p1, p2, edge));
                addPoint(cpa, count++, p2);
            }
        }
        else
        {
            if ( insideEdge(p1, edge) )
                addPoint(cpa, count++, intersectEdge(p1, p2, edge));
        }
        p1 = p2;
    }
    cpa.resize(count);
}

GraphCircleClipper::GraphCircleClipper(const GraphDoubleRect &r):
    GraphDoubleRect(r)
{
}

GraphArray<GraphDoubleInterval> GraphCircleClipper::clipCircle(
    const GraphDoublePoint &pos, double radius) const
{
    QList<GraphDoublePoint> points;
    for ( int edge = 0; edge < NEdges; edge++ )
        points += cuttingPoints((Edge)edge, pos, radius);

    GraphArray<GraphDoubleInterval> intv;
    if ( points.size() <= 0 )
    {
        GraphDoubleRect cRect(0, 0, 2 * radius, 2* radius);
        cRect.moveCenter(pos);
        if ( contains(cRect) )
            intv += GraphDoubleInterval(0.0, 2 * M_PI);
    }
    else
    {
        QList<double> angles;
        for ( int i = 0; i < points.size(); i++ )
            angles += toAngle(pos, points[i]);
        qSort(angles);

        const int in = contains(graphPolar2Pos(pos, radius,
            angles[0] + (angles[1] - angles[0]) / 2));
        if ( in )
        {
            for ( int i = 0; i < angles.size() - 1; i += 2)
                intv += GraphDoubleInterval(angles[i], angles[i+1]);
        }
        else
        {
            for ( int i = 1; i < angles.size() - 1; i += 2)
                intv += GraphDoubleInterval(angles[i], angles[i+1]);
            intv += GraphDoubleInterval(angles.last(), angles.first());
        }
    }

    return intv;
}

double GraphCircleClipper::toAngle(
    const GraphDoublePoint &from, const GraphDoublePoint &to) const
{
    if ( from.x() == to.x() )
        return from.y() <= to.y() ? M_PI / 2.0 : 3 * M_PI / 2.0;

    const double m = qAbs((to.y() - from.y()) / (to.x() - from.x()) );

    double angle = ::atan(m);
    if ( to.x() > from.x() )
    {   
        if ( to.y() > from.y() )
            angle = 2 * M_PI - angle;
    }
    else
    {
        if ( to.y() > from.y() )
            angle = M_PI + angle;
        else
            angle = M_PI - angle;
    }

    return angle;
}

QList<GraphDoublePoint> GraphCircleClipper::cuttingPoints(
    Edge edge, const GraphDoublePoint &pos, double radius) const
{
    QList<GraphDoublePoint> points;

    if ( edge == Left || edge == Right )
    {
        const double x = (edge == Left) ? left() : right();
        if ( qAbs(pos.x() - x) < radius )
        {
            const double off = ::sqrt(graphSqr(radius) - graphSqr(pos.x() - x));
            const double y1 = pos.y() + off;
            if ( y1 >= top() && y1 <= bottom() )
                points += GraphDoublePoint(x, y1);
            const double y2 = pos.y() - off;
            if ( y2 >= top() && y2 <= bottom() )
                points += GraphDoublePoint(x, y2);
        }
    }
    else
    {
        const double y = (edge == Top) ? top() : bottom();
        if ( qAbs(pos.y() - y) < radius )
        {
            const double off = ::sqrt(graphSqr(radius) - graphSqr(pos.y() - y));
            const double x1 = pos.x() + off;
            if ( x1 >= left() && x1 <= right() )
                points += GraphDoublePoint(x1, y);
            const double x2 = pos.x() - off;
            if ( x2 >= left() && x2 <= right() )
                points += GraphDoublePoint(x2, y);
        }
    }
    return points;
}
    
/*! 
   Sutherland-Hodgman polygon clipping

   \param clipRect Clip rectangle
   \param polygon Polygon

   \return Clipped polygon
*/
QPolygon GraphClipper::clipPolygon(
    const QRect &clipRect, const QPolygon &polygon)
{
    GraphPolygonClipper clipper(clipRect);
    return clipper.clipPolygon(polygon);
}

/*! 
   Sutherland-Hodgman polygon clipping

   \param clipRect Clip rectangle
   \param polygon Polygon

   \return Clipped polygon
*/
QPolygonF GraphClipper::clipPolygonF(
    const GraphDoubleRect &clipRect, const QPolygonF &polygon)
{
    GraphPolygonClipperF clipper(clipRect);
    return clipper.clipPolygon(polygon);
}

/*! 
   Circle clipping

   clipCircle() devides a circle into intervals of angles representing arcs
   of the circle. When the circle is completely inside the clip rectangle 
   an interval [0.0, 2 * M_PI] is returned.

   \param clipRect Clip rectangle
   \param center Center of the circle
   \param radius Radius of the circle

   \return Arcs of the circle
*/
GraphArray<GraphDoubleInterval> GraphClipper::clipCircle(
    const GraphDoubleRect &clipRect, 
    const GraphDoublePoint &center, double radius)
{
    GraphCircleClipper clipper(clipRect);
    return clipper.clipCircle(center, radius);
}

