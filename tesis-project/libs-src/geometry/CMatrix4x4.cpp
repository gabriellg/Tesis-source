//CMatrix4x4.cpp:

#include "CMatrix4x4.hpp"

#include "CVector.hpp"
#include "CMath.hpp"
#include "CGeometry.hpp"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"

#include "asrtbas.h"
#include "memory.h"

struct SDataPrivateMatrix4x4
{
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;
};

static const double PRV_PRECISION = 1e-5;

//-----------------------------------------------------------------------

static struct SDataPrivateMatrix4x4 *prv_createDataPrivateMatrix4x4(
                        double Ux, double Uy, double Uz, double dx,
                        double Vx, double Vy, double Vz, double dy,
                        double Nx, double Ny, double Nz, double dz,
                        double Wx, double Wy, double Wz, double Wd)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateMatrix4x4);

    dataPrivate->Ux = Ux; dataPrivate->Uy = Uy; dataPrivate->Uz = Uz; dataPrivate->dx = dx;
    dataPrivate->Vx = Vx; dataPrivate->Vy = Vy; dataPrivate->Vz = Vz; dataPrivate->dy = dy;
    dataPrivate->Nx = Nx; dataPrivate->Ny = Ny; dataPrivate->Nz = Nz; dataPrivate->dz = dz;
    dataPrivate->Wx = Wx; dataPrivate->Wy = Wy; dataPrivate->Wz = Wz; dataPrivate->Wd = Wd;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDataPrivateMatrix4x4(struct SDataPrivateMatrix4x4 **dataPrivate)
{
    FREE_T(dataPrivate, struct SDataPrivateMatrix4x4);
}

//-----------------------------------------------------------------------

CMatrix4x4::CMatrix4x4(struct SDataPrivateMatrix4x4 **dataPrivate)
{
    m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct SDataPrivateMatrix4x4);
}

//-----------------------------------------------------------------------

CMatrix4x4::CMatrix4x4(const class CMatrix4x4 *matrix)
{
    assert_no_null(matrix);
    assert_no_null(matrix->m_dataPrivate);

    m_dataPrivate = prv_createDataPrivateMatrix4x4(
                       matrix->m_dataPrivate->Ux, matrix->m_dataPrivate->Uy, matrix->m_dataPrivate->Uz, matrix->m_dataPrivate->dx,
                       matrix->m_dataPrivate->Vx, matrix->m_dataPrivate->Vy, matrix->m_dataPrivate->Vz, matrix->m_dataPrivate->dy,
                       matrix->m_dataPrivate->Nx, matrix->m_dataPrivate->Ny, matrix->m_dataPrivate->Nz, matrix->m_dataPrivate->dz,
                       matrix->m_dataPrivate->Wx, matrix->m_dataPrivate->Wy, matrix->m_dataPrivate->Wz, matrix->m_dataPrivate->Wd);
}

//-----------------------------------------------------------------------

CMatrix4x4::CMatrix4x4(const double matrix[16])
{
    m_dataPrivate = prv_createDataPrivateMatrix4x4(
                       matrix[0], matrix[1], matrix[2], matrix[3],
                       matrix[4], matrix[5], matrix[6], matrix[7],
                       matrix[8], matrix[9], matrix[10], matrix[11],
                       matrix[12], matrix[13], matrix[14], matrix[15]);
}

//-----------------------------------------------------------------------

CMatrix4x4::CMatrix4x4(const float matrix[16])
{
    m_dataPrivate = prv_createDataPrivateMatrix4x4(
                       matrix[0], matrix[1], matrix[2], matrix[3],
                       matrix[4], matrix[5], matrix[6], matrix[7],
                       matrix[8], matrix[9], matrix[10], matrix[11],
                       matrix[12], matrix[13], matrix[14], matrix[15]);
}

//-----------------------------------------------------------------------

