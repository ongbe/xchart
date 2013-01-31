
#include "graph_math.h"
#include "graph_spline.h"
#include "graph_curve_fitter.h"

using namespace GraphLib;

//! Constructor
GraphCurveFitter::GraphCurveFitter()
{
}

//! Destructor
GraphCurveFitter::~GraphCurveFitter()
{
}

class GraphSplineCurveFitter::PrivateData
{
public:
    PrivateData():
        fitMode(GraphSplineCurveFitter::Auto),
        splineSize(250)
    {
    }

    GraphSpline spline;
    GraphSplineCurveFitter::FitMode fitMode;
    int splineSize;
};

//! Constructor
GraphSplineCurveFitter::GraphSplineCurveFitter()
{
    d_data = new PrivateData;
}

//! Destructor
GraphSplineCurveFitter::~GraphSplineCurveFitter()
{
    delete d_data;
}

/*!
  Select the algorithm used for building the spline

  \param mode Mode representing a spline algorithm
  \sa fitMode()
*/
void GraphSplineCurveFitter::setFitMode(FitMode mode)
{
    d_data->fitMode = mode;
}

/*!
  \return Mode representing a spline algorithm
  \sa setFitMode()
*/
GraphSplineCurveFitter::FitMode GraphSplineCurveFitter::fitMode() const
{
    return d_data->fitMode;
}

void GraphSplineCurveFitter::setSpline(const GraphSpline &spline)
{
    d_data->spline = spline;
    d_data->spline.reset();
}

const GraphSpline &GraphSplineCurveFitter::spline() const
{
    return d_data->spline;
}

GraphSpline &GraphSplineCurveFitter::spline() 
{
    return d_data->spline;
}

/*!
   Assign a spline size ( has to be at least 10 points )

   \param splineSize Spline size
   \sa splineSize()
*/
void GraphSplineCurveFitter::setSplineSize(int splineSize)
{
    d_data->splineSize = qMax(splineSize, 10);
}

/*! 
  \return Spline size
  \sa setSplineSize()
*/
int GraphSplineCurveFitter::splineSize() const
{
    return d_data->splineSize;
}

/*!
  Find a curve which has the best fit to a series of data points

  \param points Series of data points
  \return Curve points
*/
QPolygonF GraphSplineCurveFitter::fitCurve(const QPolygonF &points) const
{
    const int size = (int)points.size();
    if ( size <= 2 )
        return points;

    FitMode fitMode = d_data->fitMode;
    if ( fitMode == Auto )
    {
        fitMode = Spline;

        const GraphDoublePoint *p = points.data();
        for ( int i = 1; i < size; i++ )
        {
            if ( p[i].x() <= p[i-1].x() )
            {
                fitMode = ParametricSpline;
                break;
            }
        };
    }

    if ( fitMode == ParametricSpline )
        return fitParametric(points);
    else
        return fitSpline(points);
}

QPolygonF GraphSplineCurveFitter::fitSpline(
    const QPolygonF &points) const
{
    d_data->spline.setPoints(points);
    if ( !d_data->spline.isValid() )
        return points;

    QPolygonF fittedPoints(d_data->splineSize);

    const double x1 = points[0].x();
    const double x2 = points[int(points.size() - 1)].x();
    const double dx = x2 - x1;
    const double delta = dx / (d_data->splineSize - 1);

    for (int i = 0; i < d_data->splineSize; i++)
    {
        GraphDoublePoint &p = fittedPoints[i];

        const double v = x1 + i * delta;
        const double sv = d_data->spline.value(v);

        p.setX(qRound(v));
        p.setY(qRound(sv));
    }
    d_data->spline.reset();

    return fittedPoints;
}

QPolygonF GraphSplineCurveFitter::fitParametric(
    const QPolygonF &points) const
{
    int i;
    const int size = points.size();

    QPolygonF fittedPoints(d_data->splineSize);
    QPolygonF splinePointsX(size);
    QPolygonF splinePointsY(size);

    const GraphDoublePoint *p = points.data();
    GraphDoublePoint *spX = splinePointsX.data();
    GraphDoublePoint *spY = splinePointsY.data();

    double param = 0.0;
    for (i = 0; i < size; i++)
    {
        const double x = p[i].x();
        const double y = p[i].y();
        if ( i > 0 )
        {
            const double delta = sqrt( graphSqr(x - spX[i-1].y())
                      + graphSqr( y - spY[i-1].y() ) );
            param += qMax(delta, 1.0);
        }
        spX[i].setX(param);
        spX[i].setY(x);
        spY[i].setX(param);
        spY[i].setY(y);
    }

    d_data->spline.setPoints(splinePointsX);
    if ( !d_data->spline.isValid() )
        return points;

    const double deltaX =
        splinePointsX[size - 1].x() / (d_data->splineSize-1);
    for (i = 0; i < d_data->splineSize; i++)
    {
        const double dtmp = i * deltaX;
        fittedPoints[i].setX(qRound(d_data->spline.value(dtmp)));
    }

    d_data->spline.setPoints(splinePointsY);
    if ( !d_data->spline.isValid() )
        return points;

    const double deltaY =
        splinePointsY[size - 1].x() / (d_data->splineSize-1);
    for (i = 0; i < d_data->splineSize; i++)
    {
        const double dtmp = i * deltaY;
        fittedPoints[i].setY(qRound(d_data->spline.value(dtmp)));
    }

    return fittedPoints;
}

