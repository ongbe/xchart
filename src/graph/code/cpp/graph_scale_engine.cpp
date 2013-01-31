
#include "graph_math.h"
#include "graph_scale_map.h"
#include "graph_scale_engine.h"


using namespace GraphLib;
static const double _eps = 1.0e-6;

/*!
  \brief Compare 2 values, relative to an interval

  Values are "equal", when :
  \f$\cdot value2 - value1 <= abs(intervalSize * 10e^{-6})\f$

  \param value1 First value to compare
  \param value2 Second value to compare
  \param intervalSize interval size

  \return 0: if equal, -1: if value2 > value1, 1: if value1 > value2
*/
int GraphScaleArithmetic::compareEps(double value1, double value2, 
    double intervalSize) 
{
    const double eps = qAbs(_eps * intervalSize);

    if ( value2 - value1 > eps )
        return -1;

    if ( value1 - value2 > eps )
        return 1;

    return 0;
}

/*!
  Ceil a value, relative to an interval

  \param value Value to ceil
  \param intervalSize Interval size
 
  \sa floorEps()
*/
double GraphScaleArithmetic::ceilEps(double value, 
    double intervalSize) 
{
    const double eps = _eps * intervalSize;

    value = (value - eps) / intervalSize;
    return ceil(value) * intervalSize;
}

/*!
  Floor a value, relative to an interval

  \param value Value to floor
  \param intervalSize Interval size
 
  \sa floorEps()
*/
double GraphScaleArithmetic::floorEps(double value, double intervalSize) 
{
    const double eps = _eps * intervalSize;

    value = (value + eps) / intervalSize;
    return floor(value) * intervalSize;
}

/*!
  \brief Divide an interval into steps

  \f$stepSize = (intervalSize - intervalSize * 10e^{-6}) / numSteps\f$

  \param intervalSize Interval size
  \param numSteps Number of steps
  \return Step size
*/
double GraphScaleArithmetic::divideEps(double intervalSize, double numSteps) 
{
    if ( numSteps == 0.0 || intervalSize == 0.0 )
        return 0.0;

    return (intervalSize - (_eps * intervalSize)) / numSteps;
} 

/*!
  Find the smallest value out of {1,2,5}*10^n with an integer number n
  which is greater than or equal to x
  
  \param x Input value
*/
double GraphScaleArithmetic::ceil125(double x) 
{
    if (x == 0.0) 
        return 0.0;

    const double sign = (x > 0) ? 1.0 : -1.0;
    const double lx = log10(fabs(x));
    const double p10 = floor(lx);
    
    double fr = pow(10.0, lx - p10);
    if (fr <=1.0)
       fr = 1.0; 
    else if (fr <= 2.0)
       fr = 2.0;
    else if (fr <= 5.0) 
       fr = 5.0;
    else
       fr = 10.0;

    return sign * fr * pow(10.0, p10);
}

/*!
  \brief Find the largest value out of {1,2,5}*10^n with an integer number n
  which is smaller than or equal to x

  \param x Input value
*/
double GraphScaleArithmetic::floor125(double x) 
{
    if (x == 0.0)
        return 0.0;

    double sign = (x > 0) ? 1.0 : -1.0;
    const double lx = log10(fabs(x));
    const double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr >= 10.0)
       fr = 10.0;
    else if (fr >= 5.0)
       fr = 5.0;
    else if (fr >= 2.0)
       fr = 2.0;
    else
       fr = 1.0;

    return sign * fr * pow(10.0, p10);
}

class GraphScaleEngine::PrivateData
{
public:
    PrivateData():
        attributes(GraphScaleEngine::NoAttribute),
        lowerMargin(0.0),
        upperMargin(0.0),
        referenceValue(0.0)
    {
    }

    int attributes;       // scale attributes

    double lowerMargin;      // margins
    double upperMargin;

    double referenceValue; // reference value

};

//! Constructor
GraphScaleEngine::GraphScaleEngine()
{
    d_data = new PrivateData;
}


//! Destructor
GraphScaleEngine::~GraphScaleEngine ()
{
    delete d_data;
}

