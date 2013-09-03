/* -----------------------------------------------------------------------------
 *  CGeneratorSolid.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGeneratorSolid.hpp"

#include "CGeneratorModel.hpp"

#include "asrtbas.h"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "CArraysTypes.hpp"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"
#include "CStackTransformation.hpp"
#include "CPolylines.hpp"
#include "CTesselator.hpp"
#include "CMesh.hpp"
#include "CSurface2d.hpp"

static const double PRV_PRECISION = 1e-5;

// ----------------------------------------------------------------------------

class CMesh *CGeneratorSolid::createSolidByExtrusion(const class CArrPoint2d *polyline, double height)
{
    class CMesh *meshSolid;
    double area;
    class CArrPoint3d *polylineIni, *polylineEnd;
    class CArray<CArrPoint3d> *polylinesInPathWithSamePoints;

    area = CPolylines::areaPolyline(polyline);
    assert(area > 0.);

    polylineIni = CPolylines::createPolyline2dTo3d(polyline, 0.);
    polylineEnd = CPolylines::createPolyline2dTo3d(polyline, height);

    polylinesInPathWithSamePoints = new CArray<CArrPoint3d>(2);
    polylinesInPathWithSamePoints->set(0, polylineIni);
    polylinesInPathWithSamePoints->set(1, polylineEnd);

    meshSolid = CGeneratorModel::createWithPathOpen((const class CArrayRef<CArrPoint3d> *)polylinesInPathWithSamePoints);

    delete polylinesInPathWithSamePoints;

    return meshSolid;
}

// ----------------------------------------------------------------------------

static void prv_appendToMeshTwoPolynes(
                            const class CArrPoint3d *polylineIni,
                            const class CArrPoint3d *polylineEnd,
                            class CMesh *mesh)
{
    class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints;
    class CMesh *meshBody;

    polylinesInPathWithSamePoints = new CArrayRef<CArrPoint3d>(2);
    polylinesInPathWithSamePoints->set(0, polylineIni);
    polylinesInPathWithSamePoints->set(1, polylineEnd);

    meshBody = CGeneratorModel::createWithPathOpenWithoutCaps(polylinesInPathWithSamePoints);
    mesh->concatenate(meshBody);

    delete polylinesInPathWithSamePoints;
    delete meshBody;
}

// ----------------------------------------------------------------------------

static void prv_appendBody(
                    const class CArray<CArrPoint3d> *contourIni,
                    const class CArray<CArrPoint3d> *contourEnd,
                    class CMesh *mesh)
{
    unsigned long numPolylines;

    assert_no_null(contourIni);
    assert_no_null(contourEnd);

    numPolylines = contourIni->size();
    assert(numPolylines == contourEnd->size());

    for (unsigned long i = 0; i < numPolylines; i++)
    {
        const class CArrPoint3d *polylineIni, *polylineEnd;

        polylineIni = contourIni->get(i);
        polylineEnd = contourEnd->get(i);

        prv_appendToMeshTwoPolynes(polylineIni, polylineEnd, mesh);
    }
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorSolid::createSolidWithSurfaceByExtrusion(
                    const class CSurface2d *surface,
                    double height)
{
    class CMesh *mesh;
    class CStackTransformation *stackTransformation;
    class CArray<CArrPoint3d> *contourIni, *contourEnd, *contourIniToBody;
    class CArrPoint3d *pointsIni, *pointsEnd;
    class CArrPoint3d *normalsIni, *normalsEnd;
    class CArrayULong *indexsTrianglesIni, *indexsTrianglesEnd;
    class CArray<CArrayULong> *indexsTriangleStripIni, *indexsTriangleStripEnd;
    class CArray<CArrayULong> *indexsTriangleFanIni, *indexsTriangleFanEnd;

    stackTransformation = new CStackTransformation();

    stackTransformation->pushTransformation();

    stackTransformation->appendExtrusion(0., 0., -1.);
    contourIni = surface->applyTransformation3d(stackTransformation);
    stackTransformation->popTransformation();

    contourIniToBody = surface->applyTransformation3d(stackTransformation);

    stackTransformation->appendTranslation(0., 0., height);
    contourEnd = surface->applyTransformation3d(stackTransformation);

    CTesselator::tesselationContour(
                        (const class CArrayRef<CArrPoint3d> *)contourIni,
                        0., 0., -1.,
                        PRV_PRECISION,
                        &pointsIni, &normalsIni, &indexsTrianglesIni,
                        &indexsTriangleStripIni, &indexsTriangleFanIni);

    CTesselator::tesselationContour(
                        (const class CArrayRef<CArrPoint3d> *)contourEnd,
                        0., 0., 1.,
                        PRV_PRECISION,
                        &pointsEnd, &normalsEnd, &indexsTrianglesEnd,
                        &indexsTriangleStripEnd, &indexsTriangleFanEnd);

    mesh = new CMesh();

    mesh->appendTessetation(
                        &pointsIni, &normalsIni, &indexsTrianglesIni,
                        &indexsTriangleStripIni, &indexsTriangleFanIni);

    mesh->appendTessetation(
                        &pointsEnd, &normalsEnd, &indexsTrianglesEnd,
                        &indexsTriangleStripEnd, &indexsTriangleFanEnd);

    prv_appendBody(contourIniToBody, contourEnd, mesh);

    delete stackTransformation;
    delete contourIniToBody;
    delete contourIni;
    delete contourEnd;

    return mesh;
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorSolid::createUnitCube(void)
{
    class CMesh *mesh;

    mesh = new CMesh;

    mesh->appendQuad(
                0., 0., 0., //x1, y1, z1,
                0., -1., 0., //Nx1, Ny1, Nz1,
                1., 0., 0., //x2, y2, z2,
                0., -1., 0., //Nx1, Ny1, Nz1,
                1., 0., 1., //x3, y3, z3,
                0., -1., 0., //Nx1, Ny1, Nz1,
                0., 0., 1., //x4, y4, z4,
                0., -1., 0.); //Nx1, Ny1, Nz1,);

    mesh->appendQuad(
                0., 1., 0., //x1, y1, z1,
                0., 1., 0., //Nx1, Ny1, Nz1,
                0., 1., 1., //x2, y2, z2,
                0., 1., 0., //Nx2, Ny2, Nz2,
                1., 1., 1., //x3, y3, z3,
                0., 1., 0., //Nx3, Ny3, Nz3,
                1., 1., 0., //x4, y4, z4,
                0., 1., 0.); //Nx4, Ny4, Nz4,

    mesh->appendQuad(
                1., 1., 0., //x1, y1, z1,
                1., 0., 0., //Nx1, Ny1, Nz1,
                1., 1., 1., //x2, y2, z2,
                1., 0., 0., //Nx2, Ny2, Nz2,
                1., 0., 1., //x3, y3, z3,
                1., 0., 0., //Nx3, Ny3, Nz3,
                1., 0., 0., //x4, y4, z4,
                1., 0., 0.); //Nx4, Ny4, Nz4,

    mesh->appendQuad(
                0., 1., 0., //x1, y1, z1,
                -1., 0., 0., //Nx1, Ny1, Nz1,
                0., 0., 0., //x2, y2, z2,
                -1., 0., 0., //Nx2, Ny2, Nz2,
                0., 0., 1., //x3, y3, z3,
                -1., 0., 0., //Nx3, Ny3, Nz3,
                0., 1., 1., //x4, y4, z4,
                -1., 0., 0.); //Nx4, Ny4, Nz4,

    mesh->appendQuad(
                1., 1., 1., //x1, y1, z1,
                0., 0., 1., //Nx1, Ny1, Nz1,
                0., 1., 1., //x2, y2, z2,
                0., 0., 1., //Nx2, Ny2, Nz2,
                0., 0., 1., //x3, y3, z3,
                0., 0., 1., //Nx3, Ny3, Nz3,
                1., 0., 1., //x4, y4, z4,
                0., 0., 1.); //Nx4, Ny4, Nz4,

    mesh->appendQuad(
                0., 1., 0., //x1, y1, z1,
                0., 0., -1., //Nx1, Ny1, Nz1,
                1., 1., 0., //x2, y2, z2,
                0., 0., -1., //Nx2, Ny2, Nz2,
                1., 0., 0., //x3, y3, z3,
                0., 0., -1., //Nx3, Ny3, Nz3,
                0., 0., 0., //x4, y4, z4,
                0., 0., -1.); //Nx4, Ny4, Nz4,

    return mesh;
}

// ----------------------------------------------------------------------------

static class CArrayULong *prv_createIndexTriangleFan(unsigned long numPointsCircle)
{
    class CArrayULong *indexsTriangleFan;

    indexsTriangleFan = new CArrayULong(numPointsCircle);

    for (unsigned long i = 0; i < numPointsCircle; i++)
        indexsTriangleFan->set(i, i);

    return indexsTriangleFan;
}

// ----------------------------------------------------------------------------

static void prv_createPointsCapInFanByPolyline(
                        const class CStackTransformation *stackTransformation,
                        const class CArrPoint2d *polyline,
                        double Nx, double Ny, double Nz,
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrayULong **indexsTriangleFan)
{
    class CArrPoint3d *points_loc, *normals_loc;
    class CArrayULong *indexsTriangleFan_loc;
    unsigned long numPoints;

    assert_no_null(stackTransformation);
    assert_no_null(points);
    assert_no_null(normals);
    assert_no_null(indexsTriangleFan);

    points_loc = stackTransformation->transformationPoints2d(polyline);

    points_loc->insert(0, 0., 0., 0.);
    numPoints = points_loc->size();

    normals_loc = new CArrPoint3d(numPoints, Nx, Ny, Nz);
    indexsTriangleFan_loc = prv_createIndexTriangleFan(numPoints);

    *points = points_loc,
    *normals = normals_loc;
    *indexsTriangleFan = indexsTriangleFan_loc;
}

// ----------------------------------------------------------------------------

static void prv_bodyCylinder(
                    const class CArrPoint2d *circle2dIni,
                    const class CArrPoint2d *circle2dEnd,
                    double height,
                    class CMesh *mesh)
{
    class CArray<CArrPoint3d> *controlPoints;
    class CArrPoint3d *circleIni, *circleEnd;
    class CMesh *meshCylinder;

    circleIni = CPolylines::createPolyline2dTo3d(circle2dIni, 0.);
    circleEnd = CPolylines::createPolyline2dTo3d(circle2dEnd, height);

    controlPoints = new CArray<CArrPoint3d>(2);

    controlPoints->set(0, circleIni);
    controlPoints->set(1, circleEnd);

    meshCylinder = CGeneratorModel::createWithGrid3d((class CArrayRef<CArrPoint3d> *)controlPoints);

    mesh->concatenate(meshCylinder);

    delete controlPoints;
    delete meshCylinder;
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorSolid::createCylinder(
                    double radiusInf, double radiusSup, double height,
                    unsigned long numPointsCilinder)
{
    class CMesh *mesh;
    class CStackTransformation *stackTransformation;
    class CArrPoint2d *circle2dIni, *circle2dEnd;
    class CArrPoint3d *circleCapIni, *normalsIniCap;
    class CArrayULong *indexsTriangleFanCircleIni;
    class CArrPoint3d *circleCapEnd, *normalsEndCap;
    class CArrayULong *indexsTriangleFanCircleEnd;

    circle2dIni = CPolylines::createCircle(radiusInf, numPointsCilinder);
    circle2dEnd = CPolylines::createCircle(radiusSup, numPointsCilinder);

    stackTransformation = new CStackTransformation();

    stackTransformation->pushTransformation();

    stackTransformation->appendExtrusion(0., 0., -1.);

    prv_createPointsCapInFanByPolyline(
                            stackTransformation,
                            circle2dIni,
                            0., 0., -1.,
                            &circleCapIni, &normalsIniCap, &indexsTriangleFanCircleIni);

    stackTransformation->popTransformation();

    stackTransformation->appendTranslation(0., 0., height);

    prv_createPointsCapInFanByPolyline(
                            stackTransformation,
                            circle2dEnd,
                            0., 0., 1.,
                            &circleCapEnd, &normalsEndCap, &indexsTriangleFanCircleEnd);

    mesh = new CMesh();

    mesh->appendTriangleFan(&circleCapIni, &normalsIniCap, &indexsTriangleFanCircleIni);
    mesh->appendTriangleFan(&circleCapEnd, &normalsEndCap, &indexsTriangleFanCircleEnd);

    prv_bodyCylinder(circle2dIni, circle2dEnd, height, mesh);

    delete stackTransformation;
    delete circle2dIni;
    delete circle2dEnd;

    return mesh;
}
