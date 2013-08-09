// Define un relámpago.

#include "CLightning.inl"

#include "CDisplay3DMetereo.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"
#include "CVector.hpp"
#include "CArray.hpp"
#include "CArrPoint3d.hpp"

#include "CCollectionEventsSystem.hpp"
#include "CEventTime.hpp"
#include "CTransform.hpp"
#include "CTransform3D.hpp"
#include "CFigure.hpp"
#include "CMultiObject3d.hpp"

static const unsigned long prv_NUM_FRAMES = 10;
static const double prv_RADIUS_LIGTHNING = 0.1;
static const double prv_HEIGHT_LIGTHNING = 15.;

struct SPrvDataPrivateLightning
{
    double xPos, yPos;
    unsigned long indFrame;

    class CArray<CArrPoint3d> *lightningsGenerated;
};

//---------------------------------------------------------------

static struct SPrvDataPrivateLightning *prv_create(
                        double xPos, double yPos, unsigned long indFrame,
                        class CArray<CArrPoint3d> **lightningsGenerated)
{
    struct SPrvDataPrivateLightning *dataPrivate;
    
    dataPrivate = MALLOC(struct SPrvDataPrivateLightning);
    
    dataPrivate->xPos = xPos;
    dataPrivate->yPos = yPos;
    dataPrivate->indFrame = indFrame;
    dataPrivate->lightningsGenerated = ASSIGN_PP_NO_NULL(lightningsGenerated, class CArray<CArrPoint3d>);
    
    return dataPrivate;
}

//---------------------------------------------------------------

static struct SPrvDataPrivateLightning *prv_createDefault(double xPos, double yPos)
{
    class CArrPoint3d *firstPolyline;
    class CArray<CArrPoint3d> *lightningsGenerated;
    unsigned long indFrame;
    
    firstPolyline = new CArrPoint3d(1);
    firstPolyline->set(0, 0., 0., prv_HEIGHT_LIGTHNING);
    
    lightningsGenerated = new CArray<CArrPoint3d>(1);
    lightningsGenerated->set(0, firstPolyline);

    indFrame = 0;
                        
    return prv_create(xPos, yPos, indFrame, &lightningsGenerated);
}

//---------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateLightning **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);
    assert_no_null((*dataPrivate)->lightningsGenerated);
    
    delete (*dataPrivate)->lightningsGenerated;
    
    FREE_T(dataPrivate, struct SPrvDataPrivateLightning);
}

//---------------------------------------------------------------

CLightning::CLightning(double xPos, double yPos)
{
    m_dataPrivate = prv_createDefault(xPos, yPos);
}

//---------------------------------------------------------------

