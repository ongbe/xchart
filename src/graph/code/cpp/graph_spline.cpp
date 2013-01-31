
#include "graph_spline.h"
#include "graph_math.h"
#include "graph_array.h"

using namespace GraphLib;

class GraphSpline::PrivateData
{
public:
    PrivateData():
        splineType(GraphSpline::Natural)
    {
    }

    GraphSpline::SplineType splineType;

    // coefficient vectors
    GraphArray<double> a;
    GraphArray<double> b;
    GraphArray<double> c;

    // control points
    QPolygonF points;
};

static int lookup(double x, const QPolygonF &values)
{
#if 0
//qLowerBiund/qHigherBound ???
#endif
    int i1;
    const int size = (int)values.size();
    
    if (x <= values[0].x())
       i1 = 0;
    else if (x >= values[size - 2].x())
       i1 = size - 2;
    else
    {
        i1 = 0;
        int i2 = size - 2;
        int i3 = 0;

        while ( i2 - i1 > 1 )
        {
            i3 = i1 + ((i2 - i1) >> 1);

            if (values[i3].x() > x)
               i2 = i3;
            else
               i1 = i3;
        }
    }
    return i1;
}

//! Constructor
GraphSpline::GraphSpline()
{
    d_data = new PrivateData;
}

/*! 
   Copy constructor
   \param other Spline used for initilization
*/
GraphSpline::GraphSpline(const GraphSpline& other)
{
    d_data = new PrivateData(*other.d_data);
}

/*! 
   Assignment operator
   \param other Spline used for initilization
*/
GraphSpline &GraphSpline::operator=( const GraphSpline &other)
{
    *d_data = *other.d_data;
    return *this;
}

//! Destructor
GraphSpline::~GraphSpline()
{
    delete d_data;
}

/*!
   Select the algorithm used for calculating the spline

   \param splineType Spline type
   \sa splineType()
*/
void GraphSpline::setSplineType(SplineType splineType)
{
    d_data->splineType = splineType;
}

/*!
   \return the spline type
   \sa setSplineType()
*/
GraphSpline::SplineType GraphSpline::splineType() const
{
    return d_data->splineType;
}

/*!
  \brief Calculate the spline coefficients

  Depending on the value of \a periodic, this function
  will determine the coefficients for a natural or a periodic
  spline and store them internally. 
  
  \param points Points
  \return true if successful
  \warning The sequence of x (but not y) values has to be strictly monotone
           increasing, which means <code>points[i].x() < points[i+1].x()</code>.
       If this is not the case, the function will return false
*/
bool GraphSpline::setPoints(const QPolygonF& points)
{
    const int size = points.size();
    if (size <= 2) 
    {
        reset();
        return false;
    }

    d_data->points = points;
    
    d_data->a.resize(size-1);
    d_data->b.resize(size-1);
    d_data->c.resize(size-1);

    bool ok;
    if ( d_data->splineType == Periodic )
        ok = buildPeriodicSpline(points);
    else
        ok = buildNaturalSpline(points);

    if (!ok) 
        reset();

    return ok;
}

/*!
   Return points passed by setPoints
*/
QPolygonF GraphSpline::points() const
{
    return d_data->points;
}

//! \return A coefficients
const GraphArray<double> &GraphSpline::coefficientsA() const
{
    return d_data->a;
}

//! \return B coefficients
const GraphArray<double> &GraphSpline::coefficientsB() const
{
    return d_data->b;
}

//! \return C coefficients
const GraphArray<double> &GraphSpline::coefficientsC() const
{
    return d_data->c;
}


//! Free allocated memory and set size to 0
void GraphSpline::reset()
{
    d_data->a.resize(0);
    d_data->b.resize(0);
    d_data->c.resize(0);
    d_data->points.resize(0);
}

//! True if valid
bool GraphSpline::isValid() const
{
    return d_data->a.size() > 0;
}

/*!
  Calculate the interpolated function value corresponding 
  to a given argument x.
*/
double GraphSpline::value(double x) const
{
    if (d_data->a.size() == 0)
        return 0.0;

    const int i = lookup(x, d_data->points);

    const double delta = x - d_data->points[i].x();
    return( ( ( ( d_data->a[i] * delta) + d_data->b[i] ) 
        * delta + d_data->c[i] ) * delta + d_data->points[i].y() );
}

