
#ifndef _GRAPH_GEOMETRY_H_
#define _GRAPH_GEOMETRY_H_

#include <QRectF>
#include <QRect>
#include <QPoint>
#include <QPointF>

namespace GraphLib 
{
    typedef double real;
    typedef real coord;

    typedef QPointF Point;
    typedef QRect IntRectangle;
    typedef QRectF Rectangle;

    typedef Qt::PenCapStyle LineCaps;
    typedef Qt::PenJoinStyle LineJoin;
    typedef Qt::PenStyle LineStyle;
    typedef Qt::BrushStyle FillStyle;
    typedef Qt::Alignment Alignment;

    struct BezPoint {
      enum {
        BEZ_MOVE_TO, /*!< move to point p1 */
        BEZ_LINE_TO, /*!< line to point p1 */
        BEZ_CURVE_TO /*!< curve to point p3 using p1 and p2 as control points */
      }type;

      Point p1; /*!< main point in case of move or line-to, otherwise first control point */
      Point p2; /*!< second control point  */
      Point p3; /*!< main point for 'true' bezier point */
    };

    void point_sub(Point *p1, const Point *p2);
    void point_add(Point *p1, const Point *p2);
    void point_scale(Point *p, double alpha);
    double point_dot(const Point *p1, const Point *p2);

    void point_copy_add_scaled(Point *dst, const Point *src,
                const Point *vct, double alpha);
    void point_normalize(Point *p);
    void rectangle_add_point(Rectangle *r, const Point *p);
    void point_add_scaled(Point *dst, const Point *src, double alpha);

    void point_get_perp(Point *dst, const Point *src);

    double distance_rectangle_point(const Rectangle *rect, const Point *point);
    double distance_ellipse_point(const Point *centre,
                                  double width,
                                  double height,
                                  double line_width,
                                  const Point *point);
    double distance_line_point(const Point *line_start,
                               const Point *line_end,
                               int line_width,
                               const Point *point);
    double distance_polygon_point(const Point *poly,
                                  int npoints,
                                  double line_width,
                                  const Point *point);
    double distance_point_point(const Point *p1, const Point *p2);

    double line_crosses_ray(const Point *line_start,
                         const Point *line_end,
                         const Point *rayend);
}

#endif
