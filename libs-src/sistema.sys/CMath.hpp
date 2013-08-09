/* -----------------------------------------------------------------------------
*  CMath
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define ABS(x)	  ((x) > 0 ? (x) : -(x))
#define SWAP(x,y) x^=y^=x^=y;

class CMath
{
    public:

        static double PI;

        static void swap(double *a, double *b);

        static int getRandomNumber(int min, int max);
        static double getRandomNumber();

        static double absolute(double value);

        static double floor(double value);

        static long          round(double x);
        static unsigned long roundu(double value);

        static double degreeToRadian(double degree);
        static double radianToDegree(double degree);

        static double cosine(double value);
        static double sine(double value);

        static double arccos(double value);
        static double arcsin(double value);
        static double arctan(double y, double x);

        static double pow(double base, double exp);
        static double exp(double value);
        static double sqrt(double value);

        static bool isEqual(double d1, double d2, double tolerance);
        static bool isZero(double d, double tolerance);

        static double      max_element(const double *values, unsigned long numElements, unsigned long *ind_optional);

        static int          maxInt();
        static unsigned int maxUInt();
        static long         minLong();
        static long         maxLong();
        static unsigned long maxULong();
        static float        maxFloat();
        static double       maxDouble();

        static double       minDouble();
};
