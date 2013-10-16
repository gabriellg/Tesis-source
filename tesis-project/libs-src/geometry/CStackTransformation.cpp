//CStackTransformation.cpp:

#include "CStackTransformation.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CMatrix4x4.hpp"
#include "CArray.hpp"
#include "CMath.hpp"
#include "CArrPoint3d.hpp"
#include "CVector.hpp"

struct prv_dataPrivateStackTransformation_t
{
    class CArray<CMatrix4x4> *stack;
    class CMatrix4x4 *matrixCurrent;
};

//-----------------------------------------------------------------------
//
static void prv_integrity(struct prv_dataPrivateStackTransformation_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->stack);
    assert_no_null(dataPrivate->matrixCurrent);
}

//-----------------------------------------------------------------------
//
static struct prv_dataPrivateStackTransformation_t *prv_create(
                        class CArray<CMatrix4x4> **stack,
                        class CMatrix4x4 **matrixCurrent)
{
    struct prv_dataPrivateStackTransformation_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateStackTransformation_t);

    dataPrivate->stack = ASSIGN_PP_NO_NULL(stack, class CArray<CMatrix4x4>);
    dataPrivate->matrixCurrent = ASSIGN_PP_NO_NULL(matrixCurrent, class CMatrix4x4);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------
//
static void prv_destroy(struct prv_dataPrivateStackTransformation_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->stack;
    delete (*dataPrivate)->matrixCurrent;

    FREE_T(dataPrivate, struct prv_dataPrivateStackTransformation_t);
}

//-----------------------------------------------------------------------
//
CStackTransformation::CStackTransformation()
{
    class CArray<CMatrix4x4> *stack;
    class CMatrix4x4 *matrixCurrent;

    stack = new CArray<CMatrix4x4>();
    matrixCurrent = CMatrix4x4::createIdentity();

    m_dataPrivate = prv_create(&stack, &matrixCurrent);
}

//-----------------------------------------------------------------------
//
CStackTransformation::CStackTransformation(const class CStackTransformation *stackTransformation)
{
    class CArray<CMatrix4x4> *stack;
    class CMatrix4x4 *matrixCurrent;

    stack = new CArray<CMatrix4x4>(stackTransformation->m_dataPrivate->stack);
    matrixCurrent = new CMatrix4x4(stackTransformation->m_dataPrivate->matrixCurrent);

    m_dataPrivate = prv_create(&stack, &matrixCurrent);
}

//-----------------------------------------------------------------------
//
CStackTransformation::~CStackTransformation()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::resetTransformation(void)
{
    class CMatrix4x4 *matrixIdentity;

    prv_integrity(m_dataPrivate);

    matrixIdentity = CMatrix4x4::createIdentity();
    delete m_dataPrivate->matrixCurrent;
    m_dataPrivate->matrixCurrent = matrixIdentity;
}

