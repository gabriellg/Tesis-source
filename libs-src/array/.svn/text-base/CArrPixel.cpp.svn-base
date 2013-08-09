//CArrPixel.cpp:

#include "CArrPixel.hpp"

#include "CMath.hpp"

#include "asrtbas.h"
#include "memory.h"

struct prv_pixel_t
{
    long x, y;
};

//-----------------------------------------------------------------------

static struct prv_pixel_t *prv_createPixel(long x, long y)
{
    struct prv_pixel_t *pixel;

    pixel = MALLOC(struct prv_pixel_t);

    pixel->x = x;
    pixel->y = y;

    return pixel;
}

//-----------------------------------------------------------------------

static struct prv_pixel_t *prv_copyPixel(const struct prv_pixel_t *pixel)
{
    assert_no_null(pixel);
    return prv_createPixel(pixel->x, pixel->y);
}

//-----------------------------------------------------------------------

static void prv_getPixel(const struct prv_pixel_t *pixel, long *x, long *y)
{
    assert_no_null(pixel);
    assert_no_null(x);
    assert_no_null(y);

    *x = pixel->x;
    *y = pixel->y;
}

//-----------------------------------------------------------------------

static void prv_destroyPixel(struct prv_pixel_t **pixel)
{
    FREE_T(pixel, struct prv_pixel_t);
}

//-----------------------------------------------------------------------
//
CArrPixel::CArrPixel() : PrvBaseArray( 0, (CArray_FPtr_destroyElement)prv_destroyPixel)
{
    ;
}

//-----------------------------------------------------------------------
//
CArrPixel::CArrPixel(unsigned long size) : PrvBaseArray( size, (CArray_FPtr_destroyElement)prv_destroyPixel)
{
    ;
}

//-----------------------------------------------------------------------
//
CArrPixel::CArrPixel(const CArrPixel *array) : PrvBaseArray( array, (CArray_FPtr_copyElement)prv_copyPixel)
{
    ;
}

//-----------------------------------------------------------------------
//
unsigned long CArrPixel::size() const
{
    return PrvBaseArray::size();
}

//-----------------------------------------------------------------------
//
void CArrPixel::get(unsigned long ind, long *x, long *y) const
{
    const struct prv_pixel_t *pixel;

    pixel = (const struct prv_pixel_t *)PrvBaseArray::get(ind);
    prv_getPixel(pixel, x, y);
}

//-----------------------------------------------------------------------
//
void CArrPixel::getLast(long *x, long *y) const
{
    const struct prv_pixel_t *pixel;

    pixel = (const struct prv_pixel_t *)PrvBaseArray::getLast();
    prv_getPixel(pixel, x, y);
}

//-----------------------------------------------------------------------
//
void CArrPixel::set(unsigned long ind, long x, long y)
{
    struct prv_pixel_t *pixel;

    pixel = prv_createPixel(x, y);
    PrvBaseArray::set(ind, (struct prv_item_t *)pixel);
}

//-----------------------------------------------------------------------
//
void CArrPixel::add(long x, long y)
{
    struct prv_pixel_t *pixel;

    pixel = prv_createPixel(x, y);
    PrvBaseArray::add((struct prv_item_t *)pixel);
}

//-----------------------------------------------------------------------
//
void CArrPixel::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//-----------------------------------------------------------------------
//
bool CArrPixel::minMax(long *xMin_opc, long *yMin_opc, long *xMax_opc, long *yMax_opc) const
{
    bool hasMinMax;
    unsigned long num;

    num = PrvBaseArray::size();

    if (num > 0)
    {
        long xMin_loc, yMin_loc, xMax_loc, yMax_loc;

        xMin_loc = CMath::maxLong();
        yMin_loc = CMath::maxLong();
        xMax_loc = CMath::minLong();
        yMax_loc = CMath::minLong();

        for (unsigned long i = 0; i < num; i++)
        {
            const struct prv_pixel_t *pixel;

            pixel = (const struct prv_pixel_t *)PrvBaseArray::get(i);
            xMin_loc = MIN(pixel->x, xMin_loc);
            yMin_loc = MIN(pixel->y, yMin_loc);

            xMax_loc = MAX(pixel->x, xMax_loc);
            yMax_loc = MAX(pixel->y, yMax_loc);
        }

        if (xMin_opc != NULL)
            *xMin_opc = xMin_loc;

        if (xMin_opc != NULL)
            *yMin_opc = yMin_loc;

        if (xMax_opc != NULL)
            *xMax_opc = xMax_loc;

        if (xMax_opc != NULL)
            *yMax_opc = yMax_loc;

        hasMinMax = true;
    }
    else
    {
        hasMinMax = false;
    }

    return hasMinMax;
}

