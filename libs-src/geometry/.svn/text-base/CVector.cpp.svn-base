/* -----------------------------------------------------------------------------
 *  CVector.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CVector.hpp"

#include "asrtbas.h"
#include "CMath.hpp"

//---------------------------------------------------------------
//
void CVector::twoPointstoVector(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double *Ux, double *Uy, double *Uz)
{
    assert_no_null(Ux);
    assert_no_null(Uy);
    assert_no_null(Uz);

    *Ux = x2 - x1;
    *Uy = y2 - y1;
    *Uz = z2 - z1;
}

//---------------------------------------------------------------
//
void CVector::toUnitVector(double *Ux, double *Uy, double *Uz)
{
    double module;

    assert_no_null(Ux);
    assert_no_null(Uy);
    assert_no_null(Uz);

    module = CMath::sqrt(*Ux * *Ux + *Uy * *Uy + *Uz * *Uz);
    assert(module > 1e-10);

    *Ux = *Ux / module;
    *Uy = *Uy / module;
    *Uz = *Uz / module;
}

//---------------------------------------------------------------
//
bool CVector::isUnitVector(double Ux, double Uy, double Uz, double precision)
{
    bool isUnit;
    double module;

    module = Ux * Ux + Uy * Uy + Uz * Uz;

    if (CMath::absolute(module - 1.) < precision)
        isUnit = true;
    else
        isUnit = false;

    return isUnit;
}

//---------------------------------------------------------------
//
double CVector::module3d(double Ux, double Uy, double Uz)
{
    return CMath::sqrt(Ux * Ux + Uy * Uy + Uz * Uz);
}

//---------------------------------------------------------------
//
void CVector::directorVectorAngle(double angle, double *Ux, double *Uy)
{
    assert_no_null(Ux);
    assert_no_null(Uy);

    *Ux = CMath::cosine(angle);
    *Uy = CMath::sine(angle);
}

//---------------------------------------------------------------
//
double CVector::dotProduct(
                    double Ux, double Uy, double Uz,
                    double Vx, double Vy, double Vz)
{
    return Ux * Vx + Uy * Vy + Uz * Vz;
}

//---------------------------------------------------------------
//
void CVector::crossProduct(
                    double Ux, double Uy, double Uz,
                    double Vx, double Vy, double Vz,
                    double *Nx, double *Ny, double *Nz)
{
    double Nx_loc, Ny_loc, Nz_loc;

    assert_no_null(Nx);
    assert_no_null(Ny);
    assert_no_null(Nx);

    Nx_loc = Uy * Vz - Uz * Vy;
    Ny_loc = Uz * Vx - Ux * Vz;
    Nz_loc = Ux * Vy - Uy * Vx;

    assert((Nx_loc * Nx_loc + Ny_loc * Ny_loc + Nz_loc * Nz_loc) > 0.);

    *Nx = Nx_loc;
    *Ny = Ny_loc;
    *Nz = Nz_loc;
}
