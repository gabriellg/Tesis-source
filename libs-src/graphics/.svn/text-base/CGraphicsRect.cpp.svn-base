/* -----------------------------------------------------------------------------
 *  CGraphicsRect.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGraphicsRect.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"
#include "CString.hpp"
#include "CArray.hpp"
#include "CArrPoint3d.hpp"
#include "CStackTransformation.hpp"

struct prv_dataPrivateGraphicsRect_t
{
    bool hasLimits;
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;

    class CStackTransformation *stackTransforms;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateGraphicsRect_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->stackTransforms);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGraphicsRect_t *prv_create(
                        bool hasLimits,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax,
                        class CStackTransformation **stackTransforms)
{
    struct prv_dataPrivateGraphicsRect_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateGraphicsRect_t);

    dataPrivate->hasLimits = hasLimits;

    dataPrivate->xmin = xmin;
    dataPrivate->ymin = ymin;
    dataPrivate->zmin = zmin;

    dataPrivate->xmax = xmax;
    dataPrivate->ymax = ymax;
    dataPrivate->zmax = zmax;

    dataPrivate->stackTransforms = ASSIGN_PP_NO_NULL(stackTransforms, class CStackTransformation);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGraphicsRect_t *prv_createWithStack(class CStackTransformation **stackTransforms)
{
    bool hasLimits;
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;

    hasLimits = false;

    xmin = ymin = zmin = 1.e10;
    xmax = ymax = zmax = -1.e10;

    return prv_create(hasLimits, xmin, ymin, zmin, xmax, ymax, zmax, stackTransforms);
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGraphicsRect_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->stackTransforms;

    FREE_T(dataPrivate, struct prv_dataPrivateGraphicsRect_t);
}

//-----------------------------------------------------------------------

CGraphicsRect::CGraphicsRect()
{
    class CStackTransformation *stackTransforms;

    stackTransforms = new CStackTransformation();
    m_dataPrivate = prv_createWithStack(&stackTransforms);
}

//-----------------------------------------------------------------------

CGraphicsRect::CGraphicsRect(const class IGraphics *graphicsTypeRect)
{
    class CStackTransformation *stackTransforms;
    const class CGraphicsRect *graphicsRect;

    graphicsRect = dynamic_cast<const class CGraphicsRect *>(graphicsTypeRect);
    assert_no_null(graphicsRect);
    assert_no_null(graphicsRect->m_dataPrivate);

    stackTransforms = new CStackTransformation(graphicsRect->m_dataPrivate->stackTransforms);

    m_dataPrivate = prv_createWithStack(&stackTransforms);
}

//-----------------------------------------------------------------------

CGraphicsRect::~CGraphicsRect()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool CGraphicsRect::getLimits(
                    double *xmin, double *ymin, double *zmin,
                    double *xmax, double *ymax, double *zmax)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(xmin);
    assert_no_null(ymin);
    assert_no_null(zmin);

    assert_no_null(xmax);
    assert_no_null(ymax);
    assert_no_null(zmax);

    if (m_dataPrivate->hasLimits == true)
    {
        *xmin = m_dataPrivate->xmin;
        *ymin = m_dataPrivate->ymin;
        *zmin = m_dataPrivate->zmin;

        *xmax = m_dataPrivate->xmax;
        *ymax = m_dataPrivate->ymax;
        *zmax = m_dataPrivate->zmax;
    }

    return m_dataPrivate->hasLimits;
}

//-----------------------------------------------------------------------

void CGraphicsRect::resetTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->resetTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsRect::pushTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->pushTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsRect::popTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->popTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsRect::traslation(double tx, double ty, double tz)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendTranslation(tx, ty, tz);
}

//-----------------------------------------------------------------------

void CGraphicsRect::rotation(double angleDegrees, double Ux, double Uy, double Uz)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendRotation(angleDegrees, Ux, Uy, Uz);
}

//-----------------------------------------------------------------------

void CGraphicsRect::scale(double sx, double sy, double sz)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendScale(sx, sy, sz);
}

//-----------------------------------------------------------------------

void CGraphicsRect::appendMatrix(const class CMatrix4x4 *matrix)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendMatrix(matrix);
}

//-----------------------------------------------------------------------

static void prv_maxmin_point3d_transformate(
                        const class CStackTransformation *stackTransforms,
                        double x, double y, double z,
                        bool *hayLimites,
                        double *xmin, double *ymin, double *zmin,
                        double *xmax, double *ymax, double *zmax)
{
    double x_trans, y_trans, z_trans;

    assert_no_null(stackTransforms);
    assert_no_null(hayLimites);

    assert_no_null(xmin);
    assert_no_null(xmax);

    assert_no_null(ymin);
    assert_no_null(ymax);

    assert_no_null(zmin);
    assert_no_null(zmax);

    stackTransforms->transformationPoint3d(
                        x, y, z,
                        &x_trans, &y_trans, &z_trans);

    *xmin = MIN(*xmin, x_trans);
    *xmax = MAX(*xmax, x_trans);

    *ymin = MIN(*ymin, y_trans);
    *ymax = MAX(*ymax, y_trans);

    *zmin = MIN(*zmin, z_trans);
    *zmax = MAX(*zmax, z_trans);

    *hayLimites = true;
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
    prv_integrity(m_dataPrivate);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x1, y1, z1,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x2, y2, z2,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawRect(double x1, double y1, double x2, double y2)
{
    prv_integrity(m_dataPrivate);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x1, y1, 0.,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x2, y2, 0.,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);
}

//-----------------------------------------------------------------------

static void prv_drawPolylineAndPolygon(struct prv_dataPrivateGraphicsRect_t *dataPrivate, const class CArrPoint3d *points)
{
    unsigned long num;

    assert_no_null(dataPrivate);
    assert_no_null(points);

    num = points->size();

    for (unsigned long i = 0; i < num; i++)
    {
        double x, y, z;

        points->get(i, &x, &y, &z);
        prv_maxmin_point3d_transformate(
                            dataPrivate->stackTransforms,
                            x, y, z,
                            &dataPrivate->hasLimits,
                            &dataPrivate->xmin, &dataPrivate->ymin, &dataPrivate->zmin,
                            &dataPrivate->xmax, &dataPrivate->ymax, &dataPrivate->zmax);
    }
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawPolyline(const class CArrPoint3d *points)
{
    prv_drawPolylineAndPolygon(m_dataPrivate, points);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawPolygon(const class CArrPoint3d *points)
{
    prv_drawPolylineAndPolygon(m_dataPrivate, points);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawSurface(const class CArray<CArrPoint3d> *contours)
{
    unsigned long numContours;

    assert_no_null(contours);

    numContours = contours->size();

    for (unsigned long i = 0; i < numContours; i++)
    {
        const class CArrPoint3d *points;

        points = contours->get(i);
        prv_drawPolylineAndPolygon(m_dataPrivate, points);
    }
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawQuads(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double x3, double y3, double z3,
                    double x4, double y4, double z4)
{
    prv_integrity(m_dataPrivate);

    prv_maxmin_point3d_transformate(
                    m_dataPrivate->stackTransforms,
                    x1, y1, z1,
                    &m_dataPrivate->hasLimits,
                    &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                    &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                    m_dataPrivate->stackTransforms,
                    x2, y2, z2,
                    &m_dataPrivate->hasLimits,
                    &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                    &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                    m_dataPrivate->stackTransforms,
                    x3, y3, z3,
                    &m_dataPrivate->hasLimits,
                    &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                    &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                    m_dataPrivate->stackTransforms,
                    x4, y4, z4,
                    &m_dataPrivate->hasLimits,
                    &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                    &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);
}

//-----------------------------------------------------------------------

static void prv_drawUnitCube(struct prv_dataPrivateGraphicsRect_t *dataPrivate)
{
    assert_no_null(dataPrivate);

    prv_maxmin_point3d_transformate(
                    dataPrivate->stackTransforms,
                    0., 0., 0.,
                    &dataPrivate->hasLimits,
                    &dataPrivate->xmin, &dataPrivate->ymin, &dataPrivate->zmin,
                    &dataPrivate->xmax, &dataPrivate->ymax, &dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                    dataPrivate->stackTransforms,
                    1., 1., 1.,
                    &dataPrivate->hasLimits,
                    &dataPrivate->xmin, &dataPrivate->ymin, &dataPrivate->zmin,
                    &dataPrivate->xmax, &dataPrivate->ymax, &dataPrivate->zmax);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawUnitCube(enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_integrity(m_dataPrivate);
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawUnitSphere(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawUnitCylinder(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_integrity(m_dataPrivate);
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawUnitCone(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawUnitDisk(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawImageRGB(
                    double x, double y,
                    double width, double height,
                    const class CImg *image)
{
    prv_integrity(m_dataPrivate);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x, y, 0.,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x + width, y + height, 1.,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawText(
                    const char *text,
                    double x, double y,
                    double height)
{
    double lenghtText;

    prv_integrity(m_dataPrivate);

    lenghtText = height * CString::size(text);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x, y, 0.,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);

    prv_maxmin_point3d_transformate(
                        m_dataPrivate->stackTransforms,
                        x + lenghtText, y + height, 1.,
                        &m_dataPrivate->hasLimits,
                        &m_dataPrivate->xmin, &m_dataPrivate->ymin, &m_dataPrivate->zmin,
                        &m_dataPrivate->xmax, &m_dataPrivate->ymax, &m_dataPrivate->zmax);
}

//-----------------------------------------------------------------------

void CGraphicsRect::drawMesh(
                            const class CArrPoint3d *points,
                            const class CArrPoint3d *normals,
                            const class CArrPoint2d *textCoord_opt,
                            const class CArrayULong *indexTriangles,
                            const class CArray<CArrayULong> *indexTriangleFan,
                            const class CArray<CArrayULong> *indexTriangleStrip)
{
    assert_no_null(m_dataPrivate);
    prv_drawPolylineAndPolygon(m_dataPrivate, points);
}
