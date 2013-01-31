#ifndef GRAPH_DOUBLE_INTERVAL_H
#define GRAPH_DOUBLE_INTERVAL_H

#include "graph_global.h"

namespace GraphLib
{

    /*!
      \brief A class representing an interval

      The interval is represented by 2 doubles, the lower and the upper limit. 
     */

    class GraphDoubleInterval
    {
        public:
            /*!
              Flag indicating if a border is included/excluded from an interval

              - IncludeBorders\n
              min/max values are inside the interval
              - ExcludeMinimum\n
              min value is not included in the interval
              - ExcludeMaximum\n
              max value is not included in the interval
              - ExcludeBorders\n
              min/max values are not included in the interval

              \sa setBorderMode(), testBorderMode() 
             */
            enum BorderMode
            {
                IncludeBorders = 0,

                ExcludeMinimum = 1,
                ExcludeMaximum = 2,

                ExcludeBorders = ExcludeMinimum | ExcludeMaximum
            };

            GraphDoubleInterval();
            GraphDoubleInterval(double minValue, double maxValue, 
                    int borderFlags = IncludeBorders);

            void setInterval(double minValue, double maxValue, 
                    int borderFlags = IncludeBorders);

            GraphDoubleInterval normalized() const;
            GraphDoubleInterval inverted() const;
            GraphDoubleInterval limited(double minValue, double maxValue) const;

            int operator==(const GraphDoubleInterval &) const;
            int operator!=(const GraphDoubleInterval &) const;

            void setBorderFlags(int);
            int borderFlags() const;

            double minValue() const;
            double maxValue() const;

            double width() const;

            void setMinValue(double);
            void setMaxValue(double);

            bool contains(double value) const;

            bool intersects(const GraphDoubleInterval &) const;
            GraphDoubleInterval intersect(const GraphDoubleInterval &) const;
            GraphDoubleInterval unite(const GraphDoubleInterval &) const;

            GraphDoubleInterval operator|(const GraphDoubleInterval &) const;
            GraphDoubleInterval operator&(const GraphDoubleInterval &) const;

            GraphDoubleInterval &operator|=(const GraphDoubleInterval &);
            GraphDoubleInterval &operator&=(const GraphDoubleInterval &);

            GraphDoubleInterval extend(double value) const;
            GraphDoubleInterval operator|(double) const;
            GraphDoubleInterval &operator|=(double);

            bool isValid() const;
            bool isNull() const;
            void invalidate();

            GraphDoubleInterval symmetrize(double value) const;

        private:
            double d_minValue;
            double d_maxValue;
            int d_borderFlags;
    };

    /*!
      \brief Default Constructor

      Creates an invalid interval [0.0, -1.0]
      \sa setInterval(), isValid()
     */
    inline GraphDoubleInterval::GraphDoubleInterval():
        d_minValue(0.0),
        d_maxValue(-1.0),
        d_borderFlags(IncludeBorders)
    {
    }

    /*!
      Constructor

      Build an interval with from min/max values

      \param minValue Minimum value
      \param maxValue Maximum value
      \param borderFlags Include/Exclude borders
     */
    inline GraphDoubleInterval::GraphDoubleInterval(
            double minValue, double maxValue, int borderFlags):
        d_minValue(minValue),
        d_maxValue(maxValue),
        d_borderFlags(borderFlags)
    {
    }

    /*!
      Assign the limits of the interval

      \param minValue Minimum value
      \param maxValue Maximum value
      \param borderFlags Include/Exclude borders
     */
    inline void GraphDoubleInterval::setInterval(
            double minValue, double maxValue, int borderFlags)
    {
        d_minValue = minValue;
        d_maxValue = maxValue;
        d_borderFlags = borderFlags;
    }

    /*!
      Change the border flags

      \param borderFlags Or'd BorderMode flags
      \sa borderFlags()
     */
    inline void GraphDoubleInterval::setBorderFlags(int borderFlags)
    {
        d_borderFlags = borderFlags;
    }

    /*!
      \return Border flags
      \sa setBorderFlags()
     */
    inline int GraphDoubleInterval::borderFlags() const
    {
        return d_borderFlags; 
    }

    /*!
      Assign the lower limit of the interval

      \param minValue Minimum value
     */
    inline void GraphDoubleInterval::setMinValue(double minValue)
    {   
        d_minValue = minValue;
    }

    /*!
      Assign the upper limit of the interval

      \param maxValue Maximum value
     */
    inline void GraphDoubleInterval::setMaxValue(double maxValue)
    {
        d_maxValue = maxValue;
    }

    //! \return Lower limit of the interval
    inline double GraphDoubleInterval::minValue() const 
    { 
        return d_minValue; 
    }

    //! \return Upper limit of the interval
    inline double GraphDoubleInterval::maxValue() const 
    { 
        return d_maxValue; 
    }

    /*!
      Return the width of an interval
      The width of invalid intervals is 0.0, otherwise the result is
      maxValue() - minValue().

      \sa isValid()
     */
    inline double GraphDoubleInterval::width() const 
    { 
        return isValid() ? (d_maxValue - d_minValue) : 0.0; 
    }

    /*! 
      Intersection of two intervals
      \sa intersect()
     */
    inline GraphDoubleInterval GraphDoubleInterval::operator&(
            const GraphDoubleInterval &interval ) const
    {
        return intersect(interval);
    }

    /*! 
      Union of two intervals
      \sa unite()
     */
    inline GraphDoubleInterval GraphDoubleInterval::operator|(
            const GraphDoubleInterval &interval) const
    {
        return unite(interval);
    }

    //! Compare two intervals
    inline int GraphDoubleInterval::operator==(const GraphDoubleInterval &other) const
    {
        return (d_minValue == other.d_minValue) &&
            (d_maxValue == other.d_maxValue) &&
            (d_borderFlags == other.d_borderFlags);
    }

    //! Compare two intervals
    inline int GraphDoubleInterval::operator!=(const GraphDoubleInterval &other) const
    {
        return (!(*this == other));
    }

    /*!
      Extend an interval
      \sa extend()
     */
    inline GraphDoubleInterval GraphDoubleInterval::operator|(double value) const
    {
        return extend(value);
    }

    //! \return true, if isValid() && (minValue() >= maxValue())
    inline bool GraphDoubleInterval::isNull() const
    {
        return isValid() && d_minValue >= d_maxValue;
    }

    /*! 
      A interval is valid when minValue() <= maxValue().
      In case of GraphDoubleInterval::ExcludeBorders it is true
      when minValue() < maxValue()
     */
    inline bool GraphDoubleInterval::isValid() const
    {
        if ( (d_borderFlags & ExcludeBorders) == 0 )
            return d_minValue <= d_maxValue;
        else
            return d_minValue < d_maxValue;
    }

    /*!
      Invalidate the interval

      The limits are set to interval [0.0, -1.0]
      \sa isValid()
     */
    inline void GraphDoubleInterval::invalidate()
    {
        d_minValue = 0.0;
        d_maxValue = -1.0;
    }

}
#endif