/*!
  \brief Determines the coefficients for a natural spline
  \return true if successful
*/
bool GraphSpline::buildNaturalSpline(const QPolygonF &points)
{
    int i;
    
    const QPointF *p = points.data();
    const int size = points.size();

    double *a = d_data->a.data();
    double *b = d_data->b.data();
    double *c = d_data->c.data();

    //  set up tridiagonal equation system; use coefficient
    //  vectors as temporary buffers
    GraphArray<double> h(size-1);
    for (i = 0; i < size - 1; i++) 
    {
        h[i] = p[i+1].x() - p[i].x();
        if (h[i] <= 0)
            return false;
    }
    
    GraphArray<double> d(size-1);
    double dy1 = (p[1].y() - p[0].y()) / h[0];
    for (i = 1; i < size - 1; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (h[i-1] + h[i]);

        const double dy2 = (p[i+1].y() - p[i].y()) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
    }

    //
    // solve it
    //
    
    // L-U Factorization
    for(i = 1; i < size - 2;i++)
    {
        c[i] /= a[i];
        a[i+1] -= b[i] * c[i]; 
    }

    // forward elimination
    GraphArray<double> s(size);
    s[1] = d[1];
    for ( i = 2; i < size - 1; i++)
       s[i] = d[i] - c[i-1] * s[i-1];
    
    // backward elimination
    s[size - 2] = - s[size - 2] / a[size - 2];
    for (i = size -3; i > 0; i--)
       s[i] = - (s[i] + b[i] * s[i+1]) / a[i];
    s[size - 1] = s[0] = 0.0;

    //
    // Finally, determine the spline coefficients
    //
    for (i = 0; i < size - 1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( p[i+1].y() - p[i].y() ) / h[i] 
            - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0; 
    }

    return true;
}

/*!
  \brief Determines the coefficients for a periodic spline
  \return true if successful
*/
bool GraphSpline::buildPeriodicSpline(const QPolygonF &points)
{
    int i;
    
    const QPointF *p = points.data();
    const int size = points.size();

    double *a = d_data->a.data();
    double *b = d_data->b.data();
    double *c = d_data->c.data();

    GraphArray<double> d(size-1);
    GraphArray<double> h(size-1);
    GraphArray<double> s(size);
    
    //
    //  setup equation system; use coefficient
    //  vectors as temporary buffers
    //
    for (i = 0; i < size - 1; i++)
    {
        h[i] = p[i+1].x() - p[i].x();
        if (h[i] <= 0.0)
            return false;
    }
    
    const int imax = size - 2;
    double htmp = h[imax];
    double dy1 = (p[0].y() - p[imax].y()) / htmp;
    for (i = 0; i <= imax; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (htmp + h[i]);
        const double dy2 = (p[i+1].y() - p[i].y()) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
        htmp = h[i];
    }

    //
    // solve it
    //
    
    // L-U Factorization
    a[0] = sqrt(a[0]);
    c[0] = h[imax] / a[0];
    double sum = 0;

    for( i = 0; i < imax - 1; i++)
    {
        b[i] /= a[i];
        if (i > 0)
           c[i] = - c[i-1] * b[i-1] / a[i];
        a[i+1] = sqrt( a[i+1] - graphSqr(b[i]));
        sum += graphSqr(c[i]);
    }
    b[imax-1] = (b[imax-1] - c[imax-2] * b[imax-2]) / a[imax-1];
    a[imax] = sqrt(a[imax] - graphSqr(b[imax-1]) - sum);
    

    // forward elimination
    s[0] = d[0] / a[0];
    sum = 0;
    for( i = 1; i < imax; i++)
    {
        s[i] = (d[i] - b[i-1] * s[i-1]) / a[i];
        sum += c[i-1] * s[i-1];
    }
    s[imax] = (d[imax] - b[imax-1] * s[imax-1] - sum) / a[imax];
    
    
    // backward elimination
    s[imax] = - s[imax] / a[imax];
    s[imax-1] = -(s[imax-1] + b[imax-1] * s[imax]) / a[imax-1];
    for (i= imax - 2; i >= 0; i--)
       s[i] = - (s[i] + b[i] * s[i+1] + c[i] * s[imax]) / a[i];

    //
    // Finally, determine the spline coefficients
    //
    s[size-1] = s[0];
    for ( i=0; i < size-1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( p[i+1].y() - p[i].y() ) 
            / h[i] - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0; 
    }

    return true;
}

