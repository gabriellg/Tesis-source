/* -----------------------------------------------------------------------------
*  CMath
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CMath.hpp"

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <limits>
#include <algorithm>

#include "asrtbas.h"

double CMath::PI = 3.14159265358979323846;

static bool prv_randomNumber_is_initialized = false;


//------------------------------------------------------------------------------
//
void CMath::swap(double *a, double *b)
{
    double temp;

    assert_no_null(a);
    assert_no_null(b);

    temp = *a;
    *a = *b;
    *b = temp;
}

//------------------------------------------------------------------------------
//
int CMath::getRandomNumber(int min, int max)
{
    assert(min < max);

    if( prv_randomNumber_is_initialized == false )
    {
        srand ( time(NULL) );
        prv_randomNumber_is_initialized = true;
    }

    return ( (rand() % (max - min + 1)) + min);
}

//------------------------------------------------------------------------------
//
double CMath::getRandomNumber()
{
    if ( prv_randomNumber_is_initialized == false)
    {
        srand48( time(NULL) );
        prv_randomNumber_is_initialized = true;
    }

    return drand48();
}

//------------------------------------------------------------------------------
//
double CMath::absolute(double value)
{
    return ::fabs(value);
}

//------------------------------------------------------------------------------
//
double CMath::floor(double value)
{
    return ::floor(value);
}

//------------------------------------------------------------------------------
//
long CMath::round(double value)
{
    double rounded;

    rounded = ::round(value);
    assert(rounded < LONG_MAX);

    return (long)rounded;
}

//------------------------------------------------------------------------------
//
unsigned long CMath::roundu( double value )
{
    double rounded;

    rounded = ::round(value);
    assert(rounded > -0.00001);
    assert(rounded < ULONG_MAX);

    return (unsigned long)rounded;
}

//------------------------------------------------------------------------------
//
double CMath::degreeToRadian(double degree)
{
    return degree * PI / 180.;
}

//------------------------------------------------------------------------------
//
double CMath::radianToDegree(double degree)
{
    return degree * 180. / PI;
}

//------------------------------------------------------------------------------
//
double CMath::cosine(double value)
{
    return cos(value);
}

//------------------------------------------------------------------------------
//
double CMath::sine(double value)
{
    return sin(value);
}

//------------------------------------------------------------------------------
//
double CMath::arccos(double value)
{
    assert(value <= 1.);
    assert(value >= -1.);

    return acos(value);
}

//------------------------------------------------------------------------------
//
double CMath::arcsin(double value)
{
    assert(value <= 1.);
    assert(value >= -1.);

    return asin(value);
}

//------------------------------------------------------------------------------
//
double CMath::arctan(double y, double x)
{
    assert(y != 0. || x != 0.);

    return atan2(y, x);
}

//------------------------------------------------------------------------------
//#define PI        3.14159265358979323846


double CMath::pow(double base, double exp)
{
    return ::pow( base, exp );
}

//------------------------------------------------------------------------------
//
double CMath::exp(double value)
{
    return ::exp(value);
}

//------------------------------------------------------------------------------
//
double CMath::sqrt(double value)
{
    assert(value >= 0.);
    return ::sqrt(value);
}

//------------------------------------------------------------------------------
//
bool CMath::isEqual(double d1, double d2, double tolerance)
{
    double distance;

    distance = fabs(d1 - d2);

    if (distance < tolerance)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
bool CMath::isZero(double d, double tolerance)
{
    double distance;

    distance = fabs(d);

    if (distance < tolerance)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
double CMath :: max_element(const double *values,
                             unsigned long numElements,
                             unsigned long *ind_optional)
{
    const double *max_value;

    max_value = std::max_element(values, values + numElements);

    if(ind_optional != NULL)
    {
        *ind_optional = max_value - values;
    }

    return *max_value;
}

//-------------------------------------------------------------
// http://en.wikipedia.org/wiki/Limits.h
int CMath::maxInt()
{
    return INT_MAX;
}

//-------------------------------------------------------------
//
unsigned int CMath::maxUInt()
{
    return UINT_MAX;
}

//-------------------------------------------------------------
//
long CMath::minLong()
{
    return LONG_MIN;
}

//-------------------------------------------------------------
//
long CMath::maxLong()
{
    return LONG_MAX;
}

//-------------------------------------------------------------
//
unsigned long CMath::maxULong()
{
    return ULONG_MAX;
}

//-------------------------------------------------------------
//
float CMath::maxFloat()
{
    return std::numeric_limits<float>::max();
}

//-------------------------------------------------------------
//
double CMath::maxDouble()
{
    return std::numeric_limits<double>::max();
}

//-------------------------------------------------------------
//
double CMath::minDouble()
{
    return std::numeric_limits<double>::min();
}