/*!
    \return the margin at the lower end of the scale
    The default margin is 0.

    \sa setMargins()
*/
double GraphScaleEngine::lowerMargin() const 
{ 
    return d_data->lowerMargin; 
}

/*!
    \return the margin at the upper end of the scale
    The default margin is 0.

    \sa setMargins()
*/
double GraphScaleEngine::upperMargin() const 
{ 
    return d_data->upperMargin; 
}

/*!
  \brief Specify margins at the scale's endpoints
  \param lower minimum distance between the scale's lower boundary and the
             smallest enclosed value
  \param upper minimum distance between the scale's upper boundary and the
             greatest enclosed value

  Margins can be used to leave a minimum amount of space between
  the enclosed intervals and the boundaries of the scale.

  \warning
  \li GraphLog10ScaleEngine measures the margins in decades.

  \sa upperMargin(), lowerMargin()
*/

void GraphScaleEngine::setMargins(double lower, double upper)
{
    d_data->lowerMargin = qMax(lower, 0.0);
    d_data->upperMargin = qMax(upper, 0.0);
}

/*!
  Calculate a step size for an interval size

  \param intervalSize Interval size
  \param numSteps Number of steps
  
  \return Step size
*/
double GraphScaleEngine::divideInterval(
    double intervalSize, int numSteps) const
{
    if ( numSteps <= 0 )
        return 0.0;

    double v = GraphScaleArithmetic::divideEps(intervalSize, numSteps);
    return GraphScaleArithmetic::ceil125(v);
}

/*!
  Check if an interval "contains" a value

  \param interval Interval
  \param value Value

  \sa GraphScaleArithmetic::compareEps()
*/
bool GraphScaleEngine::contains(
    const GraphDoubleInterval &interval, double value) const
{
    if (!interval.isValid() )
        return false;
    
    if ( GraphScaleArithmetic::compareEps(value, 
        interval.minValue(), interval.width()) < 0 )
    {
        return false;
    }

    if ( GraphScaleArithmetic::compareEps(value, 
        interval.maxValue(), interval.width()) > 0 )
    {
        return false;
    }

    return true;
}

/*!
  Remove ticks from a list, that are not inside an interval

  \param ticks Tick list
  \param interval Interval

  \return Stripped tick list
*/
GraphValueList GraphScaleEngine::strip( 
    const GraphValueList& ticks, 
    const GraphDoubleInterval &interval) const
{
    if ( !interval.isValid() || ticks.count() == 0 )
        return GraphValueList();

    if ( contains(interval, ticks.first())
        && contains(interval, ticks.last()) )
    {
        return ticks;
    }

    GraphValueList strippedTicks;
    for ( int i = 0; i < (int)ticks.count(); i++ )
    {
        if ( contains(interval, ticks[i]) )
            strippedTicks += ticks[i];
    }
    return strippedTicks;
}

/*!
  \brief Build an interval for a value

  In case of v == 0.0 the interval is [-0.5, 0.5],
  otherwide it is [0.5 * v, 1.5 * v]
*/

GraphDoubleInterval GraphScaleEngine::buildInterval(double v) const
{
    const double delta = (v == 0.0) ? 0.5 : qAbs(0.5 * v);
    return GraphDoubleInterval(v - delta, v + delta);
}

/*!
  Change a scale attribute

  \param attribute Attribute to change
  \param on On/Off

  \sa Attribute, testAttribute()
*/
void GraphScaleEngine::setAttribute(Attribute attribute, bool on)
{
    if (on)
       d_data->attributes |= attribute;
    else
       d_data->attributes &= (~attribute);
}

/*!
  Check if a attribute is set.

  \param attribute Attribute to be tested
  \sa Attribute, setAttribute()
*/
bool GraphScaleEngine::testAttribute(Attribute attribute) const
{
    return bool(d_data->attributes & attribute);
}

/*!
  Change the scale attribute

  \param attributes Set scale attributes
  \sa Attribute, attributes()
*/
void GraphScaleEngine::setAttributes(int attributes)
{
    d_data->attributes = attributes;
}

/*!
  Return the scale attributes
  \sa Attribute, setAttributes(), testAttribute()
*/
int GraphScaleEngine::attributes() const
{
    return d_data->attributes;
}

