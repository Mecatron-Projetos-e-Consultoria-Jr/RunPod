#include "DataAnalysis.h"
namespace Data_Analysis
{
    // Function that returns true when the x value is inside the thrashold and false otherwise
    bool inside_thrashold(double x_average, const double thrashold){

        // if outside thrashold, return false, otherwise return true
        if( x_average > thrashold || x_average < -1*thrashold)
            return false;
        else
            return true;
    }
} // namespace Data_Analysis
