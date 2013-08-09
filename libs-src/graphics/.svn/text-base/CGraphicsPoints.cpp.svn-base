/* -----------------------------------------------------------------------------
 *  CGraphicsPtos.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGraphicsPoints.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"
#include "CArrPoint3d.hpp"
#include "CStackTransformation.hpp"

struct prv_dataPrivateGraphicsPoints_t
{
    class CArrPoint3d *arr_ptos3d;
    class CStackTransformation *stackTransforms;
};

//-----------------------------------------------------------------------

static void prv_integrity(struct prv_dataPrivateGraphicsPoints_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->arr_ptos3d);
    assert_no_null(dataPrivate->stackTransforms);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGraphicsPoints_t *prv_create(
                            class CArrPoint3d **arr_ptos3d,
                            class CStackTransformation **stackTransforms)
{
    struct prv_dataPrivateGraphicsPoints_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateGraphicsPoints_t);

    dataPrivate->arr_ptos3d = ASSIGN_PP_NO_NULL(arr_ptos3d, class CArrPoint3d);
    dataPrivate->stackTransforms = ASSIGN_PP_NO_NULL(stackTransforms, class CStackTransformation);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGraphicsPoints_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);
    assert((*dataPrivate)->arr_ptos3d == NULL);

    delete (*dataPrivate)->stackTransforms;

    FREE_T(dataPrivate, struct prv_dataPrivateGraphicsPoints_t);
}

//-----------------------------------------------------------------------

CGraphicsPoints::CGraphicsPoints()
{
    class CStackTransformation *stackTransforms;
    class CArrPoint3d *arr_ptos3d;

    arr_ptos3d = new CArrPoint3d();
    stackTransforms = new CStackTransformation();

    m_dataPrivate = prv_create(&arr_ptos3d, &stackTransforms);
}

//-----------------------------------------------------------------------

CGraphicsPoints::~CGraphicsPoints()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::resetTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->resetTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsPoints::pushTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->pushTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsPoints::popTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->popTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsPoints::traslation(double tx, double ty, double tz)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendTranslation(tx, ty, tz);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::rotation(double angleDegrees, double Ux, double Uy, double Uz)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendRotation(angleDegrees, Ux, Uy, Uz);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::scale(double sx, double sy, double sz)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendScale(sx, sy, sz);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::appendMatrix(const class CMatrix4x4 *matrix)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendMatrix(matrix);
}

//-----------------------------------------------------------------------

static void prv_append_point3d_transformate(
                        const class CStackTransformation *stackTransforms,
                        double x, double y, double z,
                        class CArrPoint3d *ptos3d)
{
    double x_trans, y_trans, z_trans;

    assert_no_null(stackTransforms);

    stackTransforms->transformationPoint3d(x, y, z, &x_trans, &y_trans, &z_trans);
    ptos3d->add(x_trans, y_trans, z_trans);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
    prv_integrity(m_dataPrivate);

    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x1, y1, z1, m_dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x2, y2, z2, m_dataPrivate->arr_ptos3d);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawRect(double x1, double y1, double x2, double y2)
{
    prv_integrity(m_dataPrivate);

    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x1, y1, 0., m_dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x2, y2, 0., m_dataPrivate->arr_ptos3d);
}

//-----------------------------------------------------------------------

static void prv_drawPolylineAndPolygon(struct prv_dataPrivateGraphicsPoints_t *dataPrivate, const class CArrPoint3d *points)
{
    unsigned long num;

    assert_no_null(dataPrivate);
    assert_no_null(points);

    num = points->size();

    for (unsigned long i = 0; i < num; i++)
    {
        double x, y, z;

        points->get(i, &x, &y, &z);
        prv_append_point3d_transformate(dataPrivate->stackTransforms, x, y, z, dataPrivate->arr_ptos3d);
    }
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawSurface(const class CArray<CArrPoint3d> *contours)
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

void CGraphicsPoints::drawQuads(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double x3, double y3, double z3,
                    double x4, double y4, double z4)
{
    assert_no_null(m_dataPrivate);

    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x1, y1, z1, m_dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x2, y2, z2, m_dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x3, y3, z3, m_dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x4, y4, z4, m_dataPrivate->arr_ptos3d);
}

//-----------------------------------------------------------------------

static void prv_drawUnitCube(struct prv_dataPrivateGraphicsPoints_t *dataPrivate)
{
    prv_integrity(dataPrivate);

    prv_append_point3d_transformate(dataPrivate->stackTransforms, 0., 0., 0., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 0., 0., 1., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 0., 1., 0., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 0., 1., 1., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 1., 0., 0., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 1., 0., 1., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 1., 1., 0., dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(dataPrivate->stackTransforms, 1., 1., 1., dataPrivate->arr_ptos3d);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawUnitSphere(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawUnitCylinder(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawUnitCone(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawUnitDisk(
                    enum IGraphics::ELevelDetail levelDetail,
                    enum IGraphics::ETypeDrawSolid typeDraw)
{
    prv_drawUnitCube(m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawImageRGB(
                    double x, double y,
                    double width, double height,
                    const class CImg *image)
{
    prv_integrity(m_dataPrivate);

    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x, y, 0.,m_dataPrivate->arr_ptos3d);
    prv_append_point3d_transformate(m_dataPrivate->stackTransforms, x + width, y + height, 1., m_dataPrivate->arr_ptos3d);
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawText(
                    const char *text,
                    double x, double y,
                    double height)
{
    assert_message("Textos no implementados");
}

//-----------------------------------------------------------------------

void CGraphicsPoints::drawMesh(
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

//-----------------------------------------------------------------------

class CArrPoint3d *CGraphicsPoints::getPoints3D(void) const
{
    class CArrPoint3d *arr_ptos3d;

    prv_integrity(m_dataPrivate);

    arr_ptos3d = m_dataPrivate->arr_ptos3d;
    m_dataPrivate->arr_ptos3d = NULL;

    return arr_ptos3d;
}
