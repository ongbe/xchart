
#ifndef GRAPH_CURVE_FITTER_H
#define GRAPH_CURVE_FITTER_H

#include "graph_global.h"
#include "graph_double_rect.h"


#include <QPolygonF>

namespace GraphLib
{
    class GraphSpline;

    // MOC_SKIP_BEGIN

    // MOC_SKIP_END

    /*!
      \brief Abstract base class for a curve fitter
     */
    class GraphCurveFitter
    {
        public:
            virtual ~GraphCurveFitter();

            /*!
              Find a curve which has the best fit to a series of data points

              \param polygon Series of data points
              \return Curve points
             */
            virtual QPolygonF fitCurve(const QPolygonF &polygon) const = 0;

        protected:
            GraphCurveFitter();

        private:
            GraphCurveFitter( const GraphCurveFitter & );
            GraphCurveFitter &operator=( const GraphCurveFitter & );
    };

    /*!
      \brief A curve fitter using cubic splines
     */
    class GraphSplineCurveFitter: public GraphCurveFitter
    {
        public:
            enum FitMode
            {
                Auto,
                Spline,
                ParametricSpline
            };

            GraphSplineCurveFitter();
            virtual ~GraphSplineCurveFitter();

            void setFitMode(FitMode);
            FitMode fitMode() const;

            void setSpline(const GraphSpline&);
            const GraphSpline &spline() const;
            GraphSpline &spline();

            void setSplineSize(int size);
            int splineSize() const;

            virtual QPolygonF fitCurve(const QPolygonF &) const;

        private:
            QPolygonF fitSpline(const QPolygonF &) const;
            QPolygonF fitParametric(const QPolygonF &) const;

            class PrivateData;
            PrivateData *d_data;
    };

}
#endif

