// Define un array de puntos 2d.

#include "PrvBaseArray.inl"

class CArrPoint2d: private PrvBaseArray
{
    public:

        CArrPoint2d(void);
        CArrPoint2d(unsigned long numPoints);
        CArrPoint2d(const class CArrPoint2d *points);

        class CArrPoint2d *invert(void) const;

        unsigned long size(void) const;

        void set(unsigned long ind, double x, double y);
        void add(double x, double y);
        void insert(unsigned long ind, double x, double y);
        void erase(unsigned long ind);

        void get(unsigned long pos, double *x, double *y) const;

        bool isClosed(double precision) const;
        bool existPoint2d(double x, double y, double prec, unsigned long *indFirst_optional) const;

        float *getArrayC(void) const;
};
