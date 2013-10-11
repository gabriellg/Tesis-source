/* -----------------------------------------------------------------------------
 *  CArrayDataBase.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CArraysTypes.hpp"

#include "asrtbas.h"
#include "memory.h"

enum prv_typeData_t
{
    PRV_DOUBLE, PRV_ULONG
};

union data_t
{
    double valueD;
    unsigned long valueU;
};

struct prv_data_t
{
    enum prv_typeData_t typeData;
    union data_t value;
};

//-----------------------------------------------------------------------

static struct prv_data_t *prv_createData(enum prv_typeData_t typeData, const union data_t *value)
{
    struct prv_data_t *data;

    data = MALLOC(struct prv_data_t);

    data->typeData = typeData;
    data->value = *value;

    return data;
}

//-----------------------------------------------------------------------

static struct prv_data_t *prv_createDefaultDouble(double valueD)
{
    union data_t value;

    value.valueD = valueD;
    return prv_createData(PRV_DOUBLE, &value);
}

//-----------------------------------------------------------------------

static struct prv_data_t *prv_createDefaultULong(unsigned long valueU)
{
    union data_t value;

    value.valueU = valueU;
    return prv_createData(PRV_ULONG, &value);
}

//-----------------------------------------------------------------------

static struct prv_data_t *prv_copyData(const struct prv_data_t *data)
{
    assert_no_null(data);
    return prv_createData(data->typeData, &data->value);
}

//-----------------------------------------------------------------------

static void prv_destroyData(struct prv_data_t **data)
{
    FREE_T(data, struct prv_data_t);
}

//-----------------------------------------------------------------------

static double prv_getDouble(const struct prv_data_t *data)
{
    assert_no_null(data);
    assert(data->typeData == PRV_DOUBLE);

    return data->value.valueD;
}

//-----------------------------------------------------------------------

static unsigned long prv_getULong(const struct prv_data_t *data)
{
    assert_no_null(data);
    assert(data->typeData == PRV_ULONG);

    return data->value.valueU;
}

//------------------------------------------------------------------------------

CArrayDouble::CArrayDouble(void) : PrvBaseArray(0, (CArray_FPtr_destroyElement)prv_destroyData)
{
    ;
}

//------------------------------------------------------------------------------

CArrayDouble::CArrayDouble(unsigned long numValues) : PrvBaseArray(numValues, (CArray_FPtr_destroyElement)prv_destroyData)
{
    for (unsigned long i = 0; i < numValues; i++)
    {
        struct prv_data_t *data;

        data = prv_createDefaultDouble(0.);
        PrvBaseArray::set(i, (struct prv_item_t *)data);
    }
}

//------------------------------------------------------------------------------

CArrayDouble::CArrayDouble(const class CArrayDouble *values) : PrvBaseArray(values, (CArray_FPtr_copyElement)prv_copyData)
{
    ;
}

//------------------------------------------------------------------------------

unsigned long CArrayDouble::size(void) const
{
    return PrvBaseArray::size();
}

//------------------------------------------------------------------------------

void CArrayDouble::set(unsigned long ind, double value)
{
    struct prv_data_t *data;

    data = (struct prv_data_t *)PrvBaseArray::get(ind);
    assert_no_null(data);
    assert(data->typeData == PRV_DOUBLE);

    data->value.valueD = value;
}

//------------------------------------------------------------------------------

void CArrayDouble::add(double value)
{
    struct prv_data_t *data;

    data = prv_createDefaultDouble(value);
    PrvBaseArray::add((struct prv_item_t *)data);
}

//------------------------------------------------------------------------------

void CArrayDouble::insert(unsigned long ind, double value)
{
    struct prv_data_t *data;

    data = prv_createDefaultDouble(value);
    PrvBaseArray::insert(ind, (struct prv_item_t *)data);
}

//------------------------------------------------------------------------------

void CArrayDouble::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//------------------------------------------------------------------------------

void CArrayDouble::eraseLast(void)
{
    PrvBaseArray::eraseLast();
}

//------------------------------------------------------------------------------

double CArrayDouble::get(unsigned long ind) const
{
    struct prv_data_t *data;

    data = (struct prv_data_t *)PrvBaseArray::get(ind);
    return prv_getDouble(data);
}

//------------------------------------------------------------------------------

double CArrayDouble::getLast(void) const
{
    struct prv_data_t *data;

    data = (struct prv_data_t *)PrvBaseArray::getLast();
    return prv_getDouble(data);
}

//------------------------------------------------------------------------------

CArrayULong::CArrayULong(void) : PrvBaseArray(0, (CArray_FPtr_destroyElement)prv_destroyData)
{
    ;
}

//------------------------------------------------------------------------------

CArrayULong::CArrayULong(unsigned long numValues) : PrvBaseArray(numValues, (CArray_FPtr_destroyElement)prv_destroyData)
{
    for (unsigned long i = 0; i < numValues; i++)
    {
        struct prv_data_t *data;

        data = prv_createDefaultULong(0);
        PrvBaseArray::set(i, (struct prv_item_t *)data);
    }
}

//------------------------------------------------------------------------------

CArrayULong::CArrayULong(const class CArrayULong *values) : PrvBaseArray(values, (CArray_FPtr_copyElement)prv_copyData)
{
    ;
}

//------------------------------------------------------------------------------

unsigned long CArrayULong::size(void) const
{
    return PrvBaseArray::size();
}

//------------------------------------------------------------------------------

void CArrayULong::set(unsigned long ind, unsigned long value)
{
    struct prv_data_t *data;

    data = (struct prv_data_t *)PrvBaseArray::get(ind);
    assert_no_null(data);
    assert(data->typeData == PRV_ULONG);

    data->value.valueU = value;
}

//------------------------------------------------------------------------------

void CArrayULong::add(unsigned long value)
{
    struct prv_data_t *data;

    data = prv_createDefaultULong(value);
    PrvBaseArray::add((struct prv_item_t *)data);
}

//------------------------------------------------------------------------------

void CArrayULong::insert(unsigned long ind, unsigned long value)
{
    struct prv_data_t *data;

    data = prv_createDefaultULong(value);
    PrvBaseArray::insert(ind, (struct prv_item_t *)data);
}

//------------------------------------------------------------------------------

void CArrayULong::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//------------------------------------------------------------------------------

void CArrayULong::eraseLast(void)
{
    PrvBaseArray::eraseLast();
}

//------------------------------------------------------------------------------

void CArrayULong::concatenate(const CArrayULong *array)
{
    PrvBaseArray::concatenate(array, (CArray_FPtr_copyElement)prv_copyData);
}

//------------------------------------------------------------------------------

unsigned long CArrayULong::get(unsigned long ind) const
{
    struct prv_data_t *data;

    data = (struct prv_data_t *)PrvBaseArray::get(ind);
    return prv_getULong(data);
}

//------------------------------------------------------------------------------

unsigned long CArrayULong::getLast(void) const
{
    struct prv_data_t *data;

    data = (struct prv_data_t *)PrvBaseArray::getLast();
    return prv_getULong(data);
}

//------------------------------------------------------------------------------

unsigned int *CArrayULong::getArrayC(void) const
{
    unsigned int *array_c;
    unsigned long numElems;

    numElems = PrvBaseArray::size();

    if (numElems > 0)
    {
        array_c = CALLOC(numElems, unsigned int);

        for (unsigned long i = 0; i < numElems; i++)
        {
            struct prv_data_t *data;

            data = (struct prv_data_t *)PrvBaseArray::get(i);
            assert(data->typeData == PRV_ULONG);

            array_c[i] = data->value.valueU;
        }
    }
    else
        array_c = NULL;

    return array_c;
}

