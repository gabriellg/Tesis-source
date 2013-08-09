/* -----------------------------------------------------------------------------
*  CArrPixel
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseArray.inl"

class CArrPixel: public PrvBaseArray
{
    public:

        CArrPixel();
        CArrPixel(unsigned long size);
        CArrPixel(const CArrPixel *array);

        unsigned long size() const;

        void get(unsigned long ind, long *x, long *y) const;
        void getLast(long *x, long *y) const;

        void set(unsigned long ind, long x, long y);
        void add(long x, long y);
        void erase(unsigned long ind);

        bool minMax(long *xMin_opc, long *yMin_opc, long *xMax_opc, long *yMax_opc) const;
};
