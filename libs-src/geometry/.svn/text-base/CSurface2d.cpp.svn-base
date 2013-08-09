//CSurface2d.cpp: Definición de superficies.

#include "CSurface2d.hpp"

#include "CArray.hpp"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"
#include "CPolylines.hpp"
#include "CStackTransformation.hpp"

#include "asrtbas.h"
#include "memory.h"

struct SDataPrivateSurface2d
{
    class CArray<CArrPoint2d> *contours;
};

//-----------------------------------------------------------------------

static struct SDataPrivateSurface2d *prv_create(class CArray<CArrPoint2d> **contours)
{
    struct SDataPrivateSurface2d *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateSurface2d);

    dataPrivate->contours = ASSIGN_PP_NO_NULL(contours, class CArray<CArrPoint2d>);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct SDataPrivateSurface2d **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->contours;

    FREE_T(dataPrivate, struct SDataPrivateSurface2d);
}

//-----------------------------------------------------------------------

CSurface2d::CSurface2d()
{
    class CArray<CArrPoint2d> *contours;

    contours = new CArray<CArrPoint2d>();
    m_dataPrivate = prv_create(&contours);
}

//-----------------------------------------------------------------------

CSurface2d::~CSurface2d()
{
    prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

static void prv_addPolylineInverted(class CArrPoint2d **polyline, class CArray<CArrPoint2d> *contours)
{
    class CArrPoint2d *polylineInverted;

    assert_no_null(contours);
    assert_no_null(polyline);
    assert_no_null(*polyline);

    polylineInverted = (*polyline)->invert();
    contours->add(polylineInverted);

    delete *polyline;
    *polyline = NULL;
}

//---------------------------------------------------------------

void CSurface2d::appendContour(class CArrPoint2d **polyline)
{
    double area;

    assert_no_null(m_dataPrivate);
    assert_no_null(polyline);
    assert_no_null(*polyline);

    area = CPolylines::areaPolyline(*polyline);

    if (area > 0)
    {
        m_dataPrivate->contours->add(*polyline);
        *polyline = NULL;
    }
    else
        prv_addPolylineInverted(polyline, m_dataPrivate->contours);
}

//---------------------------------------------------------------

void CSurface2d::appendHole(class CArrPoint2d **polyline)
{
    double area;

    assert_no_null(m_dataPrivate);
    assert_no_null(polyline);
    assert_no_null(*polyline);

    area = CPolylines::areaPolyline(*polyline);

    if (area < 0.)
    {
        m_dataPrivate->contours->add(*polyline);
        *polyline = NULL;
    }
    else
        prv_addPolylineInverted(polyline, m_dataPrivate->contours);
}

//---------------------------------------------------------------

class CArray<CArrPoint3d> *CSurface2d::applyTransformation3d(const class CStackTransformation *stackTransformation) const
{
    class CArray<CArrPoint3d> *contours3d;
    unsigned long i, num;

    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->contours);

    num = m_dataPrivate->contours->size();
    contours3d = new CArray<CArrPoint3d>(num);

    for (i = 0; i < num; i++)
    {
        class CArrPoint3d *contour3d;
        const class CArrPoint2d *contour;

        contour = m_dataPrivate->contours->get(i);
        contour3d = stackTransformation->transformationPoints2d(contour);
        contours3d->set(i, contour3d);
    }

    return contours3d;
}
