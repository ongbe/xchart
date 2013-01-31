
#ifndef GRAPH_MATH_H
#define GRAPH_MATH_H

#include <math.h>
#include <qpoint.h>
#include "graph_global.h"
#include "graph_double_rect.h"

namespace GraphLib 
{

#if QT_VERSION < 0x040000

#define graphMax QMAX
#define graphMin QMIN
#define graphAbs QABS

#else // QT_VERSION >= 0x040000

#define graphMax qMax
#define graphMin qMin
#define graphAbs qAbs

#endif

#ifndef LOG10_2
#define LOG10_2     0.30102999566398119802  /* log10(2) */
#endif

#ifndef LOG10_3
#define LOG10_3     0.47712125471966243540  /* log10(3) */
#endif

#ifndef LOG10_5
#define LOG10_5     0.69897000433601885749  /* log10(5) */
#endif

#ifndef M_2PI
#define M_2PI       6.28318530717958623200  /* 2 pi */
#endif

#ifndef LOG_MIN
    //! Mininum value for logarithmic scales
#define LOG_MIN 1.0e-100
#endif

#ifndef LOG_MAX
    //! Maximum value for logarithmic scales
#define LOG_MAX 1.0e100
#endif

#ifndef M_E
#define M_E            2.7182818284590452354   /* e */
#endif

#ifndef M_LOG2E
#define M_LOG2E 1.4426950408889634074   /* log_2 e */
#endif

#ifndef M_LOG10E
#define M_LOG10E    0.43429448190325182765  /* log_10 e */
#endif

#ifndef M_LN2
#define M_LN2       0.69314718055994530942  /* log_e 2 */
#endif

#ifndef M_LN10
#define M_LN10         2.30258509299404568402  /* log_e 10 */
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846  /* pi */
#endif

#ifndef M_PI_2
#define M_PI_2      1.57079632679489661923  /* pi/2 */
#endif

#ifndef M_PI_4
#define M_PI_4      0.78539816339744830962  /* pi/4 */
#endif

#ifndef M_1_PI
#define M_1_PI      0.31830988618379067154  /* 1/pi */
#endif

#ifndef M_2_PI
#define M_2_PI      0.63661977236758134308  /* 2/pi */
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI  1.12837916709551257390  /* 2/sqrt(pi) */
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880  /* sqrt(2) */
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2   0.70710678118654752440  /* 1/sqrt(2) */
#endif

    GRAPH_EXPORT double graphGetMin(const double *array, int size);
    GRAPH_EXPORT double graphGetMax(const double *array, int size);


    //! Return the sign 
    inline int graphSign(double x)
    {
        if (x > 0.0)
            return 1;
        else if (x < 0.0)
            return (-1);
        else
            return 0;
    }            

    //! Return the square of a number
    inline double graphSqr(const double x)
    {
        return x*x;
    }

    /*!
      \brief Limit a value to fit into a specified interval
      \param x Input value
      \param x1 First interval boundary
      \param x2 Second interval boundary  
     */
    template <class T>
        T graphLim(const T& x, const T& x1, const T& x2)
        {
            T rv;
            T xmin, xmax;

            xmin = graphMin(x1, x2);
            xmax = graphMax(x1, x2);

            if ( x < xmin )
                rv = xmin;
            else if ( x > xmax )
                rv = xmax;
            else
                rv = x;

            return rv;
        }

    inline QPoint graphPolar2Pos(const QPoint &pole,
            double radius, double angle)
    {
        const double x = pole.x() + radius * ::cos(angle);
        const double y = pole.y() - radius * ::sin(angle);

        return QPoint(qRound(x), qRound(y));
    }

    inline QPoint graphDegree2Pos(const QPoint &pole,
            double radius, double angle)
    {
        return graphPolar2Pos(pole, radius, angle / 180.0 * M_PI);
    }

    inline GraphDoublePoint graphPolar2Pos(const GraphDoublePoint &pole,
            double radius, double angle)
    {
        const double x = pole.x() + radius * ::cos(angle);
        const double y = pole.y() - radius * ::sin(angle);

        return QPoint(qRound(x), qRound(y));
    }

    inline GraphDoublePoint graphDegree2Pos(const GraphDoublePoint &pole,
            double radius, double angle)
    {
        return graphPolar2Pos(pole, radius, angle / 180.0 * M_PI);
    }

    //! Rounding of doubles, like qRound for integers
    inline double graphRound(double value)
    {
        return ::floor(value + 0.5); // MSVC has no ::round().
    }

}
#endif

