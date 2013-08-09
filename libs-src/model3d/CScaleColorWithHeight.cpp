/* -----------------------------------------------------------------------------
 *  CScaleColorWithHeight.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CScaleColorWithHeight.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"

struct SPrvColorAndHeight
{
    SPrvColorAndHeight(unsigned char r_param, unsigned char g_param, unsigned char b_param, double height_param);
    ~SPrvColorAndHeight() {;}

    unsigned char r, g, b;
    double height;
};

struct SPrvDataPrivateScaleColorWithHeight
{
    bool hasHeightDefault;
    double heightDefault;

    class CArray<SPrvColorAndHeight> *colorsAndHeights;
};

//-----------------------------------------------------------------------

SPrvColorAndHeight::SPrvColorAndHeight(unsigned char r_param, unsigned char g_param, unsigned char b_param, double height_param)
{
    r = r_param;
    g = g_param;
    b = b_param;

    height = height_param;
}

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateScaleColorWithHeight *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->colorsAndHeights);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateScaleColorWithHeight *prv_createScaleColorWithHeight(
                            bool hasHeightDefault,
                            double heightDefault,
                            class CArray<SPrvColorAndHeight> **colorsAndHeights)
{
    struct SPrvDataPrivateScaleColorWithHeight *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateScaleColorWithHeight);

    dataPrivate->hasHeightDefault = hasHeightDefault;
    dataPrivate->heightDefault = heightDefault;
    dataPrivate->colorsAndHeights = ASSIGN_PP_NO_NULL(colorsAndHeights, class CArray<SPrvColorAndHeight>);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyScaleColorWithHeight(struct SPrvDataPrivateScaleColorWithHeight **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->colorsAndHeights;

    FREE_T(dataPrivate, struct SPrvDataPrivateScaleColorWithHeight);
}

//-----------------------------------------------------------------------

CScaleColorWithHeight::CScaleColorWithHeight()
{
    class CArray<SPrvColorAndHeight> *colorsAndHeights;
    bool hasHeightDefault;
    double heightDefault;

    hasHeightDefault = false;
    heightDefault = 0.;
    colorsAndHeights = new CArray<SPrvColorAndHeight>;

    m_dataPrivate = prv_createScaleColorWithHeight(hasHeightDefault, heightDefault, &colorsAndHeights);
}

//-----------------------------------------------------------------------

CScaleColorWithHeight::CScaleColorWithHeight(double heightDefault)
{
    class CArray<SPrvColorAndHeight> *colorsAndHeights;
    bool hasHeightDefault;

    hasHeightDefault = true;
    colorsAndHeights = new CArray<SPrvColorAndHeight>;

    m_dataPrivate = prv_createScaleColorWithHeight(hasHeightDefault, heightDefault, &colorsAndHeights);
}

//-----------------------------------------------------------------------

CScaleColorWithHeight::~CScaleColorWithHeight()
{
    prv_destroyScaleColorWithHeight(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CScaleColorWithHeight::appendColorWithHeight(unsigned char r, unsigned char g, unsigned char b, double height)
{
    struct SPrvColorAndHeight *colorAndHeight;

    prv_integrity(m_dataPrivate);

    colorAndHeight = new SPrvColorAndHeight(r, g, b, height);
    m_dataPrivate->colorsAndHeights->add(colorAndHeight);
}

//-----------------------------------------------------------------------

static double prv_getHeight(
                        const class CArray<SPrvColorAndHeight> *colorsAndHeights,
                        bool hasHeightDefault,
                        double heightDefault,
                        unsigned char r, unsigned char g, unsigned char b)
{
    unsigned long num;
    bool isFound;
    double heightFound;

    assert_no_null(colorsAndHeights);

    num = colorsAndHeights->size();
    assert(num > 0);

    isFound = false;
    heightFound = 0.;


    for (unsigned long i = 0; i < num; i++)
    {
        const class SPrvColorAndHeight *colorAndHeight;

        colorAndHeight = colorsAndHeights->get(i);
        assert_no_null(colorAndHeight);

        if (colorAndHeight->r == r && colorAndHeight->g == g && colorAndHeight->b == b)
        {
            assert(isFound == false);
            isFound = true;
            heightFound = colorAndHeight->height;
        }
    }

    if (isFound == false)
    {
        assert(hasHeightDefault == true);
        return heightDefault;
    }
    else
    {
        return heightFound;
    }
}

//-----------------------------------------------------------------------

double CScaleColorWithHeight::getHeight(unsigned char r, unsigned char g, unsigned b) const
{
    prv_integrity(m_dataPrivate);

    return prv_getHeight(
                    m_dataPrivate->colorsAndHeights,
                    m_dataPrivate->hasHeightDefault, m_dataPrivate->heightDefault,
                    r, g, b);
}
