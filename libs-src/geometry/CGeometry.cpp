//CGeometry.cpp:

#include "CGeometry.hpp"

#include "CMath.hpp"
#include "CVector.hpp"

#include "asrtbas.h"

//---------------------------------------------------------------
//
double CGeometry::module2D(double x1, double y1, double x2, double y2)
{
    double dx, dy;

    dx = x2 - x1;
    dy = y2 - y1;

    return CMath::sqrt(dx * dx + dy * dy);
}

//---------------------------------------------------------------
//
void CGeometry::rotate2D(
                    double x, double y,
                    double xCenter, double yCenter,
                    double Ux, double Uy,
                    double *xRotated, double *yRotated)
{
    double dx, dy;

    assert_no_null(xRotated);
    assert_no_null(yRotated);

    dx = x - xCenter;
    dy = y - yCenter;

    *xRotated = dx * Ux - dy * Uy;
    *yRotated = dx * Uy + dy * Ux;

    *xRotated += xCenter;
    *yRotated += yCenter;
}

//---------------------------------------------------------------
//
bool CGeometry::isSamePoint2D(double x1, double y1, double x2, double y2, double precision)
{
    double dx, dy;

    dx = CMath::absolute(x2 - x1);
    dy = CMath::absolute(y2 - y1);

    if (dx > precision)
        return false;

    if (dy > precision)
        return false;

    return true;
}

//---------------------------------------------------------------
//
void CGeometry::traslatePoint2D(double x, double y, double dx, double dy, double Ux, double Uy, double *xtraslate, double *ytraslate)
{
    assert_no_null(xtraslate);
    assert_no_null(ytraslate);

    *xtraslate = x + Ux * dx - Uy * dy;
    *ytraslate = y + Uy * dx + Ux * dy;
}

//---------------------------------------------------------------
//
double CGeometry::angleVectorZeroTo2PiRadians(double x, double y)
{
    double angleRadian;

    angleRadian = CMath::arctan(y, x);

    if (angleRadian < 0.)
        angleRadian += 2. * CMath::PI;

    return angleRadian;
}

//---------------------------------------------------------------
//
double CGeometry::module3D(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2)
{
    double dx, dy, dz;

    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;

    return CMath::sqrt(dx * dx + dy * dy + dz * dz);
}

//---------------------------------------------------------------
//
bool CGeometry::isSamePoint3D(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double precision)
{
    if (CMath::isEqual(x1, x2, precision) == false)
        return false;

    if (CMath::isEqual(y1, y2, precision) == false)
        return false;

    if (CMath::isEqual(z1, z2, precision) == false)
        return false;

    return true;
}

//---------------------------------------------------------------
//

bool CGeometry::areCollinearPoints(
            double x1, double y1, double z1,
            double x2, double y2, double z2,
            double x3, double y3, double z3, double precision)
{
    double Ux, Uy, Uz, Vx, Vy, Vz;
    double moduleU, moduleV;

    CVector::twoPointstoVector(x1, y1, z1, x2, y2, z2, &Ux, &Uy, &Uz);
    CVector::twoPointstoVector(x1, y1, z1, x3, y3, z3, &Vx, &Vy, &Vz);

    moduleU = CVector::module3d(Ux, Uy, Uz);
    moduleV = CVector::module3d(Vx, Vy, Vz);

    if (moduleU > precision && moduleV > precision)
    {
        double dotProduct, dotProductAbs;

        CVector::toUnitVector(&Ux, &Uy, &Uz);
        CVector::toUnitVector(&Vx, &Vy, &Vz);
        dotProduct = CVector::dotProduct(Ux, Uy, Uz, Vx, Vy, Vz);
        dotProductAbs = CMath::absolute(dotProduct);

        if (CMath::isEqual(1., dotProductAbs, precision) == true)
            return true;
        else
            return false;
    }
    else
        return true;

}

//---------------------------------------------------------------
//
void CGeometry::calculateParametricPoint(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double t,
                    double *x, double *y, double *z)
{
    assert_no_null(x);
    assert_no_null(y);
    assert_no_null(z);

    *x = x1 + t * (x2 - x1);
    *y = y1 + t * (y2 - y1);
    *z = z1 + t * (z2 - z1);
}

//---------------------------------------------------------------
//
void CGeometry::extrusion(
                        double Nx, double Ny, double Nz,
                        double *Ux, double *Uy, double *Uz,
                        double *Vx, double *Vy, double *Vz)
{
    double limit;

    assert_no_null(Ux);
    assert_no_null(Uy);
    assert_no_null(Uz);

    assert_no_null(Vx);
    assert_no_null(Vy);
    assert_no_null(Vz);

    limit = 1. / 64.;

    if (CMath::absolute(Nx) < limit && CMath::absolute(Ny) < limit)
    {
        *Vx = 0.;
        *Vy = 1.;
        *Vz = 0.;
    }
    else
    {
        *Vx = 0.;
        *Vy = 0.;
        *Vz = 1.;
    }

    CVector::crossProduct(*Vx, *Vy, *Vz, Nx, Ny, Nz, Ux, Uy, Uz);
    CVector::toUnitVector(Ux, Uy, Uz);

    CVector::crossProduct(Nx, Ny, Nz, *Ux, *Uy, *Uz, Vx, Vy, Vz);
    CVector::toUnitVector(Vx, Vy, Vz);
}

// ----------------------------------------------------------------------------

