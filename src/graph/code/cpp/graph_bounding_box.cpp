#include "graph_bounding_box.h"
#include "graph_geometry.h"

#include <math.h>

namespace GraphLib
{

int bicubicbezier_extrema(const double p[4],double u[2])
{
    double A,B,C,D,delta;

    bernstein_develop(p,&A,&B,&C,&D);
    delta = 4*B*B - 12*A*C;

    u[0] = u[1] = 0.0;
    if (delta<0) return 0;

    u[0] = (-2*B + sqrt(delta)) / (6*A);
    if (delta==0) return 1;
    u[1] = (-2*B - sqrt(delta)) / (6*A);
    return 2;
}

void bernstein_develop(const double p[4], double *A, double *B, double *C, double *D)
{
    *A = -p[0]+3*p[1]-3*p[2]+p[3];
    *B = 3*p[0]-6*p[1]+3*p[2];
    *C = 3*p[1]-3*p[0];
    *D = p[0];
}

double bezier_eval(const double p[4], double u)
{
    double A,B,C,D;
    bernstein_develop(p,&A,&B,&C,&D);
    return A*u*u*u+B*u*u+C*u+D;
}

double bezier_eval_tangent(const double p[4], double u)
{
    double A,B,C,D;
    bernstein_develop(p,&A,&B,&C,&D);
    return 3*A*u*u+2*B*u+C;
}

void bicubicbezier2D_bbox(const Point *p0,const Point *p1,
                          const Point *p2,const Point *p3,
                          const PolyBBExtras *extra,
                          Rectangle *rect)
{
    double x[4],y[4];
    Point vl,vt,p,tt;
    double *xy;
    int i,extr;
    double u[2];

    rect->setLeft(p0->x());
    rect->setRight(p0->x());
    rect->setTop(p0->y());
    rect->setBottom(p0->y());

    rectangle_add_point(rect,p3);

    point_copy_add_scaled(&vl,p0,p1,-1);
    point_normalize(&vl);

    add_arrow_rectangle(rect,p0,&vl,extra->startLong,
            qMax(extra->startTrans, extra->middleTrans));

    point_copy_add_scaled(&vl,p3,p2,-1);
    point_normalize(&vl);

    add_arrow_rectangle(rect,p3,&vl,extra->endLong,
            qMax(extra->endTrans, extra->middleTrans));

    x[0] = p0->x(); x[1] = p1->x(); x[2] = p2->x(); x[3] = p3->x();
    y[0] = p0->y(); y[1] = p1->y(); y[2] = p2->y(); y[3] = p3->y();

    for (xy = x; xy ; xy=(xy==x?y:NULL) ) {
        extr = bicubicbezier_extrema(xy,u);
        for (i=0;i<extr;i++) {
            if ((u[i]<0) || (u[i]>1)) continue;
            p.setX(bezier_eval(x,u[i]));
            vl.setX(bezier_eval_tangent(x,u[i]));
            p.setY(bezier_eval(y,u[i]));
            vl.setY(bezier_eval_tangent(y,u[i]));
            point_normalize(&vl);
            point_get_perp(&vt,&vl);

            point_copy_add_scaled(&tt,&p,&vt,extra->middleTrans);
            rectangle_add_point(rect,&tt);
            point_copy_add_scaled(&tt,&p,&vt,-extra->middleTrans);
            rectangle_add_point(rect,&tt);
        }
    }
}

void polyline_bbox(const Point *pts, int numpoints,
                   const PolyBBExtras *extra, bool closed,
                   Rectangle *rect)
{
    int i;
    BezPoint bpts[numpoints+1];

    bpts[0].type = BezPoint::BEZ_MOVE_TO;
    bpts[0].p1 = pts[0];

    for (i=1;i<numpoints;i++) {
        bpts[i].type = BezPoint::BEZ_LINE_TO;
        bpts[i].p1 = pts[i];
    }
    polybezier_bbox(bpts,numpoints + (closed?1:0),extra,closed,rect);
}

void polybezier_bbox(const BezPoint *pts, int numpoints,
                     const PolyBBExtras *extra, bool closed,
                     Rectangle *rect)
{
    Point vx,vp,vn,vsc;
    int i,prev,next;
    Rectangle rt;
    PolyBBExtras bextra,start_bextra,end_bextra;
    LineBBExtras lextra,start_lextra,end_lextra,full_lextra;
    bool start,end;

    rect->setLeft(pts[0].p1.x());
    rect->setRight(pts[0].p1.x());

    rect->setTop(pts[0].p1.y());
    rect->setBottom(pts[0].p1.y());

    if (!closed) {
        start_lextra.startLong = extra->startLong;
        start_lextra.startTrans = qMax(extra->startTrans,extra->middleTrans);
        start_lextra.endLong = 0;
        start_lextra.endTrans = extra->middleTrans;

        end_lextra.startLong = 0;
        end_lextra.startTrans = extra->middleTrans;
        end_lextra.endLong = extra->endLong;
        end_lextra.endTrans = qMax(extra->endTrans,extra->middleTrans);
    }

    full_lextra.startLong = extra->startLong;
    full_lextra.startTrans = qMax(extra->startTrans,extra->middleTrans);
    full_lextra.endLong = extra->endLong;
    full_lextra.endTrans = qMax(extra->endTrans,extra->middleTrans);

    if (!closed) {
        lextra.startLong = 0;
        lextra.startTrans = extra->middleTrans;
        lextra.endLong = 0;
        lextra.endTrans = extra->middleTrans;

        start_bextra.startLong = extra->startLong;
        start_bextra.startTrans = extra->startTrans;
        start_bextra.middleTrans = extra->middleTrans;
        start_bextra.endLong = 0;
        start_bextra.endTrans = extra->middleTrans;

        end_bextra.startLong = 0;
        end_bextra.startTrans = extra->middleTrans;
        end_bextra.middleTrans = extra->middleTrans;
        end_bextra.endLong = extra->endLong;
        end_bextra.endTrans = extra->endTrans;
    }

    bextra.startLong = 0;
    bextra.startTrans = extra->middleTrans;
    bextra.middleTrans = extra->middleTrans;
    bextra.endLong = 0;
    bextra.endTrans = extra->middleTrans;


    for (i=1;i<numpoints;i++) {
        next = (i+1) % numpoints;
        prev = (i-1) % numpoints;
        if (closed && (next == 0)) next=1;
        if (closed && (prev == 0)) prev=numpoints-1;

        if (pts[i].type == BezPoint::BEZ_MOVE_TO) {
            continue;
        }

        switch(pts[i].type) {
            case BezPoint::BEZ_MOVE_TO:
                break;
            case BezPoint::BEZ_LINE_TO:
                vx = pts[i].p1;
                switch(pts[prev].type) {
                    case BezPoint::BEZ_MOVE_TO:
                    case BezPoint::BEZ_LINE_TO:
                        vsc = pts[prev].p1;
                        vp = pts[prev].p1;
                        break;
                    case BezPoint::BEZ_CURVE_TO:
                        vsc = pts[prev].p3;
                        vp = pts[prev].p3;
                        break;
                }
                break;
            case BezPoint::BEZ_CURVE_TO:
                vx = pts[i].p3;
                vp = pts[i].p2;
                switch(pts[prev].type) {
                    case BezPoint::BEZ_MOVE_TO:
                    case BezPoint::BEZ_LINE_TO:
                        vsc = pts[prev].p1;
                        break;
                    case BezPoint::BEZ_CURVE_TO:
                        vsc = pts[prev].p3;
                        break;
                }

                break;
        }
        start = (pts[prev].type == BezPoint::BEZ_MOVE_TO);
        end = (pts[next].type == BezPoint::BEZ_MOVE_TO);
        vn = pts[next].p1;

        if (closed) {
            if (pts[i].type == BezPoint::BEZ_LINE_TO) {
                line_bbox(&vsc,&vx,&full_lextra,&rt);
            } else {
                bicubicbezier2D_bbox(&vsc,
                        &pts[i].p1,&pts[i].p2,&pts[i].p3,
                        &bextra,
                        &rt);
            }
        } else if (start) {
            if (pts[i].type == BezPoint::BEZ_LINE_TO) {
                if (end) {
                    line_bbox(&vsc,&vx,&full_lextra,&rt);
                } else {
                    line_bbox(&vsc,&vx,&start_lextra,&rt);
                }
            } else {
                if (end) {
                    bicubicbezier2D_bbox(&vsc,
                            &pts[i].p1,&pts[i].p2,&pts[i].p3,
                            extra,
                            &rt);
                } else {
                    bicubicbezier2D_bbox(&vsc,
                            &pts[i].p1,&pts[i].p2,&pts[i].p3,
                            &start_bextra,
                            &rt);
                }
            }
        } else if (end) {
            if (pts[i].type == BezPoint::BEZ_LINE_TO) {
                line_bbox(&vsc,&vx,&end_lextra,&rt);
            } else {
                bicubicbezier2D_bbox(&vsc,
                        &pts[i].p1,&pts[i].p2,&pts[i].p3,
                        &end_bextra,
                        &rt);
            }
        } else {
            if (pts[i].type == BezPoint::BEZ_LINE_TO) {
                line_bbox(&vsc,&vx,&lextra,&rt);
            } else {
                bicubicbezier2D_bbox(&vsc,
                        &pts[i].p1,&pts[i].p2,&pts[i].p3,
                        &bextra,
                        &rt);
            }
        }
        *rect = rect->united(rt);

        if ((!start) && (!end)) {
            Point vpx,vxn;
            double co,alpha;

            point_copy_add_scaled(&vpx,&vx,&vp,-1);
            point_normalize(&vpx);
            point_copy_add_scaled(&vxn,&vn,&vx,-1);
            point_normalize(&vxn);

            co = point_dot(&vpx,&vxn);
            alpha = acos(-co);
            if ((co > -0.9816) && (finite(alpha))) {
                double overshoot = extra->middleTrans / sin(alpha/2.0);
                Point vovs,pto;

                point_copy_add_scaled(&vovs,&vpx,&vxn,-1);
                point_normalize(&vovs);
                point_copy_add_scaled(&pto,&vx,&vovs,overshoot);

                rectangle_add_point(rect,&pto);
            } else {
                Point vpxt,vxnt,tmp;

                point_get_perp(&vpxt,&vpx);
                point_get_perp(&vxnt,&vxn);

                point_copy_add_scaled(&tmp,&vx,&vpxt,1);
                rectangle_add_point(rect,&tmp);
                point_copy_add_scaled(&tmp,&vx,&vpxt,-1);
                rectangle_add_point(rect,&tmp);
                point_copy_add_scaled(&tmp,&vx,&vxnt,1);
                rectangle_add_point(rect,&tmp);
                point_copy_add_scaled(&tmp,&vx,&vxnt,-1);
                rectangle_add_point(rect,&tmp);
            }
        }
    }
}

void add_arrow_rectangle(Rectangle *rect,
                                const Point *vertex,
                                const Point *normed_dir,
                                double extra_long,double extra_trans)
{
    Point vl,vt,pt;
    vl = *normed_dir;

    point_get_perp(&vt,&vl);
    point_copy_add_scaled(&pt,vertex,&vl,extra_long);
    point_add_scaled(&pt,&vt,extra_trans);
    rectangle_add_point(rect,&pt);
    point_add_scaled(&pt,&vt,-2.0 * extra_trans);
    rectangle_add_point(rect,&pt);
    point_add_scaled(&pt,&vl,-2.0 * extra_long);
    rectangle_add_point(rect,&pt);
    point_add_scaled(&pt,&vt,2.0 * extra_trans);
    rectangle_add_point(rect,&pt);
}

void line_bbox(const Point *p1, const Point *p2,
        const LineBBExtras *extra,
        Rectangle *rect)
{
    Point vl;

    rect->setLeft(p1->x());
    rect->setRight(p1->x());
    rect->setTop(p1->y());
    rect->setBottom(p1->y());

    rectangle_add_point(rect,p2);

    point_copy_add_scaled(&vl,p1,p2,-1);
    point_normalize(&vl);
    add_arrow_rectangle(rect,p1,&vl,extra->startLong,extra->startTrans);
    point_scale(&vl,-1);
    add_arrow_rectangle(rect,p2,&vl,extra->endLong,extra->endTrans);
}

}
