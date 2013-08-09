// Define un array de puntos 3d.

#include "PrvBaseArray.inl"

class CArrPoint3d : private PrvBaseArray
{
    public:

        CArrPoint3d(void);
        CArrPoint3d(unsigned long numPoints);
        CArrPoint3d(unsigned long numPoints, double x, double y, double z);
        CArrPoint3d(const class CArrPoint3d *array);

        class CArrPoint3d *invert(void) const;

        unsigned long size(void) const;

        void set(unsigned long ind, double x, double y, double z);
        void insert(unsigned long ind, double x, double y, double z);
        void add(double x, double y, double z);
        void erase(unsigned long ind);

        void concatenate(const class CArrPoint3d *points);

        void get(unsigned long ind, double *x, double *y, double *z) const;
        void getLastPoint(double *x, double *y, double *z) const;

        bool minMax3D(double *xmin, double *ymin, double *zmin, double *xmax, double *ymax, double *zmax) const;
        bool isClosed(double precision) const;

        bool existPoint3d(double x, double y, double z, double prec, unsigned long *indFirst_optional) const;
        bool existOnlyOnePoint3d(double x, double y, double z, double prec, unsigned long *indPos) const;

        float *getArrayC(void) const;
        double *getArrayDoubleC(void) const;
};
