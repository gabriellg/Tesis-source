// Define un array de puntos 3d.

#include "CArrPoint2d.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"

struct SPrvPoint2d
{
    double x, y;
};

struct SPrvDataSearchPoint2d
{
    double x, y;
    double prec;
};

//-----------------------------------------------------------------------

static struct SPrvPoint2d *prv_createPoint2d(double x, double y)
{
    struct SPrvPoint2d *point2d;

    point2d = MALLOC(struct SPrvPoint2d);

    point2d->x = x;
    point2d->y = y;

    return point2d;
}

//-----------------------------------------------------------------------

static struct SPrvPoint2d *prv_copyPoint2d(const struct SPrvPoint2d *point)
{
    assert_no_null(point);
    return prv_createPoint2d(point->x, point->y);
}

//-----------------------------------------------------------------------

static void prv_destroyPoint2d(struct SPrvPoint2d **point)
{
    FREE_T(point, struct SPrvPoint2d);
}

//-----------------------------------------------------------------------

static bool prv_isSamePoint2d(double x1, double y1, double x2, double y2, double prec)
{
    if (CMath::isEqual(x1, x2, prec) == false)
        return false;

    if (CMath::isEqual(y1, y2, prec) == false)
        return false;

    return true;
}

//-----------------------------------------------------------------------

static struct SPrvDataSearchPoint2d *prv_createDataSearch(double x, double y, double prec)
{
    struct SPrvDataSearchPoint2d *dataSearch;

    dataSearch = MALLOC(struct SPrvDataSearchPoint2d);

    dataSearch->x = x;
    dataSearch->y = y;
    dataSearch->prec = prec;

    return dataSearch;
}

//-----------------------------------------------------------------------

static void prv_destroyDataSearch(struct SPrvDataSearchPoint2d **dataSearch)
{
    FREE_T(dataSearch, struct SPrvDataSearchPoint2d);
}

//-----------------------------------------------------------------------

CArrPoint2d::CArrPoint2d(void) : PrvBaseArray(0, (CArray_FPtr_destroyElement)prv_destroyPoint2d)
{
    ;
}

//-----------------------------------------------------------------------

CArrPoint2d::CArrPoint2d(unsigned long numPoints) : PrvBaseArray(numPoints, (CArray_FPtr_destroyElement)prv_destroyPoint2d)
{
    for (unsigned long i = 0; i < numPoints; i++)
    {
        struct SPrvPoint2d *pointZero;

        pointZero = prv_createPoint2d(0., 0.);
        PrvBaseArray::set(i, (struct prv_item_t *)pointZero);
    }
}

//-----------------------------------------------------------------------

CArrPoint2d::CArrPoint2d(const class CArrPoint2d *points) : PrvBaseArray(points, (CArray_FPtr_copyElement)prv_copyPoint2d)
{
    ;
}

//-----------------------------------------------------------------------

class CArrPoint2d *CArrPoint2d::invert(void) const
{
    return (class CArrPoint2d *)PrvBaseArray::invert((CArray_FPtr_copyElement)prv_copyPoint2d);
}

//-----------------------------------------------------------------------

unsigned long CArrPoint2d::size(void) const
{
    return PrvBaseArray::size();
}

//-----------------------------------------------------------------------

void CArrPoint2d::set(unsigned long ind, double x, double y)
{
    struct SPrvPoint2d *point;

    point = (struct SPrvPoint2d *)PrvBaseArray::get(ind);

    point->x = x;
    point->y = y;
}

//-----------------------------------------------------------------------

void CArrPoint2d::add(double x, double y)
{
    struct SPrvPoint2d *point;

    point = prv_createPoint2d(x, y);
    PrvBaseArray::add((struct prv_item_t *)point);
}

//-----------------------------------------------------------------------

void CArrPoint2d::insert(unsigned long ind, double x, double y)
{
    struct SPrvPoint2d *point;

    point = prv_createPoint2d(x, y);
    PrvBaseArray::insert(ind, (struct prv_item_t *)point);
}

//-----------------------------------------------------------------------

void CArrPoint2d::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//-----------------------------------------------------------------------

void CArrPoint2d::get(unsigned long ind, double *x, double *y) const
{
    struct SPrvPoint2d *point;

    assert_no_null(x);
    assert_no_null(y);

    point = (struct SPrvPoint2d *)PrvBaseArray::get(ind);

    *x = point->x;
    *y = point->y;
}

//-----------------------------------------------------------------------

static bool prv_searchPoint(
                        const struct SPrvPoint2d *point,
                        const struct SPrvDataSearchPoint2d *dataSearch)
{
    assert_no_null(point);
    assert_no_null(dataSearch);

    return prv_isSamePoint2d(point->x, point->y, dataSearch->x, dataSearch->y, dataSearch->prec);
}

//-----------------------------------------------------------------------

bool CArrPoint2d::isClosed(double precision) const
{
    const struct SPrvPoint2d *pointFirst, *pointLast;

    pointFirst = (const struct SPrvPoint2d *)PrvBaseArray::get(0);
    pointLast = (const struct SPrvPoint2d *)PrvBaseArray::getLast();

    return prv_isSamePoint2d(pointFirst->x, pointFirst->y, pointLast->x, pointLast->y, precision);
}

//-----------------------------------------------------------------------

bool CArrPoint2d::existPoint2d(double x, double y, double prec, unsigned long *indFirst_optional) const
{
    bool existPoint;
    struct SPrvDataSearchPoint2d *dataSearch;

    dataSearch = prv_createDataSearch(x, y, prec);

    existPoint = PrvBaseArray::exist(
            (const struct prv_itemSearch_t *)dataSearch,
            (CArray_FPtr_search)prv_searchPoint, indFirst_optional);

    prv_destroyDataSearch(&dataSearch);

    return existPoint;
}

//-----------------------------------------------------------------------

float *CArrPoint2d::getArrayC(void) const
{
    float *points;
    unsigned long numPoints;

    numPoints = PrvBaseArray::size();

    if (numPoints > 0)
    {
        points = CALLOC(2 * numPoints, float);

        for (unsigned long i = 0; i < numPoints; i++)
        {
            const struct SPrvPoint2d *point;
            unsigned long indPoint;

            point = (const struct SPrvPoint2d *)PrvBaseArray::get(i);
            assert_no_null(point);

            indPoint = 2 * i;

            points[indPoint] = (float)point->x;
            points[indPoint + 1] = (float)point->y;
        }
    }
    else
        points = NULL;

    return points;
}
