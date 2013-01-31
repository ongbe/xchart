
#ifndef GRAPH_SPLINE_H
#define GRAPH_SPLINE_H

#include "graph_global.h"
#include "graph_double_rect.h"

#include <QPolygonF>


    // MOC_SKIP_BEGIN
    // MOC_SKIP_END

    /*!
      \brief A class for spline interpolation

      The GraphSpline class is used for cubical spline interpolation.
      Two types of splines, natural and periodic, are supported.

      \par Usage:
      <ol>
      <li>First call setPoints() to determine the spline coefficients 
      for a tabulated function y(x).
      <li>After the coefficients have been set up, the interpolated
      function value for an argument x can be determined by calling 
      GraphSpline::value().
      </ol>

      \par Example:
      \code
#include <graph_spline.h>

QPolygonF interpolate(const QPolygonF& points, int numValues)
{
GraphSpline spline;
if ( !spline.setPoints(points) ) 
return points;

QPolygonF interpolatedPoints(numValues);

const double delta = 
(points[numPoints - 1].x() - points[0].x()) / (points.size() - 1);
for(i = 0; i < points.size(); i++)  / interpolate
{
const double x = points[0].x() + i * delta;
interpolatedPoints[i].setX(x);
interpolatedPoints[i].setY(spline.value(x));
}
return interpolatedPoints;
}
\endcode
     */

namespace GraphLib
{
    class  GraphSpline
    {
        public:
            //! Spline type
            enum SplineType
            {
                Natural,
                Periodic
            };

            GraphSpline();
            GraphSpline( const GraphSpline & );

            ~GraphSpline();

            GraphSpline &operator=( const GraphSpline & );

            void setSplineType(SplineType);
            SplineType splineType() const;

            bool setPoints(const QPolygonF& points);
            QPolygonF points() const;

            void reset();

            bool isValid() const;
            double value(double x) const;

            const GraphArray<double> &coefficientsA() const;
            const GraphArray<double> &coefficientsB() const;
            const GraphArray<double> &coefficientsC() const;

        protected:

            bool buildNaturalSpline(const QPolygonF &);
            bool buildPeriodicSpline(const QPolygonF &);

            class PrivateData;
            PrivateData *d_data;
    };

}
#endif

