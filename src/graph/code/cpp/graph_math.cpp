
#include "graph_math.h"

using namespace GraphLib;

/*!
  \brief Find the smallest value in an array
  \param array Pointer to an array
  \param size Array size
*/
double graphGetMin(const double *array, int size)
{
    if (size <= 0)
       return 0.0;

    double rv = array[0];
    for (int i = 1; i < size; i++)
       rv = graphMin(rv, array[i]);

    return rv;
}


/*!
  \brief Find the largest value in an array
  \param array Pointer to an array
  \param size Array size
*/
double graphGetMax(const double *array, int size)
{
    if (size <= 0)
       return 0.0;
    
    double rv = array[0];
    for (int i = 1; i < size; i++)
       rv = graphMax(rv, array[i]);

    return rv;
}

