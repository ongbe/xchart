
#ifndef GRAPH_SCALE_MAP_H
#define GRAPH_SCALE_MAP_H

#include "graph_global.h"
#include "graph_math.h"

namespace GraphLib
{
    /*!
      \brief Operations for linear or logarithmic (base 10) transformations
     */
    class GraphScaleTransformation
    {
        public:
            enum Type
            {
                Linear,
                Log10,

                Other
            };

            GraphScaleTransformation(Type type);
            virtual ~GraphScaleTransformation();

            virtual double xForm(double x, double s1, double s2,
                    double p1, double p2) const;
            virtual double invXForm(double x, double s1, double s2,
                    double p1, double p2) const;

            Type type() const;

            virtual GraphScaleTransformation *copy() const;

        private:
            GraphScaleTransformation();
            GraphScaleTransformation &operator=( const GraphScaleTransformation);

            const Type d_type;
    };

    //! \return Transformation type
    inline GraphScaleTransformation::Type GraphScaleTransformation::type() const 
    { 
        return d_type; 
    }

    /*!
      \brief A scale map

      GraphScaleMap offers transformations from a scale 
      into a paint interval and vice versa.
     */
    class GraphScaleMap
    {
        public:
            GraphScaleMap();
            GraphScaleMap(const GraphScaleMap&);

            ~GraphScaleMap();

            GraphScaleMap &operator=(const GraphScaleMap &);

            void setTransformation(GraphScaleTransformation * );
            const GraphScaleTransformation *transformation() const;

            void setPaintInterval(int p1, int p2);
            void setPaintXInterval(double p1, double p2);
            void setScaleInterval(double s1, double s2);

            int transform(double x) const;
            double invTransform(double i) const;

            double xTransform(double x) const;

            double p1() const;
            double p2() const;

            double s1() const;
            double s2() const;

            double pDist() const;
            double sDist() const;

            QT_STATIC_CONST double LogMin;
            QT_STATIC_CONST double LogMax;

        private:
            void newFactor();   

            double d_s1, d_s2;     // scale interval boundaries
            double d_p1, d_p2;     // paint device interval boundaries

            double d_cnv;       // conversion factor

            GraphScaleTransformation *d_transformation;
    };

    /*!
      \return First border of the scale interval
     */
    inline double GraphScaleMap::s1() const 
    {
        return d_s1;
    }

    /*!
      \return Second border of the scale interval
     */
    inline double GraphScaleMap::s2() const 
    {
        return d_s2;
    }

    /*!
      \return First border of the paint interval
     */
    inline double GraphScaleMap::p1() const 
    {
        return d_p1;
    }

    /*!
      \return Second border of the paint interval
     */
    inline double GraphScaleMap::p2() const 
    {
        return d_p2;
    }

    /*!
      \return qwtAbs(p2() - p1())
     */
    inline double GraphScaleMap::pDist() const
    {
        return graphAbs(d_p2 - d_p1);
    }

    /*!
      \return qwtAbs(s2() - s1())
     */
    inline double GraphScaleMap::sDist() const
    {
        return graphAbs(d_s2 - d_s1);
    }

    /*!
      Transform a point related to the scale interval into an point 
      related to the interval of the paint device

      \param s Value relative to the coordinates of the scale
     */
    inline double GraphScaleMap::xTransform(double s) const
    {
        // try to inline code from GraphScaleTransformation

        if ( d_transformation->type() == GraphScaleTransformation::Linear )
            return d_p1 + (s - d_s1) * d_cnv;

        if ( d_transformation->type() == GraphScaleTransformation::Log10 )
            return d_p1 + log(s / d_s1) * d_cnv;

        return d_transformation->xForm(s, d_s1, d_s2, d_p1, d_p2 );
    }

    /*!
      Transform an paint device value into a value in the 
      interval of the scale.

      \param p Value relative to the coordinates of the paint device
      \sa transform()
     */
    inline double GraphScaleMap::invTransform(double p) const
    {
        return d_transformation->invXForm(p, d_p1, d_p2, d_s1, d_s2 );
    }

    /*!
      Transform a point related to the scale interval into an point 
      related to the interval of the paint device and round it to
      an integer. (In Qt <= 3.x paint devices are integer based. )

      \param s Value relative to the coordinates of the scale
      \sa xTransform()
     */
    inline int GraphScaleMap::transform(double s) const
    {
        return qRound(xTransform(s));
    }
}

#endif

