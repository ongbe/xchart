
#include "graph_geometry.h"

#include <math.h>

namespace GraphLib 
{

    void point_sub(Point *p1, const Point *p2)
    {
        p1->setX( p1->x() - p2->x() );
        p1->setY( p1->y() - p2->y() );
    }

    void point_add(Point *p1, const Point *p2)
    {
        p1->setX( p1->x() + p2->x() );
        p1->setY( p1->y() + p2->y() );
    }

    double point_dot(const Point *p1, const Point *p2)
    {
        return p1->x()*p2->x() + p1->y()*p2->y();
    }

    void point_scale(Point *p, double alpha)
    {
        double x = p->x() * alpha;
        double y = p->y() * alpha;
        p->setX(x);
        p->setY(y);
    }

    void point_add_scaled(Point *dst, const Point *src, double alpha)
    {
        dst->setX(alpha * src->x());
        dst->setY(alpha * src->y());
    }

    void point_copy_add_scaled(Point *dst, const Point *src,
                const Point *vct, double alpha)
    {
        dst->setX(src->x() + (alpha * vct->x()));
        dst->setY(src->y() + (alpha * vct->y()));
    }

    void point_normalize(Point *p)
    {
        double len;
        len = sqrt(p->x() *p->x() + p->y() *p->y());

        if (len > 0.0) {
            double x = p->x() / len;
            double y = p->y() / len;
            p->setX(x);
            p->setY(y);
        } else {
            p->setX(0.0);
            p->setY(0.0);
        }
    }

    void rectangle_add_point(Rectangle *r, const Point *p)
    {
        if (p->x() < r->left())
            r->setLeft(p->x());
        else if (p->x() > r->right())
            r->setRight(p->x());

        if (p->y() < r->top())
            r->setTop(p->y());
        else if (p->y() > r->bottom())
            r->setBottom(p->y());
    }

    void point_get_perp(Point *dst, const Point *src)
    {
        dst->setY(src->x());
        dst->setX(-src->y());
    }

    double distance_rectangle_point(const Rectangle *rect, const Point *point)
    {
        double dx = 0.0;
        double dy = 0.0;

        if (point->x() < rect->left()) {
            dx = rect->left() - point->x();
        } else if (point->x() > rect->right()) {
            dx = point->x() - rect->right();
        }

        if (point->y() < rect->top()) {
            dy = rect->top() - point->y();
        } else if (point->y() > rect->bottom()) {
            dy = point->y() - rect->bottom();
        }

        return dx + dy;
    }

    double distance_ellipse_point(const Point *centre,
                                  double width,
                                  double height,
                                  double line_width,
                                  const Point *point)
    {
        double w2 = width * width;
        double h2 = height * height;
        double scale;
        double rad;
        double dist;

        Point pt = *point;

        point_sub(&pt, centre);

        pt.setX( pt.x() * pt.x() );
        pt.setY( pt.y() * pt.y() );

        scale = w2 * h2 / ( 4 * h2 * pt.x() + 4 * w2 * pt.y() );

        rad = sqrt( ( pt.x() + pt.y() ) * scale ) + line_width / 2;

        dist = sqrt( pt.x() + pt.y() );

        if (dist <= rad)
            return 0.0;

        return dist - rad;
    }

    double distance_line_point(const Point *line_start,
                               const Point *line_end,
                               int line_width,
                               const Point *point)
    {
        Point v1, v2;
        double v1_lensq;
        double projlen;
        double perp_dist;

        v1 = *line_end;
        point_sub(&v1,line_start);

        v2 = *point;
        point_sub(&v2, line_start);

        v1_lensq = point_dot(&v1,&v1);
        if (v1_lensq<0.000001) {
          return sqrt(point_dot(&v2,&v2));
        }

        projlen = point_dot(&v1,&v2) / v1_lensq;
        if (projlen<0.0) {
          return sqrt(point_dot(&v2,&v2));
        }

        if (projlen>1.0) {
          Point v3 = *point;
          point_sub(&v3,line_end);
          return sqrt(point_dot(&v3,&v3));
        }

        point_scale(&v1, projlen);
        point_sub(&v1, &v2);

        perp_dist = sqrt(point_dot(&v1,&v1));
        perp_dist -= line_width / 2.0;
        if (perp_dist < 0.0) {
          perp_dist = 0.0;
        }

        return perp_dist;
    }

    double distance_polygon_point(const Point *poly,
                                  int npoints,
                                  double lineWidth,
                                  const Point *point)
    {
        int i, last = npoints - 1;
    //    double line_dist = G_MAXFLOAT;
        double line_dist = 1.0;
        int crossings = 0;

        for (i = 0; i < npoints; i++) {
            double dist;

            crossings += line_crosses_ray(&poly[last], &poly[i], point);
            dist = distance_line_point(&poly[last], &poly[i], lineWidth, point);
            line_dist = qMin(line_dist, dist);
            last = i;
        }

        if (crossings % 2 == 1)
            return 0.0;
        else
            return line_dist;
    }

    double distance_point_point(const Point *p1, const Point *p2)
    {
        double dx = p1->x() - p2->x();
        double dy = p1->y() - p2->y();
        return sqrt(dx*dx + dy*dy);
    }

    double line_crosses_ray(const Point *lineStart,
                         const Point *lineEnd,
                         const Point *rayend)
    {
        double xpos;

        if (lineStart->y() > lineEnd->y()) {
            const Point *tmp;

            tmp = lineStart;
            lineStart = lineEnd;
            lineEnd = tmp;
        }

        if (lineStart->y() > rayend->y() || lineEnd->y() < rayend->y())
            return 0;

        if (lineEnd->y() - lineStart->y() < 0.00000000001) {
            return (lineEnd->y() - rayend->y() < 0.00000000001);
        }
        xpos = lineStart->x() + (rayend->y() - lineStart->y()) *
                (lineEnd->x() - lineStart->x()) / (lineEnd->y() - lineStart->y());

        return xpos <= rayend->x();
    }
}