/*!
  \brief Specify a reference point
  \param r new reference value

  The reference point is needed if options IncludeReference or
  Symmetric are active. Its default value is 0.0.

  \sa Attribute
*/
void GraphScaleEngine::setReference(double r)
{
    d_data->referenceValue = r;
}

/*!
 \return the reference value
 \sa setReference(), setAttribute()
*/
double GraphScaleEngine::reference() const 
{ 
    return d_data->referenceValue; 
}

/*!
  Return a transformation, for linear scales
*/
GraphScaleTransformation *GraphLinearScaleEngine::transformation() const
{
    return new GraphScaleTransformation(GraphScaleTransformation::Linear);
}

/*!
    Align and divide an interval 

   \param maxNumSteps Max. number of steps
   \param x1 First limit of the interval (In/Out)
   \param x2 Second limit of the interval (In/Out)
   \param stepSize Step size (Out)

   \sa setAttribute()
*/
void GraphLinearScaleEngine::autoScale(int maxNumSteps, 
    double &x1, double &x2, double &stepSize) const
{
    GraphDoubleInterval interval(x1, x2);
    interval = interval.normalized();

    interval.setMinValue(interval.minValue() - lowerMargin());
    interval.setMaxValue(interval.maxValue() + upperMargin());

    if (testAttribute(GraphScaleEngine::Symmetric))
        interval = interval.symmetrize(reference());
 
    if (testAttribute(GraphScaleEngine::IncludeReference))
        interval = interval.extend(reference());

    if (interval.width() == 0.0)
        interval = buildInterval(interval.minValue());

    stepSize = divideInterval(interval.width(), qMax(maxNumSteps, 1));

    if ( !testAttribute(GraphScaleEngine::Floating) )
        interval = align(interval, stepSize);

    x1 = interval.minValue();
    x2 = interval.maxValue();

    if (testAttribute(GraphScaleEngine::Inverted))
    {
        qSwap(x1, x2);
        stepSize = -stepSize;
    }
}

/*!
   \brief Calculate a scale division

   \param x1 First interval limit 
   \param x2 Second interval limit 
   \param maxMajSteps Maximum for the number of major steps
   \param maxMinSteps Maximum number of minor steps
   \param stepSize Step size. If stepSize == 0, the scaleEngine
                   calculates one.

   \sa GraphScaleEngine::stepSize(), GraphScaleEngine::subDivide()
*/
GraphScaleDiv GraphLinearScaleEngine::divideScale(double x1, double x2,
    int maxMajSteps, int maxMinSteps, double stepSize) const
{
    GraphDoubleInterval interval = GraphDoubleInterval(x1, x2).normalized();
    if (interval.width() <= 0 )
        return GraphScaleDiv();

    stepSize = qAbs(stepSize);
    if ( stepSize == 0.0 )
    {
        if ( maxMajSteps < 1 )
            maxMajSteps = 1;

        stepSize = divideInterval(interval.width(), maxMajSteps);
    }

    GraphScaleDiv scaleDiv;

    if ( stepSize != 0.0 )
    {
        GraphValueList ticks[GraphScaleDiv::NTickTypes];
        buildTicks(interval, stepSize, maxMinSteps, ticks);

        scaleDiv = GraphScaleDiv(interval, ticks);
    }

    if ( x1 > x2 )
        scaleDiv.invert();

    return scaleDiv;
}

void GraphLinearScaleEngine::buildTicks(
    const GraphDoubleInterval& interval, double stepSize, int maxMinSteps,
    GraphValueList ticks[GraphScaleDiv::NTickTypes]) const
{
    const GraphDoubleInterval boundingInterval =
        align(interval, stepSize);
    
    ticks[GraphScaleDiv::MajorTick] = 
        buildMajorTicks(boundingInterval, stepSize);

    if ( maxMinSteps > 0 )
    {
        buildMinorTicks(ticks[GraphScaleDiv::MajorTick], maxMinSteps, stepSize,
            ticks[GraphScaleDiv::MinorTick], ticks[GraphScaleDiv::MediumTick]);
    }
    
    for ( int i = 0; i < GraphScaleDiv::NTickTypes; i++ )
    {
        ticks[i] = strip(ticks[i], interval);

        // ticks very close to 0.0 are 
        // explicitely set to 0.0

        for ( int j = 0; j < (int)ticks[i].count(); j++ )
        {
            if ( GraphScaleArithmetic::compareEps(ticks[i][j], 0.0, stepSize) == 0 )
                ticks[i][j] = 0.0;
        }
    }
}

