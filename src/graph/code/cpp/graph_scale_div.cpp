#include "graph_scale_div.h"
#include "graph_math.h"
#include "graph_double_interval.h"

using namespace GraphLib;

//! Construct an invalid GraphScaleDiv instance.
GraphScaleDiv::GraphScaleDiv():
    d_lowerBound(0.0),
    d_upperBound(0.0),
    d_isValid(false)
{
}

/*! 
  Construct GraphScaleDiv instance.

  \param interval Interval
  \param ticks List of major, medium and minor ticks
*/
GraphScaleDiv::GraphScaleDiv(
        const GraphDoubleInterval &interval, 
        GraphValueList ticks[NTickTypes]):
    d_lowerBound(interval.minValue()),
    d_upperBound(interval.maxValue()),
    d_isValid(true)
{
    for ( int i = 0; i < NTickTypes; i++ )
        d_ticks[i] = ticks[i];
}

/*! 
  Construct GraphScaleDiv instance.

  \param lowerBound First interval limit
  \param upperBound Second interval limit
  \param ticks List of major, medium and minor ticks
*/
GraphScaleDiv::GraphScaleDiv(
        double lowerBound, double upperBound,
        GraphValueList ticks[NTickTypes]):
    d_lowerBound(lowerBound),
    d_upperBound(upperBound),
    d_isValid(true)
{
    for ( int i = 0; i < NTickTypes; i++ )
        d_ticks[i] = ticks[i];
}

/*!
   Change the interval
   \param interval Interval
*/
void GraphScaleDiv::setInterval(const GraphDoubleInterval &interval)
{
    setInterval(interval.minValue(), interval.maxValue());
}

/*!
  \brief Equality operator
  \return true if this instance is equal to other
*/
int GraphScaleDiv::operator==(const GraphScaleDiv &other) const
{
    if ( d_lowerBound != other.d_lowerBound ||
        d_upperBound != other.d_upperBound ||
        d_isValid != other.d_isValid )
    {
        return false;
    }

    for ( int i = 0; i < NTickTypes; i++ )
    {
        if ( d_ticks[i] != other.d_ticks[i] )
            return false;
    }

    return true;
}

/*!
  \brief Inequality
  \return true if this instance is not equal to s
*/
int GraphScaleDiv::operator!=(const GraphScaleDiv &s) const
{
    return (!(*this == s));
}

//! Invalidate the scale division
void GraphScaleDiv::invalidate()
{
    d_isValid = false;

    // detach arrays
    for ( int i = 0; i < NTickTypes; i++ )
        d_ticks[i].clear();

    d_lowerBound = d_upperBound = 0;
}

//! Check if the scale division is valid
bool GraphScaleDiv::isValid() const
{
    return d_isValid;
}

/*!
  Return if a value is between lowerBound() and upperBound()

  \param value Value
  \return true/false
*/ 
bool GraphScaleDiv::contains(double value) const
{
    if ( !d_isValid )
        return false;

    const double min = graphMin(d_lowerBound, d_upperBound);
    const double max = graphMax(d_lowerBound, d_upperBound);

    return value >= min && value <= max;
}

//! Invert the scale divison
void GraphScaleDiv::invert()
{
    qSwap(d_lowerBound, d_upperBound);

    for ( int i = 0; i < NTickTypes; i++ )
    {
        GraphValueList& ticks = d_ticks[i];

        const int size = ticks.count();
        const int size2 = size / 2;
 
        for (int i=0; i < size2; i++)
            qSwap(ticks[i], ticks[size - 1 - i]);
    }
}

/*!
    Assign ticks

   \param type MinorTick, MediumTick or MajorTick
   \param ticks Values of the tick positions
*/
void GraphScaleDiv::setTicks(int type, const GraphValueList &ticks)
{
    if ( type >= 0 || type < NTickTypes )
        d_ticks[type] = ticks;
}

/*!
   Return a list of ticks

   \param type MinorTick, MediumTick or MajorTick
*/
const GraphValueList &GraphScaleDiv::ticks(int type) const
{
    if ( type >= 0 || type < NTickTypes )
        return d_ticks[type];

    static GraphValueList noTicks;
    return noTicks;
}