CMatrix4x4::~CMatrix4x4()
{
    prv_destroyDataPrivateMatrix4x4(&m_dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createIdentity(void)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;

    Ux = 1.; Uy = 0.; Uz = 0.; dx = 0.;
    Vx = 0.; Vy = 1.; Vz = 0.; dy = 0.;
    Nx = 0.; Ny = 0.; Nz = 1.; dz = 0.;
    Wx = 0.; Wy = 0.; Wz = 0.; Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createTraslation(double tx, double ty, double tz)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;

    Ux = 1.; Uy = 0.; Uz = 0.; dx = tx;
    Vx = 0.; Vy = 1.; Vz = 0.; dy = ty;
    Nx = 0.; Ny = 0.; Nz = 1.; dz = tz;
    Wx = 0.; Wy = 0.; Wz = 0.; Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createScale(double sx, double sy, double sz)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;

    Ux = sx; Uy = 0.; Uz = 0.; dx = 0.;
    Vx = 0.; Vy = sy; Vz = 0.; dy = 0.;
    Nx = 0.; Ny = 0.; Nz = sz; dz = 0.;
    Wx = 0.; Wy = 0.; Wz = 0.; Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createAxis(double Ux, double Uy, double Uz, double Vx, double Vy, double Vz)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double dx, dy, dz;
    double Nx, Ny, Nz;
    double Wx, Wy, Wz, Wd;

    dx = 0.; dy = 0.; dz = 0.;
    Wx = 0.; Wy = 0.; Wz = 0.; Wd = 1.;

    assert(CMath::isZero(CVector::dotProduct(Ux, Uy, Uz, Vx, Vy, Vz), PRV_PRECISION) == true);
    CVector::crossProduct(Ux, Uy, Uz, Vx, Vy, Vz, &Nx, &Ny, &Nz);

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createExtrusion(double Nx, double Ny, double Nz)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double dz;
    double Wx, Wy, Wz, Wd;

    dx = 0.; dy = 0.; dz = 0.;
    Wx = 0.; Wy = 0.; Wz = 0.; Wd = 1.;

    CGeometry::extrusion(Nx, Ny, Nz, &Ux, &Uy, &Uz, &Vx, &Vy, &Vz);

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createRotate(double angleRotate, double UxAxisRotation, double UyAxisRotation, double UzAxisRotation)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;
    double cosine, sine;

    cosine = CMath::cosine(angleRotate);
    sine = CMath::sine(angleRotate);

    Ux = UxAxisRotation * UxAxisRotation * (1 - cosine) + cosine;
    Uy = UxAxisRotation * UyAxisRotation * (1 - cosine) - UzAxisRotation * sine;
    Uz = UxAxisRotation * UzAxisRotation * (1 - cosine) + UyAxisRotation * sine;
    dx = 0.;

    Vx = UyAxisRotation * UxAxisRotation * (1 - cosine) + UzAxisRotation * sine;
    Vy = UyAxisRotation * UyAxisRotation * (1 - cosine) + cosine;
    Vz = UyAxisRotation * UzAxisRotation * (1 - cosine) - UxAxisRotation * sine;
    dy = 0.;

    Nx = UzAxisRotation * UxAxisRotation * (1 - cosine) - UyAxisRotation * sine;
    Ny = UzAxisRotation * UyAxisRotation * (1 - cosine) + UxAxisRotation * sine;
    Nz = UzAxisRotation * UzAxisRotation * (1 - cosine) + cosine;
    dy = 0.;

    dz = 0.;
    Wx = 0.; Wy = 0.; Wz = 0.; Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createRotatePlaneXY(double angleRotateAxisZ)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;
    double coseno, seno;

    coseno = CMath::cosine(angleRotateAxisZ);
    seno = CMath::sine(angleRotateAxisZ);

    Ux = coseno; Uy = seno;    Uz = 0.; dx = 0.;
    Vx = -seno;   Vy = coseno; Vz = 0.; dy = 0.;
    Nx = 0.;     Ny = 0.;      Nz = 1.; dz = 0.;
    Wx = 0.;     Wy = 0.;      Wz = 0.; Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createRotatePlaneYZ(double angleRotateAxisX)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;
    double coseno, seno;

    coseno = CMath::cosine(angleRotateAxisX);
    seno = CMath::sine(angleRotateAxisX);

    Ux = 1.;     Uy = 0.;      Uz = 0.;     dx = 0.;
    Vx = 0;      Vy = coseno;  Vz = seno;  dy = 0.;
    Nx = 0.;     Ny = -seno;    Nz = coseno; dz = 0.;
    Wx = 0.;     Wy = 0.;      Wz = 0.;     Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::createRotatePlaneXZ(double angleRotateAxisY)
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;
    double coseno, seno;

    coseno = CMath::cosine(angleRotateAxisY);
    seno = CMath::sine(angleRotateAxisY);

    Ux = coseno; Uy = 0.;  Uz = -seno;  dx = 0.;
    Vx = 0;    Vy = 1.;  Vz = 0.;     dy = 0.;
    Nx = seno; Ny = 0.;  Nz = coseno; dz = 0.;
    Wx = 0.;   Wy = 0.;  Wz = 0.;     Wd = 1.;

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

static double prv_rowForColumn(
                        double f1, double f2, double f3, double f4,
                        double c1, double c2, double c3, double c4)
{
    return f1 * c1 + f2 *c2 + f3 * c3 + f4 * c4;
}

//-----------------------------------------------------------------------

static void prv_applyTransformation(
                        const struct SDataPrivateMatrix4x4 *dataPrivateMatrix4x4,
                        double x, double y, double z, double w,
                        double *xTrans, double *yTrans, double *zTrans, double *wTrans)
{
    assert_no_null(dataPrivateMatrix4x4);
    assert_no_null(xTrans);
    assert_no_null(yTrans);
    assert_no_null(zTrans);
    assert_no_null(wTrans);

    *xTrans = prv_rowForColumn(dataPrivateMatrix4x4->Ux, dataPrivateMatrix4x4->Uy, dataPrivateMatrix4x4->Uz, dataPrivateMatrix4x4->dx, x, y, z, w);
    *yTrans = prv_rowForColumn(dataPrivateMatrix4x4->Vx, dataPrivateMatrix4x4->Vy, dataPrivateMatrix4x4->Vz, dataPrivateMatrix4x4->dy, x, y, z, w);
    *zTrans = prv_rowForColumn(dataPrivateMatrix4x4->Nx, dataPrivateMatrix4x4->Ny, dataPrivateMatrix4x4->Nz, dataPrivateMatrix4x4->dz, x, y, z, w);
    *wTrans = prv_rowForColumn(dataPrivateMatrix4x4->Wx, dataPrivateMatrix4x4->Wy, dataPrivateMatrix4x4->Wz, dataPrivateMatrix4x4->Wd, x, y, z, w);
}

//-----------------------------------------------------------------------

class CMatrix4x4 *CMatrix4x4::multiplication(const class CMatrix4x4 *matrix) const
{
    struct SDataPrivateMatrix4x4 *dataPrivate;
    double Ux, Uy, Uz, dx;
    double Vx, Vy, Vz, dy;
    double Nx, Ny, Nz, dz;
    double Wx, Wy, Wz, Wd;

    assert_no_null(m_dataPrivate);
    assert_no_null(matrix->m_dataPrivate);

    prv_applyTransformation(
                        m_dataPrivate,
                        matrix->m_dataPrivate->Ux, matrix->m_dataPrivate->Vx, matrix->m_dataPrivate->Nx, matrix->m_dataPrivate->Wx,
                        &Ux, &Vx, &Nx, &Wx);

    prv_applyTransformation(
                        m_dataPrivate,
                        matrix->m_dataPrivate->Uy, matrix->m_dataPrivate->Vy, matrix->m_dataPrivate->Ny, matrix->m_dataPrivate->Wy,
                        &Uy, &Vy, &Ny, &Wy);

    prv_applyTransformation(
                        m_dataPrivate,
                        matrix->m_dataPrivate->Uz, matrix->m_dataPrivate->Vz, matrix->m_dataPrivate->Nz, matrix->m_dataPrivate->Wz,
                        &Uz, &Vz, &Nz, &Wz);

    prv_applyTransformation(
                        m_dataPrivate,
                        matrix->m_dataPrivate->dx, matrix->m_dataPrivate->dy, matrix->m_dataPrivate->dz, matrix->m_dataPrivate->Wd,
                        &dx, &dy, &dz, &Wd);

    dataPrivate = prv_createDataPrivateMatrix4x4(
                Ux, Uy, Uz, dx,
                Vx, Vy, Vz, dy,
                Nx, Ny, Nz, dz,
                Wx, Wy, Wz, Wd);

    return new CMatrix4x4(&dataPrivate);
}

//-----------------------------------------------------------------------

class CArrPoint3d *CMatrix4x4::transformationPoints2d(const class CArrPoint2d *points) const
{
    class CArrPoint3d *points3d;
    unsigned long numPoints;

    assert_no_null(m_dataPrivate);
    assert_no_null(points);

    numPoints = points->size();
    points3d = new CArrPoint3d(numPoints);

    for (unsigned long i = 0; i < numPoints; i++)
    {
        double x, y, z, w;

        points->get(i, &x, &y);
        z = 0.;

        prv_applyTransformation(m_dataPrivate, x, y, z, 1., &x, &y, &z, &w);

        points3d->set(i, x, y, z);
    }

    return points3d;
}

//-----------------------------------------------------------------------

void CMatrix4x4::transformationPoints3d(class CArrPoint3d *points) const
{
    unsigned long numPoints;

    assert_no_null(m_dataPrivate);
    assert_no_null(points);

    numPoints = points->size();

    for (unsigned long i = 0; i < numPoints; i++)
    {
        double x, y, z, w;

        points->get(i, &x, &y, &z);
        prv_applyTransformation(m_dataPrivate, x, y, z, 1., &x, &y, &z, &w);
        points->set(i, x, y, z);
    }
}

//-----------------------------------------------------------------------

void CMatrix4x4::transformationPoint3d(
                        double x, double y, double z,
                        double *xTrans, double *yTrans, double *zTrans) const
{
    double Wd;

    assert_no_null(m_dataPrivate);
    assert_no_null(xTrans);
    assert_no_null(yTrans);
    assert_no_null(zTrans);

    prv_applyTransformation(
                        m_dataPrivate,
                        x, y, z, 1.,
                        xTrans, yTrans, zTrans, &Wd);
}

//-----------------------------------------------------------------------

void CMatrix4x4::transformationPointF3d(
                        float x, float y, float z,
                        float *xTrans, float *yTrans, float *zTrans) const
{
    double Wd;
    double xDouble, yDouble, zDouble;

    assert_no_null(m_dataPrivate);
    assert_no_null(xTrans);
    assert_no_null(yTrans);
    assert_no_null(zTrans);

    prv_applyTransformation(
                        m_dataPrivate,
                        x, y, z, 1.,
                        &xDouble, &yDouble, &zDouble, &Wd);

    *xTrans = (float)xDouble;
    *yTrans = (float)yDouble;
    *zTrans = (float)zDouble;
}

//-----------------------------------------------------------------------
//
static void prv_transformationUnitVector(
                            const struct SDataPrivateMatrix4x4 *dataPrivate,
                            double Ux, double Uy, double Uz,
                            double *UxTrans, double *UyTrans, double *UzTrans)
{
    double UxOriginTrans, UyOriginTrans, UzOriginTrans, WzOriginTrans;
    double UxTransAux, UyTransAux, UzTransAux, WzTransUuax;

    prv_applyTransformation(dataPrivate, 0., 0., 0., 1., &UxOriginTrans, &UyOriginTrans, &UzOriginTrans, &WzOriginTrans);
    prv_applyTransformation(dataPrivate, Ux, Uy, Uz, 1., &UxTransAux, &UyTransAux, &UzTransAux, &WzTransUuax);

    CVector::twoPointstoVector(
                    UxOriginTrans, UyOriginTrans, UzOriginTrans,
                    UxTransAux, UyTransAux, UzTransAux,
                    UxTrans, UyTrans, UzTrans);

    CVector::toUnitVector(UxTrans, UyTrans, UzTrans);
}

//-----------------------------------------------------------------------

void CMatrix4x4::transformationUnitVector(
                        double Ux, double Uy, double Uz,
                        double *UxTrans, double *UyTrans, double *UzTrans) const
{
    assert_no_null(m_dataPrivate);
    prv_transformationUnitVector(m_dataPrivate, Ux, Uy, Uz, UxTrans, UyTrans, UzTrans);
}

//-----------------------------------------------------------------------

void CMatrix4x4::transformationUnitVectorF(
                        float Ux, float Uy, float Uz,
                        float *UxTrans, float *UyTrans, float *UzTrans) const
{
    double UxDouble, UyDouble, UzDouble;

    assert_no_null(m_dataPrivate);
    assert_no_null(UxTrans);
    assert_no_null(UyTrans);
    assert_no_null(UzTrans);

    prv_transformationUnitVector(m_dataPrivate, Ux, Uy, Uz, &UxDouble, &UyDouble, &UzDouble);

    *UxTrans = (float)UxDouble;
    *UyTrans = (float)UyDouble;
    *UzTrans = (float)UzDouble;
}

//-----------------------------------------------------------------------

void CMatrix4x4::getMatrix(double valuesMatrix[4][4]) const
{
    assert_no_null(m_dataPrivate);

    valuesMatrix[0][0] = m_dataPrivate->Ux;
    valuesMatrix[0][1] = m_dataPrivate->Uy;
    valuesMatrix[0][2] = m_dataPrivate->Uz;
    valuesMatrix[0][3] = m_dataPrivate->dx;

    valuesMatrix[1][0] = m_dataPrivate->Vx;
    valuesMatrix[1][1] = m_dataPrivate->Vy;
    valuesMatrix[1][2] = m_dataPrivate->Vz;
    valuesMatrix[1][3] = m_dataPrivate->dy;

    valuesMatrix[2][0] = m_dataPrivate->Nx;
    valuesMatrix[2][1] = m_dataPrivate->Ny;
    valuesMatrix[2][2] = m_dataPrivate->Nz;
    valuesMatrix[2][3] = m_dataPrivate->dz;

    valuesMatrix[3][0] = 0.;
    valuesMatrix[3][1] = 0.;
    valuesMatrix[3][2] = 0.;
    valuesMatrix[3][3] = 1.;
}

//---------------------------------------------------------------

void CMatrix4x4::getNormal(double *Nx, double *Ny, double *Nz) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(Nx);
    assert_no_null(Ny);
    assert_no_null(Nz);

    *Nx = m_dataPrivate->Nx;
    *Ny = m_dataPrivate->Ny;
    *Nz = m_dataPrivate->Nz;
}
