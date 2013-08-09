// Define un array de puntos 3d.

#include "CArrPoint3d.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CMath.hpp"

struct SPrvPoint3d
{
	double x, y, z;
};

struct SPrvDataSearchPoint3d
{
    double x, y, z;
    double prec;
};

//-----------------------------------------------------------------------

static struct SPrvPoint3d *prv_createPoint3d(double x, double y, double z)
{
	struct SPrvPoint3d *point3d;
	
	point3d = MALLOC(struct SPrvPoint3d);
	
	point3d->x = x;
	point3d->y = y;
	point3d->z = z;
	
	return point3d;
}

//-----------------------------------------------------------------------

static struct SPrvPoint3d *prv_copyPoint3d(const struct SPrvPoint3d *point)
{
    assert_no_null(point);
    return prv_createPoint3d(point->x, point->y, point->z);
}

//-----------------------------------------------------------------------

static void prv_destroyPoint3d(struct SPrvPoint3d **point3d)
{
	FREE_T(point3d, struct SPrvPoint3d);
}

//-----------------------------------------------------------------------

static bool prv_isSamePoint3d(double x1, double y1, double z1, double x2, double y2, double z2, double prec)
{
    if (CMath::isEqual(x1, x2, prec) == false)
        return false;

    if (CMath::isEqual(y1, y2, prec) == false)
        return false;

    if (CMath::isEqual(z1, z2, prec) == false)
        return false;

    return true;
}

//-----------------------------------------------------------------------

static struct SPrvDataSearchPoint3d *prv_createDataSearch(double x, double y, double z, double prec)
{
    struct SPrvDataSearchPoint3d *dataSearch;

    dataSearch = MALLOC(struct SPrvDataSearchPoint3d);

    dataSearch->x = x;
    dataSearch->y = y;
    dataSearch->z = z;
    dataSearch->prec = prec;

    return dataSearch;
}

//-----------------------------------------------------------------------

static void prv_destroyDataSearch(struct SPrvDataSearchPoint3d **dataSearch)
{
    FREE_T(dataSearch, struct SPrvDataSearchPoint3d);
}

//-----------------------------------------------------------------------

CArrPoint3d::CArrPoint3d(void) : PrvBaseArray(0, (CArray_FPtr_destroyElement)prv_destroyPoint3d)
{
    ;
}

//-----------------------------------------------------------------------

CArrPoint3d::CArrPoint3d(unsigned long numPoints) : PrvBaseArray(numPoints, (CArray_FPtr_destroyElement)prv_destroyPoint3d)
{
    for (unsigned long i = 0; i < numPoints; i++)
    {
        struct SPrvPoint3d *pointZero;

        pointZero = prv_createPoint3d(0., 0., 0.);
        PrvBaseArray::set(i, (struct prv_item_t *)pointZero);
    }
}

//-----------------------------------------------------------------------

CArrPoint3d::CArrPoint3d(unsigned long numPoints, double x, double y, double z) : PrvBaseArray(numPoints, (CArray_FPtr_destroyElement)prv_destroyPoint3d)
{
    for (unsigned long i = 0; i < numPoints; i++)
    {
        struct SPrvPoint3d *pointZero;

        pointZero = prv_createPoint3d(x, y, z);
        PrvBaseArray::set(i, (struct prv_item_t *)pointZero);
    }
}

//-----------------------------------------------------------------------

CArrPoint3d::CArrPoint3d(const class CArrPoint3d *array) : PrvBaseArray(array, (CArray_FPtr_copyElement)prv_copyPoint3d)
{
    ;
}

//-----------------------------------------------------------------------

class CArrPoint3d *CArrPoint3d::invert(void) const
{
    return (class CArrPoint3d *)PrvBaseArray::invert((CArray_FPtr_copyElement)prv_copyPoint3d);
}

//-----------------------------------------------------------------------

unsigned long CArrPoint3d::size(void) const
{
    return PrvBaseArray::size();
}

//-----------------------------------------------------------------------

void CArrPoint3d::set(unsigned long ind, double x, double y, double z)
{
	struct SPrvPoint3d *point;
	
	point = (struct SPrvPoint3d *)PrvBaseArray::get(ind);

    point->x = x;
    point->y = y;
    point->z = z;
}

//-----------------------------------------------------------------------

void CArrPoint3d::insert(unsigned long ind, double x, double y, double z)
{
    struct SPrvPoint3d *point;

    point = prv_createPoint3d(x, y, z);
    PrvBaseArray::insert(ind, (struct prv_item_t *)point);
}

//-----------------------------------------------------------------------

void CArrPoint3d::add(double x, double y, double z)
{
	struct SPrvPoint3d *point;
	
	point = prv_createPoint3d(x, y, z);
	PrvBaseArray::add((struct prv_item_t *)point);
}

//-----------------------------------------------------------------------

void CArrPoint3d::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//-----------------------------------------------------------------------

void CArrPoint3d::concatenate(const class CArrPoint3d *points)
{
    PrvBaseArray::concatenate(points, (CArray_FPtr_copyElement)prv_copyPoint3d);
}

//-----------------------------------------------------------------------

void CArrPoint3d::get(unsigned long ind, double *x, double *y, double *z) const
{
	struct SPrvPoint3d *point;
	
	assert_no_null(x);
	assert_no_null(y);	
	assert_no_null(z);	
	
	point = (struct SPrvPoint3d *)PrvBaseArray::get(ind);
	
	*x = point->x;
	*y = point->y;
	*z = point->z;
}

//-----------------------------------------------------------------------