CLightning::~CLightning()
{
    prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

static void i_generate_point(
                        const class CArrPoint3d *polyline,
                        unsigned long level,
                        double *xGenerated, double *yGenerated, double *zGenerated)
{
    double xLast, yLast, zLast;
    double cos_random, sin_random, radius;

    assert_no_null(polyline);
    assert_no_null(xGenerated);
    assert_no_null(yGenerated);
    assert_no_null(zGenerated);

    polyline->getLastPoint(&xLast, &yLast, &zLast);
    assert(zLast > 0.00001);

    cos_random = 2. * CMath::getRandomNumber() - 1.;
    sin_random = 2. * CMath::getRandomNumber() - 1.;
    radius = (level + 1) * prv_RADIUS_LIGTHNING;

    *xGenerated = radius * cos_random;
    *yGenerated = radius * sin_random;
    *zGenerated = zLast - prv_HEIGHT_LIGTHNING / prv_NUM_FRAMES;
}

//---------------------------------------------------------------

class CAgent *CLightning::evolution(class CCollectionEventsSystem *allEvents)
{
    class CAgent *agentReturned;

    assert_no_null(m_dataPrivate);
    assert_no_null(allEvents);

    if (allEvents->existEventSystem(CEventTime::ID_EVENT, NULL) == true)
    {
        double xGenerated, yGenerated, zGenerated;
        unsigned long numPolylines;

        assert_no_null(m_dataPrivate->lightningsGenerated);

        numPolylines = m_dataPrivate->lightningsGenerated->size();
        assert(numPolylines > 0);

        for (unsigned long i = 0; i < numPolylines; i++)
        {
            class CArrPoint3d *polyline;

            polyline = m_dataPrivate->lightningsGenerated->get(i);

            i_generate_point(polyline, m_dataPrivate->indFrame, &xGenerated, &yGenerated, &zGenerated);
            polyline->add(xGenerated, yGenerated, zGenerated);

            if (m_dataPrivate->indFrame % 3 == 0)
            {
                class CArrPoint3d *newPolyline;

                newPolyline = new CArrPoint3d(1);
                newPolyline->set(0, xGenerated, yGenerated, zGenerated);
                m_dataPrivate->lightningsGenerated->add(newPolyline);
            }
        }

        m_dataPrivate->indFrame++;

        if (m_dataPrivate->indFrame < prv_NUM_FRAMES)
            agentReturned = this;
        else
            agentReturned = NULL;
    }
    else
        agentReturned = this;

    return agentReturned;
}


//---------------------------------------------------------------

static class CAgent *prv_PolylineCilinder(const class CArrPoint3d *points)
{
    class CMultiObject3d *figurePolylineCilinder;
    unsigned long numPoints;

    assert_no_null(points);

    numPoints = points->size();
    assert(numPoints > 1);

    figurePolylineCilinder = new CMultiObject3d();

    for (unsigned long i = 0; i < numPoints - 1; i++)
    {
        double x1, y1, z1, x2, y2, z2;
        double module, Nx, Ny, Nz;
        class CAgent *cilinder, *scale, *extrusion, *translation;

        points->get(i, &x1, &y1, &z1);
        points->get(i + 1, &x2, &y2, &z2);

        CVector::twoPointstoVector(x1, y1, z1, x2, y2, z2, &Nx, &Ny, &Nz);

        module = CVector::module3d(Nx, Ny, Nz);
        CVector::toUnitVector(&Nx, &Ny, &Nz);

        cilinder = new CFigure(CDisplay3DMetereo::SYMBOL_LIGHTING);
        scale = CTransform3D::createScale3D(&cilinder, 0.07, 0.07, module);
        extrusion = CTransform3D::createExtrusion3D(&scale, Nx, Ny, Nz);
        translation = CTransform3D::createTraslate3D(&extrusion, x1, y1, z1);

        figurePolylineCilinder->appendChild(&translation);
    }

    return figurePolylineCilinder;
}

//---------------------------------------------------------------

static class CAgent *prv_createLightningFigure(const class CArray<CArrPoint3d> *lightningsGenerated)
{
    unsigned long num;
    class CMultiObject3d *figureLightning;

    assert_no_null(lightningsGenerated);

    num = lightningsGenerated->size();

    figureLightning = new CMultiObject3d();

    for (unsigned long i = 0; i < num; i++)
    {
        const CArrPoint3d *points;
        unsigned long numPoints;

        points = lightningsGenerated->get(i);

        numPoints = points->size();

        if (numPoints > 1)
        {
            class CAgent *objFigure;

            objFigure = prv_PolylineCilinder(points);
            figureLightning->appendChild(&objFigure);
        }
    }

    return figureLightning;
}

//---------------------------------------------------------------

class CAgent *CLightning::representation(class CTypeDescription *evtDraw)
{
	class CAgent *lightningFigure;
    
    assert_no_null(m_dataPrivate);
    
    lightningFigure = prv_createLightningFigure(m_dataPrivate->lightningsGenerated);
    return CTransform3D::createTraslate3D(&lightningFigure, m_dataPrivate->xPos, m_dataPrivate->yPos, 0.);
}