GraphValueList GraphLinearScaleEngine::buildMajorTicks(
    const GraphDoubleInterval &interval, double stepSize) const
{
    int numTicks = qRound(interval.width() / stepSize) + 1;
    if ( numTicks > 10000 )
        numTicks = 10000;

    GraphValueList ticks;

    ticks += interval.minValue();
    for (int i = 1; i < numTicks - 1; i++)
        ticks += interval.minValue() + i * stepSize;
    ticks += interval.maxValue();

    return ticks;
}

void GraphLinearScaleEngine::buildMinorTicks(
    const GraphValueList& majorTicks,
    int maxMinSteps, double stepSize,
    GraphValueList &minorTicks, 
    GraphValueList &mediumTicks) const
{   
    double minStep = divideInterval(stepSize, maxMinSteps);
    if (minStep == 0.0)  
        return; 
        
    // # ticks per interval
    int numTicks = (int)::ceil(qAbs(stepSize / minStep)) - 1;
    
    // Do the minor steps fit into the interval?
    if ( GraphScaleArithmetic::compareEps((numTicks +  1) * qAbs(minStep), 
        qAbs(stepSize), stepSize) > 0)
    {   
        numTicks = 1;
        minStep = stepSize * 0.5;
    }

    int medIndex = -1;
    if ( numTicks % 2 )
        medIndex = numTicks / 2;

    // calculate minor ticks

    for (int i = 0; i < (int)majorTicks.count(); i++)
    {
        double val = majorTicks[i];
        for (int k = 0; k < numTicks; k++)
        {
            val += minStep;

            double alignedValue = val;
            if (GraphScaleArithmetic::compareEps(val, 0.0, stepSize) == 0) 
                alignedValue = 0.0;

            if ( k == medIndex )
                mediumTicks += alignedValue;
            else
                minorTicks += alignedValue;
        }
    }
}

/*!
  \brief Align an interval to a step size

  The limits of an interval are aligned that both are integer
  multiples of the step size.

  \param interval Interval
  \param stepSize Step size

  \return Aligned interval
*/
GraphDoubleInterval GraphLinearScaleEngine::align(
    const GraphDoubleInterval &interval, double stepSize) const
{
    const double x1 = 
        GraphScaleArithmetic::floorEps(interval.minValue(), stepSize);
    const double x2 = 
        GraphScaleArithmetic::ceilEps(interval.maxValue(), stepSize);

    return GraphDoubleInterval(x1, x2);
}

/*!
  Return a transformation, for logarithmic (base 10) scales
*/
GraphScaleTransformation *GraphLog10ScaleEngine::transformation() const
{
    return new GraphScaleTransformation(GraphScaleTransformation::Log10);
}

