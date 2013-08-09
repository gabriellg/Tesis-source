/* -----------------------------------------------------------------------------
 *  CArrayDataBase.h
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "PrvBaseArray.inl"

class CArrayDouble : private PrvBaseArray
{
    public:
        CArrayDouble(void);
        CArrayDouble(unsigned long numValues);
        CArrayDouble(const class CArrayDouble *array);

        unsigned long size(void) const;

        void set(unsigned long ind, double value);
        void add(double value);
        void insert(unsigned long ind, double value);
        void erase(unsigned long ind);
        void eraseLast(void);

        double get(unsigned long ind) const;
        double getLast(void) const;
};

class CArrayULong : private PrvBaseArray
{
    public:
        CArrayULong(void);
        CArrayULong(unsigned long numValues);
        CArrayULong(const class CArrayULong *array);

        unsigned long size(void) const;

        void set(unsigned long ind, unsigned long value);
        void add(unsigned long value);
        void insert(unsigned long ind, unsigned long value);
        void erase(unsigned long ind);
        void eraseLast(void);

        void concatenate(const CArrayULong *array);

        unsigned long get(unsigned long ind) const;
        unsigned long getLast(void) const;

        unsigned long *getArrayC(void) const;
};