//-----------------------------------------------------------------------
//
void CStackTransformation::pushTransformation(void)
{
    class CMatrix4x4 *matrixCopy;

    prv_integrity(m_dataPrivate);

    matrixCopy = new CMatrix4x4(m_dataPrivate->matrixCurrent);
    m_dataPrivate->stack->add(matrixCopy);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::popTransformation(void)
{
    class CMatrix4x4 *matrix;
    unsigned long numMatrix, indLastPos;

    prv_integrity(m_dataPrivate);

    numMatrix = m_dataPrivate->stack->size();
    assert(numMatrix > 0);

    indLastPos = numMatrix - 1;

    matrix = m_dataPrivate->stack->get(indLastPos);
    delete m_dataPrivate->matrixCurrent;
    m_dataPrivate->matrixCurrent = new CMatrix4x4(matrix);

    m_dataPrivate->stack->erase(indLastPos);
}

//-----------------------------------------------------------------------
//
static void prv_appendTransformation(
                        class CMatrix4x4 **matrixCurrent,
                        class CMatrix4x4 **matrixToAppend)
{
    class CMatrix4x4 *matrixResult;

    assert_no_null(matrixCurrent);
    assert_no_null(*matrixCurrent);
    assert_no_null(*matrixToAppend);

    matrixResult = (*matrixCurrent)->multiplication(*matrixToAppend);

    delete *matrixCurrent;
    *matrixCurrent = matrixResult;

    delete *matrixToAppend;
    *matrixToAppend = NULL;
}

//-----------------------------------------------------------------------
//
void CStackTransformation::appendTranslation(double tx, double ty, double tz)
{
    class CMatrix4x4 *matrixTraslation;

    prv_integrity(m_dataPrivate);

    matrixTraslation = CMatrix4x4::createTraslation(tx, ty, tz);
    prv_appendTransformation(&m_dataPrivate->matrixCurrent, &matrixTraslation);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::appendRotation(double angleDegrees, double Ux, double Uy, double Uz)
{
    class CMatrix4x4 *matrixRotate;
    double angleRotate;

    prv_integrity(m_dataPrivate);

    angleRotate = angleDegrees * CMath::PI / 180.;
    matrixRotate = CMatrix4x4::createRotate(angleRotate, Ux, Uy, Uz);

    prv_appendTransformation(&m_dataPrivate->matrixCurrent, &matrixRotate);

    delete matrixRotate;
}

//-----------------------------------------------------------------------
//
void CStackTransformation::appendAxis(double Ux, double Uy, double Uz, double Vx, double Vy, double Vz)
{
    class CMatrix4x4 *matrixAxis;

    prv_integrity(m_dataPrivate);

    matrixAxis = CMatrix4x4::createAxis(Ux, Uy, Uz, Vx, Vy, Vz);

    prv_appendTransformation(&m_dataPrivate->matrixCurrent, &matrixAxis);

    delete matrixAxis;
}

//-----------------------------------------------------------------------
//
void CStackTransformation::appendExtrusion(double Nx, double Ny, double Nz)
{
    class CMatrix4x4 *matrixExtrusion;

    prv_integrity(m_dataPrivate);

    matrixExtrusion = CMatrix4x4::createExtrusion(Nx, Ny, Nz);

    prv_appendTransformation(&m_dataPrivate->matrixCurrent, &matrixExtrusion);

    delete matrixExtrusion;
}

//-----------------------------------------------------------------------
//
void CStackTransformation::appendScale(double sx, double sy, double sz)
{
    class CMatrix4x4 *matrixScale;

    prv_integrity(m_dataPrivate);

    matrixScale = CMatrix4x4::createScale(sx, sy, sz);
    prv_appendTransformation(&m_dataPrivate->matrixCurrent, &matrixScale);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::appendMatrix(const class CMatrix4x4 *matrix)
{
    class CMatrix4x4 *matrixCopy;

    prv_integrity(m_dataPrivate);

    matrixCopy = new CMatrix4x4(matrix);
    prv_appendTransformation(&m_dataPrivate->matrixCurrent, &matrixCopy);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::getNormal(double *Nx, double *Ny, double *Nz) const
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->matrixCurrent->getNormal(Nx, Ny, Nz);
}

//-----------------------------------------------------------------------
//
class CArrPoint3d *CStackTransformation::transformationPoints2d(const class CArrPoint2d *points) const
{
    prv_integrity(m_dataPrivate);
    return m_dataPrivate->matrixCurrent->transformationPoints2d(points);
}


//-----------------------------------------------------------------------
//
void CStackTransformation::transformationPoints3d(class CArrPoint3d *points) const
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->matrixCurrent->transformationPoints3d(points);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::transformationPoint3d(
                        double x, double y, double z,
                        double *xTrans, double *yTrans, double *zTrans) const
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->matrixCurrent->transformationPoint3d(x, y, z, xTrans, yTrans, zTrans);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::transformationPointF3d(
                        float x, float y, float z,
                        float *xTrans, float *yTrans, float *zTrans) const
{
    double xd, yd, zd;

    prv_integrity(m_dataPrivate);
    assert_no_null(xTrans);
    assert_no_null(yTrans);
    assert_no_null(zTrans);

    xd = x; yd = y; zd = z;

    m_dataPrivate->matrixCurrent->transformationPoint3d(xd, yd, zd, &xd, &yd, &zd);

    *xTrans = (float)xd;
    *yTrans = (float)yd;
    *zTrans = (float)zd;
}

//-----------------------------------------------------------------------
//
void CStackTransformation::transformationUnitVector(
                        double Ux, double Uy, double Uz,
                        double *UxTrans, double *UyTrans, double *UzTrans) const
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->matrixCurrent->transformationUnitVector(Ux, Uy, Uz, UxTrans, UyTrans, UzTrans);
}

//-----------------------------------------------------------------------
//
void CStackTransformation::transformationUnitVectorF(
                        float Ux, float Uy, float Uz,
                        float *UxTrans, float *UyTrans, float *UzTrans) const
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->matrixCurrent->transformationUnitVectorF(Ux, Uy, Uz, UxTrans, UyTrans, UzTrans);
}