/*!
    Align and divide an interval

   \param maxNumSteps Max. number of steps
   \param x1 First limit of the interval (In/Out)
   \param x2 Second limit of the interval (In/Out)
   \param stepSize Step size (Out)

   \sa GraphScaleEngine::setAttribute()
*/
void GraphLog10ScaleEngine::autoScale(int maxNumSteps, 
    double &x1, double &x2, double &stepSize) const
{
    if ( x1 > x2 )
        qSwap(x1, x2);

    GraphDoubleInterval interval(x1 / pow(10.0, lowerMargin()), 
        x2 * pow(10.0, upperMargin()) );

    if (interval.maxValue() / interval.minValue() < 10.0)
    {
        // scale width is less than one decade -> build linear scale

        GraphLinearScaleEngine linearScaler;
        linearScaler.setAttributes(attributes());
        linearScaler.setReference(reference());
        linearScaler.setMargins(lowerMargin(), upperMargin());

        linearScaler.autoScale(maxNumSteps, x1, x2, stepSize);
        stepSize = ::log10(stepSize);

        return;
    }

    double logRef = 1.0;
    if (reference() > LOG_MIN / 2)
        logRef = qMin(reference(), LOG_MAX / 2);

    if (testAttribute(GraphScaleEngine::Symmetric))
    {
        const double delta = qMax(interval.maxValue() / logRef,  
            logRef / interval.minValue());
        interval.setInterval(logRef / delta, logRef * delta);
    }

    if (testAttribute(GraphScaleEngine::IncludeReference))
        interval = interval.extend(logRef);

    interval = interval.limited(LOG_MIN, LOG_MAX);

    if (interval.width() == 0.0)
        interval = buildInterval(interval.minValue());

    stepSize = divideInterval(log10(interval).width(), qMax(maxNumSteps, 1));
    if ( stepSize < 1.0 )
        stepSize = 1.0;

    if (!testAttribute(GraphScaleEngine::Floating))
        interval = align(interval, stepSize);

    x1 = interval.minValue();
    x2 = interval.maxValue();

    if (testAttribute(GraphScaleEngine::Inverted))
    {
        qSwap(x1, x2);
        stepSize = -stepSize;
    }
}

/*!
   \brief Calculate a scale division

   \param x1 First interval limit 
   \param x2 Second interval limit 
   \param maxMajSteps Maximum for the number of major steps
   \param maxMinSteps Maximum number of minor steps
   \param stepSize Step size. If stepSize == 0, the scaleEngine
                   calculates one.

   \sa GraphScaleEngine::stepSize(), GraphLog10ScaleEngine::subDivide()
*/
GraphScaleDiv GraphLog10ScaleEngine::divideScale(double x1, double x2,
    int maxMajSteps, int maxMinSteps, double stepSize) const
{
    GraphDoubleInterval interval = GraphDoubleInterval(x1, x2).normalized();
    interval = interval.limited(LOG_MIN, LOG_MAX);

    if (interval.width() <= 0 )
        return GraphScaleDiv();

    if (interval.maxValue() / interval.minValue() < 10.0)
    {
        // scale width is less than one decade -> build linear scale
    
        GraphLinearScaleEngine linearScaler;
        linearScaler.setAttributes(attributes());
        linearScaler.setReference(reference());
        linearScaler.setMargins(lowerMargin(), upperMargin());

        if ( stepSize != 0.0 )
            stepSize = ::pow(10.0, stepSize);

        return linearScaler.divideScale(x1, x2, 
            maxMajSteps, maxMinSteps, stepSize);
    }

    stepSize = qAbs(stepSize);
    if ( stepSize == 0.0 )
    {
        if ( maxMajSteps < 1 )
            maxMajSteps = 1;

        stepSize = divideInterval(log10(interval).width(), maxMajSteps);
        if ( stepSize < 1.0 )
            stepSize = 1.0; // major step must be >= 1 decade
    }

    GraphScaleDiv scaleDiv;
    if ( stepSize != 0.0 )
    {
        GraphValueList ticks[GraphScaleDiv::NTickTypes];
        buildTicks(interval, stepSize, maxMinSteps, ticks);

        scaleDiv = GraphScaleDiv(interval, ticks);
    }

    if ( x1 > x2 )
        scaleDiv.invert();

    return scaleDiv;
}

void GraphLog10ScaleEngine::buildTicks(
    const GraphDoubleInterval& interval, double stepSize, int maxMinSteps,
    GraphValueList ticks[GraphScaleDiv::NTickTypes]) const
{
    const GraphDoubleInterval boundingInterval =
        align(interval, stepSize);
    
    ticks[GraphScaleDiv::MajorTick] = 
        buildMajorTicks(boundingInterval, stepSize);

    if ( maxMinSteps > 0 )
    {
        ticks[GraphScaleDiv::MinorTick] = buildMinorTicks(
            ticks[GraphScaleDiv::MajorTick], maxMinSteps, stepSize);
    }
    
    for ( int i = 0; i < GraphScaleDiv::NTickTypes; i++ )
        ticks[i] = strip(ticks[i], interval);
}

