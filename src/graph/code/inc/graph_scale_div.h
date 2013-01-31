#ifndef GRAPH_SCALE_DIV_H
#define GRAPH_SCALE_DIV_H

#include "graph_global.h"
#include "graph_valuelist.h"
#include "graph_double_interval.h"

namespace GraphLib
{

    class GraphDoubleInterval;

    /*!
      \brief A class representing a scale division

      A scale division consists of its limits and 3 list
      of tick values qualified as major, medium and minor ticks.

      In most cases scale divisions are calculated by a QwtScaleEngine.

      \sa subDivideInto(), subDivide()
     */

    class GraphScaleDiv
    {
        public:
            //! Scale tick types
            enum TickType
            {
                NoTick = -1,

                MinorTick,
                MediumTick,
                MajorTick,

                NTickTypes
            };

            explicit GraphScaleDiv();
            explicit GraphScaleDiv(const GraphDoubleInterval &,
                    GraphValueList[NTickTypes]);
            explicit GraphScaleDiv(double lowerBound, double upperBound,
                    GraphValueList[NTickTypes]);

            int operator==(const GraphScaleDiv &s) const;
            int operator!=(const GraphScaleDiv &s) const;

            void setInterval(double lowerBound, double upperBound);
            void setInterval(const GraphDoubleInterval &);
            GraphDoubleInterval interval() const;

            double lowerBound() const;
            double upperBound() const;
            double range() const;

            bool contains(double v) const;

            void setTicks(int type, const GraphValueList &);
            const GraphValueList &ticks(int type) const;

            void invalidate();
            bool isValid() const;

            void invert();

        private:
            double d_lowerBound;
            double d_upperBound;
            GraphValueList d_ticks[NTickTypes];

            bool d_isValid;
    };

    /*!
      Change the interval
      \param lowerBound lower bound
      \param upperBound upper bound
     */
    inline void GraphScaleDiv::setInterval(double lowerBound, double upperBound)
    {
        d_lowerBound = lowerBound;
        d_upperBound = upperBound;
    }

    /*! 
      \return lowerBound -> upperBound
     */
    inline GraphDoubleInterval GraphScaleDiv::interval() const
    {
        return GraphDoubleInterval(d_lowerBound, d_upperBound);
    }

    /*! 
      \return lower bound
      \sa upperBound()
     */
    inline double GraphScaleDiv::lowerBound() const 
    { 
        return d_lowerBound;
    }

    /*! 
      \return upper bound
      \sa lowerBound()
     */
    inline double GraphScaleDiv::upperBound() const 
    { 
        return d_upperBound;
    }

    /*! 
      \return upperBound() - lowerBound()
     */
    inline double GraphScaleDiv::range() const 
    { 
        return d_upperBound - d_lowerBound;
    }

}
#endif

