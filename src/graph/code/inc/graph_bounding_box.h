#ifndef GRAPH_BOUNDING_BOX_H
#define GRAPH_BOUNDING_BOX_H

#include "graph_geometry.h"

namespace GraphLib {

struct PolyBBExtras
{
    double startLong, startTrans;
    double middleTrans;
    double endLong, endTrans;
};

struct LineBBExtras
{
    double startLong, startTrans;
    double endLong, endTrans;
};

struct ElementBBExtras {
    double borderTrans;
} ;

void bicubicbezier2D_bbox(const Point *p0,const Point *p1,
                          const Point *p2,const Point *p3,
                          const PolyBBExtras *extra,
                          Rectangle *rect);
void polyline_bbox(const Point *pts, int numpoints,
                   const PolyBBExtras *extra, bool closed,
                   Rectangle *rect);
void polybezier_bbox(const BezPoint *pts, int numpoints,
                     const PolyBBExtras *extra, bool closed,
                     Rectangle *rect);
void line_bbox(const Point *p1, const Point *p2,
               const LineBBExtras *extra,
               Rectangle *rect);

void add_arrow_rectangle(Rectangle *rect,
                         const Point *vertex,
                         const Point *normed_dir,
                         double extra_long,double extra_trans);
int bicubicbezier_extrema(const double p[4],double u[2]);
void bernstein_develop(const double p[4], double *A,
                       double *B, double *C, double *D);
double bezier_eval(const double p[4], double u);
double bezier_eval_tangent(const double p[4], double u);

}

#endif // GRAPH_BOUNDING_BOX_H