GraphValueList GraphLog10ScaleEngine::buildMajorTicks(
    const GraphDoubleInterval &interval, double stepSize) const
{
    double width = log10(interval).width();

    int numTicks = qRound(width / stepSize) + 1;
    if ( numTicks > 10000 )
        numTicks = 10000;

    const double lxmin = log(interval.minValue());
    const double lxmax = log(interval.maxValue());
    const double lstep = (lxmax - lxmin) / double(numTicks - 1);

    GraphValueList ticks;

    ticks += interval.minValue();

    for (int i = 1; i < numTicks; i++)
       ticks += exp(lxmin + double(i) * lstep);

    ticks += interval.maxValue();

    return ticks;
}

GraphValueList GraphLog10ScaleEngine::buildMinorTicks(
    const GraphValueList &majorTicks, 
    int maxMinSteps, double stepSize) const
{   
    if (stepSize < 1.1)            // major step width is one decade
    {
        if ( maxMinSteps < 1 )
            return GraphValueList();
            
        int k0, kstep, kmax;
        
        if (maxMinSteps >= 8)
        {
            k0 = 2;
            kmax = 9;
            kstep = 1;
        }   
        else if (maxMinSteps >= 4)
        {
            k0 = 2;
            kmax = 8;
            kstep = 2;
        }   
        else if (maxMinSteps >= 2)
        {
            k0 = 2;
            kmax = 5;
            kstep = 3;
        }
        else
        {
            k0 = 5;
            kmax = 5;
            kstep = 1;
        }

        GraphValueList minorTicks;

        for (int i = 0; i < (int)majorTicks.count(); i++)
        {
            const double v = majorTicks[i];
            for (int k = k0; k<= kmax; k+=kstep)
                minorTicks += v * double(k);
        }

        return minorTicks;
    }
    else  // major step > one decade
    {
        double minStep = divideInterval(stepSize, maxMinSteps);
        if ( minStep == 0.0 )
            return GraphValueList();

        if ( minStep < 1.0 )
            minStep = 1.0;

        // # subticks per interval
        int nMin = qRound(stepSize / minStep) - 1;

        // Do the minor steps fit into the interval?

        if ( GraphScaleArithmetic::compareEps((nMin +  1) * minStep, 
            qAbs(stepSize), stepSize) > 0)
        {
            nMin = 0;
        }

        if (nMin < 1)
            return GraphValueList();      // no subticks

        // substep factor = 10^substeps
        const double minFactor = qMax(pow(10.0, minStep), 10.0);

        GraphValueList minorTicks;
        for (int i = 0; i < (int)majorTicks.count(); i++)
        {
            double val = majorTicks[i];
            for (int k=0; k< nMin; k++)
            {
                val *= minFactor;
                minorTicks += val;
            }
        }
        return minorTicks;
    }
}

/*!
  \brief Align an interval to a step size

  The limits of an interval are aligned that both are integer
  multiples of the step size.

  \param interval Interval
  \param stepSize Step size

  \return Aligned interval
*/
GraphDoubleInterval GraphLog10ScaleEngine::align(
    const GraphDoubleInterval &interval, double stepSize) const
{
    const GraphDoubleInterval intv = log10(interval);

    const double x1 = GraphScaleArithmetic::floorEps(intv.minValue(), stepSize);
    const double x2 = GraphScaleArithmetic::ceilEps(intv.maxValue(), stepSize);

    return pow10(GraphDoubleInterval(x1, x2));
}

/*!
  Return the interval [log10(interval.minValue(), log10(interval.maxValue]
*/

GraphDoubleInterval GraphLog10ScaleEngine::log10(
    const GraphDoubleInterval &interval) const
{
    return GraphDoubleInterval(::log10(interval.minValue()),
            ::log10(interval.maxValue()));
}

/*!
  Return the interval [pow10(interval.minValue(), pow10(interval.maxValue]
*/
GraphDoubleInterval GraphLog10ScaleEngine::pow10(
    const GraphDoubleInterval &interval) const
{
    return GraphDoubleInterval(pow(10.0, interval.minValue()),
            pow(10.0, interval.maxValue()));
}