void CGeometry::horizontalAxisFromThePlaneNormal(
                        double A, double B, double C,
                        double *Ux, double *Uy, double *Uz,
                        double *Vx, double *Vy, double *Vz)
{
    const double prv_PRECISION = 1e-5;

    assert_no_null(Ux);
    assert_no_null(Uy);
    assert_no_null(Uz);

    assert(CVector::isUnitVector(A, B, C, prv_PRECISION) == true);

    if (CMath::absolute(A) < prv_PRECISION && CMath::absolute(B) < prv_PRECISION)
    {
        if (CMath::absolute(C - 1.) < .000001)
            *Ux = 1.;
        else
            *Ux = -1.;

        *Uy = 0.;
        *Uz = 0.;
    }
    else
    {
        *Ux = -B;
        *Uy = A;
        *Uz = 0.;

        CVector::toUnitVector(Ux, Uy, Uz);
    }

    if (Vx != NULL || Vy != NULL || Vz != NULL)
        CVector::crossProduct(A, B, C, *Ux, *Uy, *Uz, Vx, Vy, Vz);
}

//-----------------------------------------------------------------------

void CGeometry::rectangleWithProporcional(
                    double widthWindow, double heigthWindow,
                    double coefPorcBorder,
                    double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax,
                    double *xminOut, double *yminOut, double *zminOut,
                    double *xmaxOut, double *ymaxOut, double *zmaxOut)
{
    double widthBounds, heigthBounds;
    double widthResult, heigthResult, orgX, orgY;
    double borderX, borderY;
    double xMinWithBorder, yMinWithBorder, xMaxWithBorder, yMaxWithBorder;

    assert(widthWindow > 0);
    assert(heigthWindow > 0);

    assert(coefPorcBorder > 0.00001);
    assert(coefPorcBorder < 0.5);

    assert_no_null(xminOut);
    assert_no_null(yminOut);
    assert_no_null(zminOut);

    assert_no_null(xmaxOut);
    assert_no_null(ymaxOut);
    assert_no_null(zmaxOut);

    borderX = coefPorcBorder * (xmax - xmin);
    borderY = coefPorcBorder * (ymax - ymin);

    xMinWithBorder = xmin - borderX;
    yMinWithBorder = ymin - borderY;
    xMaxWithBorder = xmax + borderX;
    yMaxWithBorder = ymax + borderY;

    widthBounds = xMaxWithBorder - xMinWithBorder;
    heigthBounds = yMaxWithBorder - yMinWithBorder;

    if (widthBounds < 0.00001
            || heigthBounds / widthBounds > heigthWindow / widthWindow)
    {
        heigthResult = heigthBounds;
        widthResult = heigthBounds * widthWindow / heigthWindow;

        orgX = (xMaxWithBorder + xMinWithBorder - widthResult) / 2.;
        orgY = yMinWithBorder;
    }
    else
    {
        widthResult = widthBounds;
        heigthResult = widthBounds * heigthWindow / widthWindow ;

        orgX = xMinWithBorder;
        orgY = (yMaxWithBorder + yMinWithBorder - heigthResult) / 2.;
    }

    *xminOut = orgX;
    *yminOut = orgY;
    *zminOut = zmin;

    *xmaxOut = orgX + widthResult;
    *ymaxOut = orgY + heigthResult;
    *zmaxOut = zmax;
}

//-----------------------------------------------------------------------
//
void CGeometry::calculateCenter(
                    double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax,
                    double *xcenter, double *ycenter, double *zcenter)
{
    assert_no_null(xcenter);
    assert_no_null(ycenter);
    assert_no_null(zcenter);

    *xcenter = (xmax + xmin) / 2.;
    *ycenter = (ymax + ymin) / 2.;
    *zcenter = (zmax + zmin) / 2.;
}

//-----------------------------------------------------------------------
//
void CGeometry::calculateCoefEcuationPlane(
                double x1, double y1, double z1,
                double x2, double y2, double z2,
                double x3, double y3, double z3,
                double *A, double *B, double *C, double *D)
{
    double A_loc, B_loc, C_loc, D_loc;
    double Ux, Uy, Uz;
    double Vx, Vy, Vz;

    assert_no_null(A);
    assert_no_null(B);
    assert_no_null(C);
    assert_no_null(D);

    CVector::twoPointstoVector(x1, y1, z1, x2, y2, z2, &Ux, &Uy, &Uz);
    CVector::twoPointstoVector(x1, y1, z1, x3, y3, z3, &Vx, &Vy, &Vz);

    CVector::toUnitVector(&Ux, &Uy, &Uz);
    CVector::toUnitVector(&Vx, &Vy, &Vz);
    CVector::crossProduct(Ux, Uy, Uz, Vx, Vy, Vz, &A_loc, &B_loc, &C_loc);

    D_loc = -A_loc * x1 - B_loc * y1 - C_loc * z1;

    *A = A_loc;
    *B = B_loc;
    *C = C_loc;
    *D = D_loc;
}

//-----------------------------------------------------------------------
//
double CGeometry::distanceToPlane(double A, double B, double C, double D, double x, double y, double z)
{
    return A * x + B * y + C * z + D;
}

// ----------------------------------------------------------------------------

void CGeometry::point3dProjectedTo2D(
                        double Xo, double Yo, double Zo,
                        double Ux, double Uy, double Uz,
                        double Vx, double Vy, double Vz,
                        double x3D, double y3D, double z3D,
                        double *x2D, double *y2D)
{
    double x, y, z;

    assert_no_null(x2D);
    assert_no_null(y2D);

    x = x3D - Xo;
    y = y3D - Yo;
    z = z3D - Zo;

    *x2D = x * Ux + y * Uy + z * Uz;
    *y2D = x * Vx + y * Vy + z * Vz;
}
