/* -----------------------------------------------------------------------------
 *  CGeneratoPolilinesClosed.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CPolylines.hpp"

#include "asrtbas.h"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"
#include "CMath.hpp"
#include "CGeometry.hpp"

static const double PRV_PRECISION = 1e-6;

//---------------------------------------------------------

static void prv_pointElipse(double radioX, double radioY, double angle, double *x, double *y)
{
    assert_no_null(x);
    assert_no_null(y);

    *x = radioX * CMath::cosine(angle);
    *y = radioY * CMath::sine(angle);
}

//------------------------------------------------------------------------------

static class CArrPoint2d *prv_createArcEllipse(
                    double rx, double ry,
                    double angleIni, double angleEnd,
                    unsigned long numPoints)
{
    class CArrPoint2d *polyline;
    double dangle, angle;
    double x, y;

    assert(numPoints > 1);
    polyline = new CArrPoint2d(numPoints + 1);

    dangle = (angleEnd - angleIni) / numPoints;
    angle = angleIni;

    for (unsigned long i = 0; i < numPoints; i++)
    {
        prv_pointElipse(rx, ry, angle, &x, &y);
        polyline->set(i, x, y);

        angle += dangle;
    }

    prv_pointElipse(rx, ry, angleEnd, &x, &y);
    polyline->set(numPoints, x, y);

    return polyline;
}

//------------------------------------------------------------------------------

class CArrPoint2d *CPolylines::createCircle(double radius, unsigned long numPoints, bool counterclockwise)
{
    class CArrPoint2d *polyline;
    double angleIni, angleEnd;

    if (counterclockwise == true)
    {
        angleIni = 0.;
        angleEnd = 2. * CMath::PI;
    }
    else
    {
        angleIni = 2. * CMath::PI;
        angleEnd = 0.;
    }

    polyline = prv_createArcEllipse(radius, radius, angleIni, angleEnd, numPoints);
    assert(polyline->isClosed(PRV_PRECISION) == true);

    return polyline;
}

//------------------------------------------------------------------------------

class CArrPoint2d *CPolylines::createRectangleCentredInOrigin(double width, double height)
{
    class CArrPoint2d *polyline;
    double halfWidth, halfHeight;

    halfWidth = width / 2.;
    halfHeight = height / 2.;

    polyline = new CArrPoint2d(5);

    polyline->set(0, halfWidth, halfHeight);
    polyline->set(1, -halfWidth, halfHeight);
    polyline->set(2, -halfWidth, -halfHeight);
    polyline->set(3, halfWidth, -halfHeight);
    polyline->set(4, halfWidth, halfHeight);

    return polyline;
}

//------------------------------------------------------------------------------

static void prv_closePolyline(class CArrPoint2d *polyline)
{
    double xFirst, yFirst;

    assert_no_null(polyline);
    assert(polyline->isClosed(PRV_PRECISION) == false);

    polyline->get(0, &xFirst, &yFirst);
    polyline->add(xFirst, yFirst);
}

//------------------------------------------------------------------------------

class CArrPoint2d *CPolylines::createPoligonRegular(double radius, unsigned long numEdges)
{
    class CArrPoint2d *polyline;
    double dAngle, angleCurrent;

    dAngle = 2. * CMath::PI / numEdges;
    angleCurrent = 0.;

    polyline = new CArrPoint2d(numEdges);

    for (unsigned long i = 0; i < numEdges; i++)
    {
        double x, y;

        x = radius * CMath::cosine(angleCurrent);
        y = radius * CMath::sine(angleCurrent);

        polyline->set(i, x, y);
        angleCurrent += dAngle;
    }

    prv_closePolyline(polyline);

    return polyline;
}

//------------------------------------------------------------------------------

class CArrPoint3d *CPolylines::createPolyline2dTo3d(const class CArrPoint2d *polyline, double z)
{
    class CArrPoint3d *polyline3d;
    unsigned long numPoints;

    numPoints = polyline->size();
    polyline3d = new CArrPoint3d(numPoints);

    for (unsigned long i = 0; i < numPoints; i++)
    {
        double x, y;

        polyline->get(i, &x, &y);
        polyline3d->set(i, x, y, z);
    }

    return polyline3d;
}

//---------------------------------------------------------

void CPolylines::move2d(class CArrPoint2d *polyline, double dx, double dy)
{
    unsigned long i, num;

    assert_no_null(polyline);
    num = polyline->size();

    for (i = 0; i < num; i++)
    {
        double x, y;

        polyline->get(i, &x, &y);
        polyline->set(i, x + dx, y + dy);
    }
}

//---------------------------------------------------------

void CPolylines::rotate2d(class CArrPoint2d *polyline, double xc, double yc, double Ux, double Uy)
{
    unsigned long numPoints;

    assert_no_null(polyline);

    numPoints = polyline->size();

    for (unsigned long i = 0; i < numPoints; i++)
    {
        double x, y;

        polyline->get(i, &x, &y);

        CGeometry::rotate2D(x, y, xc, yc, Ux, Uy, &x, &x);

        polyline->set(i, x, y);
    }
}

//-----------------------------------------------------------------------

void CPolylines::centerGravity2d(const class CArrPoint2d *polyline, double *xCenterGravity, double *yCenterGravity)
{
    unsigned long numPoints;
    double xSum, ySum;

    assert_no_null(xCenterGravity);
    assert_no_null(yCenterGravity);

    xSum = 0;
    ySum = 0;

    numPoints = polyline->size();
    assert(numPoints > 0);

    for (unsigned long i = 0; i < numPoints; i++)
    {
        double x, y;

        polyline->get(i, &x, &y);

        xSum += x;
        ySum += y;
    }

    *xCenterGravity = xSum / numPoints;
    *yCenterGravity = ySum / numPoints;
}

//-----------------------------------------------------------------------

double CPolylines::areaPolyline(const class CArrPoint2d *polyline)
{
    double area;
    unsigned long numPoints;
    double sum1, sum2;

    numPoints = polyline->size();
    assert(numPoints > 1);
    assert(polyline->isClosed(PRV_PRECISION) == true);

    sum1 = 0.;
    sum2 = 0.;

    for (unsigned long i = 0; i < numPoints - 1; i++)
    {
        double x1, y1, x2, y2;

        polyline->get(i, &x1, &y1);
        polyline->get(i + 1, &x2, &y2);

        sum1 = x1 * y2;
        sum2 = y1 * x2;
    }

    area = (sum1 - sum2) / 2.;

    return area;
}

//-----------------------------------------------------------------------

void CPolylines::move3d(class CArrPoint3d *polyline, double dx, double dy, double dz)
{
    unsigned long num;

    assert_no_null(polyline);

    num = polyline->size();

    for (unsigned long i = 0; i < num; i++)
    {
        double x, y, z;

        polyline->get(i, &x, &y, &z);

        x += dx;
        y += dy;
        z += dz;

        polyline->set(i, x, y, z);
    }
}