void CArrPoint3d::getLastPoint(double *x, double *y, double *z) const
{
    struct SPrvPoint3d *point;
    
    assert_no_null(x);
    assert_no_null(y);  
    assert_no_null(z);  
    
    point = (struct SPrvPoint3d *)PrvBaseArray::getLast();
    
    *x = point->x;
    *y = point->y;
    *z = point->z;
}

//-----------------------------------------------------------------------

bool CArrPoint3d::minMax3D(
                        double *xmin, double *ymin, double *zmin, 
                        double *xmax, double *ymax, double *zmax) const
{
    unsigned long num;
    bool havePoints;
    
    num = PrvBaseArray::size();
    
    if (num > 0)
    {
        unsigned long i;
        double xmin_loc, ymin_loc, zmin_loc; 
        double xmax_loc, ymax_loc, zmax_loc;
        
        havePoints = true;
        
        xmin_loc = 1e20; 
        ymin_loc = 1e20; 
        zmin_loc = 1e20;
         
        xmax_loc = -1e20; 
        ymax_loc = -1e20; 
        zmax_loc = -1e20;
            
        for (i = 0; i < num; i++) 
        {
            const struct SPrvPoint3d *point;
            
            point = (const struct SPrvPoint3d *)PrvBaseArray::get(i);
            assert_no_null(point);
                
            xmin_loc = MIN(xmin_loc, point->x);
            xmax_loc = MAX(xmax_loc, point->x);
            
            ymin_loc = MIN(ymin_loc, point->y);
            ymax_loc = MAX(ymax_loc, point->y);
            
            zmin_loc = MIN(zmin_loc, point->z);
            zmax_loc = MAX(zmax_loc, point->z);
        }
        
        if (xmin != NULL)
            *xmin = xmin_loc;
             
        if (ymin != NULL)
            *ymin = ymin_loc;
             
        if (zmin != NULL)
            *zmin = zmin_loc;
         
        if (xmax != NULL)
            *xmax = xmax_loc;
             
        if (ymax != NULL)
            *ymax = ymax_loc;
            
        if (zmax != NULL)
            *zmax = zmax_loc;
    }
    else
        havePoints = false;
    
    return havePoints;
}

//-----------------------------------------------------------------------

bool CArrPoint3d::isClosed(double precision) const
{
    const struct SPrvPoint3d *pointFirst, *pointLast;

    pointFirst = (const struct SPrvPoint3d *)PrvBaseArray::get(0);
    pointLast = (const struct SPrvPoint3d *)PrvBaseArray::getLast();

    return prv_isSamePoint3d(
                        pointFirst->x, pointFirst->y, pointFirst->z,
                        pointLast->x, pointLast->y, pointLast->z,
                        precision);
}

//-----------------------------------------------------------------------

static bool prv_searchPoint(
                        const struct SPrvPoint3d *point,
                        const struct SPrvDataSearchPoint3d *dataSearch)
{
    assert_no_null(point);
    assert_no_null(dataSearch);

    return prv_isSamePoint3d(point->x, point->y, point->z, dataSearch->x, dataSearch->y, dataSearch->z, dataSearch->prec);
}

//-----------------------------------------------------------------------

bool CArrPoint3d::existPoint3d(double x, double y, double z, double prec, unsigned long *indFirst_optional) const
{
    bool existPoint;
    struct SPrvDataSearchPoint3d *dataSearch;

    dataSearch = prv_createDataSearch(x, y, z, prec);

    existPoint = PrvBaseArray::exist(
            (const struct prv_itemSearch_t *)dataSearch,
            (CArray_FPtr_search)prv_searchPoint, indFirst_optional);

    prv_destroyDataSearch(&dataSearch);

    return existPoint;
}

//-----------------------------------------------------------------------

bool CArrPoint3d::existOnlyOnePoint3d(double x, double y, double z, double prec, unsigned long *indPos) const
{
    bool existPoint;
    struct SPrvDataSearchPoint3d *dataSearch;

    dataSearch = prv_createDataSearch(x, y, z, prec);

    existPoint = PrvBaseArray::existOnlyOneElement(
                    (const struct prv_itemSearch_t *)dataSearch,
                    (CArray_FPtr_search)prv_searchPoint, indPos);

    prv_destroyDataSearch(&dataSearch);

    return existPoint;
}

//-----------------------------------------------------------------------

float *CArrPoint3d::getArrayC(void) const
{
    float *points;
    unsigned long numPoints;

    numPoints = PrvBaseArray::size();

    if (numPoints > 0)
    {
        points = CALLOC(3 * numPoints, float);

        for (unsigned long i = 0; i < numPoints; i++)
        {
            const struct SPrvPoint3d *point;
            unsigned long indPoint;

            point = (const struct SPrvPoint3d *)PrvBaseArray::get(i);
            assert_no_null(point);

            indPoint = 3 * i;

            points[indPoint] = (float)point->x;
            points[indPoint + 1] = (float)point->y;
            points[indPoint + 2] = (float)point->z;
        }
    }
    else
        points = NULL;

    return points;
}

//-----------------------------------------------------------------------

double *CArrPoint3d::getArrayDoubleC(void) const
{
    double *points;
    unsigned long numPoints;

    numPoints = PrvBaseArray::size();

    if (numPoints > 0)
    {
        points = CALLOC(3 * numPoints, double);

        for (unsigned long i = 0; i < numPoints; i++)
        {
            const struct SPrvPoint3d *point;
            unsigned long indPoint;

            point = (const struct SPrvPoint3d *)PrvBaseArray::get(i);
            assert_no_null(point);

            indPoint = 3 * i;

            points[indPoint] = point->x;
            points[indPoint + 1] = point->y;
            points[indPoint + 2] = point->z;
        }
    }
    else
        points = NULL;

    return points;
}
