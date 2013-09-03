/* -----------------------------------------------------------------------------
 *  CGeneratorModel.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGeneratorModel.hpp"

#include "asrtbas.h"

#include "CMesh.hpp"
#include "CGeometry.hpp"
#include "CVector.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "CArraysTypes.hpp"
#include "CArrPoint3d.hpp"
#include "CTesselator.hpp"
#include "CPolylines.hpp"
#include "CImg.hpp"
#include "CMath.hpp"
#include "CScaleColorWithHeight.hpp"

static const double PRV_PRECISION = 1e-5;

// ----------------------------------------------------------------------------

static unsigned long prv_NumColums(const class CArrayRef<CArrPoint3d> *controlPoints)
{
    unsigned long numColums;
    const class CArrPoint3d *points;
    unsigned long numRows;

    numRows = controlPoints->size();
    assert(numRows > 0);

    points = controlPoints->get(0);
    numColums = points->size();

    for (unsigned i = 1; i < numRows; i++)
    {
        unsigned long numColumnsCheck;

        points = controlPoints->get(i);
        numColumnsCheck = points->size();

        assert(numColums == numColumnsCheck);
    }

    return numColums;
}

//-----------------------------------------------------------------------

static CArrPoint3d *prv_jointControlPoints(
                            const class CArrayRef<CArrPoint3d> *controlPoints,
                            unsigned long *numRows,
                            unsigned long *numColumns)
{
    class CArrPoint3d *points;
    unsigned long i, numRows_loc, numColums_loc, numPoints;

    assert_no_null(numRows);
    assert_no_null(numColumns);

    numRows_loc = controlPoints->size();
    numColums_loc = prv_NumColums(controlPoints);

    assert(numRows_loc > 0);
    assert(numColums_loc > 0);

    points = new CArrPoint3d;

    for (i = 0; i < numRows_loc; i++)
    {
        const class CArrPoint3d *pointsColumn;

        pointsColumn = controlPoints->get(i);
        points->concatenate(pointsColumn);
    }

    numPoints = points->size();
    assert(numPoints == numRows_loc * numColums_loc);

    *numRows = numRows_loc;
    *numColumns = numColums_loc;

    return points;
}

// ---------------------------------------------------------------------------

static unsigned long prv_GetPosPointInMesh(
                        unsigned long indRow,
                        unsigned long indColumn,
                        unsigned long numColumns)
{
    return indRow * numColumns + indColumn;
}

// ---------------------------------------------------------------------------

static void prv_sumNormal(class CArrPoint3d *normals, unsigned long indPto, double Nx, double Ny, double Nz)
{
    double NxSum, NySum, NzSum;

    normals->get(indPto, &NxSum, &NySum, &NzSum);

    NxSum += Nx;
    NySum += Ny;
    NzSum += Nz;

    normals->set(indPto, NxSum, NySum, NzSum);
}

// ---------------------------------------------------------------------------

static void prv_SumNormalsTriangle(
                            const CArrPoint3d *points,
                            unsigned long indPto1,
                            unsigned long indPto2,
                            unsigned long indPto3,
                            class CArrPoint3d *normals)
{
    double x1, y1, z1, x2, y2, z2, x3, y3, z3;
    double Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    points->get(indPto1, &x1, &y1, &z1);
    points->get(indPto2, &x2, &y2, &z2);
    points->get(indPto3, &x3, &y3, &z3);

    CVector::twoPointstoVector(x1, y1, z1, x2, y2, z2, &Ux, &Uy, &Uz);
    CVector::twoPointstoVector(x1, y1, z1, x3, y3, z3, &Vx, &Vy, &Vz);

    CVector::crossProduct(Ux, Uy, Uz, Vx, Vy, Vz, &Nx, &Ny, &Nz);

    prv_sumNormal(normals, indPto1, Nx, Ny, Nz);
    prv_sumNormal(normals, indPto2, Nx, Ny, Nz);
    prv_sumNormal(normals, indPto3, Nx, Ny, Nz);
}

// ---------------------------------------------------------------------------

static void prv_calculateNormalsBand(
                            const class CArrPoint3d *points,
                            unsigned long indRow,
                            unsigned long numColumns,
                            class CArrPoint3d *normals)
{
    for (unsigned long i = 0; i < numColumns - 1; i++)
    {
        unsigned long indSigColumn;
        unsigned long indSigRow;
        unsigned long indPto1, indPto2, indPto3, indPto4;

        indSigRow = indRow + 1;
        indSigColumn = i + 1;

        indPto1 = prv_GetPosPointInMesh(indSigRow, i, numColumns);
        indPto2 = prv_GetPosPointInMesh(indRow, i, numColumns);
        indPto3 = prv_GetPosPointInMesh(indSigRow, indSigColumn, numColumns);
        indPto4 = prv_GetPosPointInMesh(indRow, indSigColumn, numColumns);

        prv_SumNormalsTriangle(points, indPto1, indPto2, indPto3, normals);
        prv_SumNormalsTriangle(points, indPto2, indPto4, indPto3, normals);
    }
}

// ---------------------------------------------------------------------------

static void prv_NormalitationVectors(class CArrPoint3d *normals)
{
    unsigned long num;

    num = normals->size();;

    for (unsigned i = 0; i < num; i++)
    {
        double Nx, Ny, Nz;

        normals->get(i, &Nx, &Ny, &Nz);

        if (CVector::module3d(Nx, Ny, Nz) > PRV_PRECISION)
            CVector::toUnitVector(&Nx, &Ny, &Nz);
        else
        {
            Nx = 0.; Ny = 0.; Nz = 1.;
        }

        normals->set(i, Nx, Ny, Nz);
    }
}

// ---------------------------------------------------------------------------

static CArrPoint3d *prv_calculateAllNormals(
                            const class CArrPoint3d *points,
                            unsigned long numRows, unsigned long numColumns)
{
    class CArrPoint3d *normals;
    unsigned long numPoints;

    numPoints = points->size();
    assert(numPoints == numRows * numColumns);

    normals = new CArrPoint3d(numPoints);

    for (unsigned long i = 0; i < numRows - 1; i++)
        prv_calculateNormalsBand(points, i, numColumns, normals);

    prv_NormalitationVectors(normals);

    return normals;
}

// ---------------------------------------------------------------------------

static class CArrayULong *prv_calculateTriangleStripBandRow(
                                unsigned long indRow,
                                unsigned long numRows,
                                unsigned long numColumns)
{
    class CArrayULong *indexsTriangleStripBand;
    unsigned long numIndexsBand;
    unsigned long indRowNext;
    unsigned long indPto1, indPto2;

    numIndexsBand = 2 * numColumns;

    indexsTriangleStripBand = new CArrayULong(numIndexsBand);

    assert(indRow < numRows - 1);
    indRowNext = indRow + 1;

    for (unsigned long i = 0; i < numColumns; i++)
    {
        unsigned long ind;

        ind = 2 * i;

        indPto1 = prv_GetPosPointInMesh(indRowNext, i, numColumns);
        indPto2 = prv_GetPosPointInMesh(indRow, i, numColumns);

        indexsTriangleStripBand->set(ind, indPto1);
        indexsTriangleStripBand->set(ind + 1, indPto2);
    }

    return indexsTriangleStripBand;
}

// ---------------------------------------------------------------------------

static class CArray<CArrayULong> *prv_calculateTrianglesStripBands(
                            unsigned long numRows,
                            unsigned long numColumns)
{
    class CArray<CArrayULong> *indexsVertexsTrianglesStrips;

    assert(numRows > 0);

    indexsVertexsTrianglesStrips = new CArray<CArrayULong>(numRows - 1);

    for (unsigned long i = 0; i < numRows - 1; i++)
    {
        class CArrayULong *indexsTrianglesStripBand;

        indexsTrianglesStripBand = prv_calculateTriangleStripBandRow(i, numRows, numColumns);
        indexsVertexsTrianglesStrips->set(i, indexsTrianglesStripBand);
    }

    return indexsVertexsTrianglesStrips;
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorModel::createWithGrid3d(const class CArrayRef<CArrPoint3d> *controlPoints)
{
    class CMesh *mesh;
    class CArrPoint3d *points, *normals;
    class CArray<CArrayULong> *indexsVertexsTrianglesStrips;
    unsigned long numRows, numColumns;

    points = prv_jointControlPoints(controlPoints, &numRows, &numColumns);
    normals = prv_calculateAllNormals(points, numRows, numColumns);
    indexsVertexsTrianglesStrips = prv_calculateTrianglesStripBands(numRows, numColumns);

    mesh = new CMesh();

    mesh->appendMultipleTriangleStripBands(&points, &normals, &indexsVertexsTrianglesStrips);

    return mesh;
}

// ----------------------------------------------------------------------------

static unsigned long prv_numPointsPolylines(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints)
{
    unsigned long numPolylines, numPoints;
    const class CArrPoint3d *polyline;

    numPolylines = polylinesInPathWithSamePoints->size();
    assert(numPolylines > 1);

    polyline = polylinesInPathWithSamePoints->get(0);
    numPoints = polyline->size();

    for (unsigned long i = 1; i < numPolylines; i++)
    {
        polyline = polylinesInPathWithSamePoints->get(i);
        assert(numPoints == polyline->size());
    }

    return numPoints;
}

// ----------------------------------------------------------------------------

static void prv_coordEdgeBand(
                    const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints,
                    unsigned long indPolyline,
                    unsigned long indEdge,
                    double *x1Edge, double *y1Edge, double *z1Edge,
                    double *x2Edge, double *y2Edge, double *z2Edge)
{
    const CArrPoint3d *polyline;

    polyline = polylinesInPathWithSamePoints->get(indPolyline);

    polyline->get(indEdge, x1Edge, y1Edge, z1Edge);
    polyline->get(indEdge + 1, x2Edge, y2Edge, z2Edge);
}

// ----------------------------------------------------------------------------

static void prv_appendEdge(
                    const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints,
                    unsigned long indPolyline,
                    unsigned long indEdge,
                    class CArrPoint3d *points)
{
    double x1Edge, y1Edge, z1Edge;
    double x2Edge, y2Edge, z2Edge;

    prv_coordEdgeBand(
                    polylinesInPathWithSamePoints, indPolyline, indEdge,
                    &x1Edge, &y1Edge, &z1Edge,
                    &x2Edge, &y2Edge, &z2Edge);

    points->add(x1Edge, y1Edge, z1Edge);
    points->add(x2Edge, y2Edge, z2Edge);
}

// ----------------------------------------------------------------------------

static void prv_appendPointsBand(
                    const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints,
                    unsigned long indEdge,
                    bool isPathClosed,
                    class CArrPoint3d *points,
                    unsigned long *indBeginBand)
{
    unsigned long i, numPolylines;

    assert_no_null(indBeginBand);

    numPolylines = polylinesInPathWithSamePoints->size();
    assert(numPolylines > 1);

    *indBeginBand = points->size();

    for (i = 0; i < numPolylines; i++)
        prv_appendEdge(polylinesInPathWithSamePoints, i, indEdge, points);

    if (isPathClosed == true)
        prv_appendEdge(polylinesInPathWithSamePoints, 0, indEdge, points);
}

// ----------------------------------------------------------------------------

static void prv_calculateNormalPoint(
                        const class CArrPoint3d *points,
                        unsigned long indPto,
                        double *Nx, double *Ny, double *Nz)
{
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

    assert_no_null(Nx);
    assert_no_null(Ny);
    assert_no_null(Nz);

    points->get(indPto, &x1, &y1, &z1);
    points->get(indPto + 1, &x2, &y2, &z2);
    points->get(indPto + 2, &x3, &y3, &z3);

    if (CGeometry::areCollinearPoints(x1, y1, z1, x2, y2, z2, x3, y3, z3, PRV_PRECISION) == false)
    {
        CVector::crossProduct(x2 - x1, y2 - y1, z2 - z1, x3 - x1, y3 - y1, z3 - z1, Nx, Ny, Nz);
    }
    else
    {
        *Nx = 0.;
        *Ny = 0.;
        *Nz = 1.;
    }
}

// ----------------------------------------------------------------------------

static void prv_sumNormalPoint(
                        const class CArrPoint3d *points,
                        unsigned long indPto,
                        double *NxSum, double *NySum, double *NzSum)
{
    double Nx, Ny, Nz;

    assert_no_null(NxSum);
    assert_no_null(NySum);
    assert_no_null(NzSum);

    prv_calculateNormalPoint(points, indPto, &Nx, &Ny, &Nz);

    if (indPto % 2 == 0)
    {
        *NxSum += Nx;
        *NySum += Ny;
        *NzSum += Nz;
    }
    else
    {
        *NxSum -= Nx;
        *NySum -= Ny;
        *NzSum -= Nz;
    }
}

// ----------------------------------------------------------------------------

static void prv_appendNormalsBand(
                    const class CArrPoint3d *points,
                    unsigned long indBeginBand,
                    class CArrPoint3d *normals)
{
    unsigned long numPoints;

    numPoints = points->size();
    assert(numPoints >= 4);
    assert(indBeginBand < numPoints);

    for (unsigned long i = indBeginBand; i < numPoints; i++)
    {
        double NxSum, NySum, NzSum;

        NxSum = 0.;
        NySum = 0.;
        NzSum = 0.;

        if (i < numPoints - 2)
            prv_sumNormalPoint(points, i, &NxSum, &NySum, &NzSum);

        if (i >= indBeginBand + 1 && i < numPoints - 1)
            prv_sumNormalPoint(points, i - 1, &NxSum, &NySum, &NzSum);

        if (i >= indBeginBand + 2)
            prv_sumNormalPoint(points, i - 2, &NxSum, &NySum, &NzSum);

        if (CVector::module3d(NxSum, NySum, NzSum) > PRV_PRECISION)
            CVector::toUnitVector(&NxSum, &NySum, &NzSum);
        else
        {
            unsigned long numNormals;

            numNormals = normals->size();

            if (numNormals > 0)
                normals->getLastPoint(&NxSum, &NySum, &NzSum);
            else
            {
                NxSum = 0.;
                NySum = 0.;
                NzSum = 1.;
            }
        }

        normals->add(NxSum, NySum, NzSum);
    }
}

// ----------------------------------------------------------------------------

static void prv_appendIndexsBand(
                        const class CArrPoint3d *points,
                        unsigned long indBeginBand,
                        class CArray<CArrayULong> *indexTriangleStrip)
{
    unsigned long numPoints, numPointsBand;
    class CArrayULong *indexBand;

    numPoints = points->size();
    assert(numPoints > 0);
    assert(indBeginBand < numPoints);

    numPointsBand = numPoints - indBeginBand;
    indexBand = new CArrayULong(numPointsBand);

    for (unsigned long i = indBeginBand; i < numPoints; i++)
        indexBand->set(i - indBeginBand, i);

    indexTriangleStrip->add(indexBand);
}

// ----------------------------------------------------------------------------

static void prv_appendTriangleStripBand(
                        const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints,
                        unsigned long indEdge,
                        bool isPathClosed,
                        class CArrPoint3d *points,
                        class CArrPoint3d *normals,
                        class CArray<CArrayULong> *indexsTriangleStrip)
{
    unsigned long indBeginBand;

    prv_appendPointsBand(polylinesInPathWithSamePoints,
                        indEdge,
                        isPathClosed,
                        points,
                        &indBeginBand);

    prv_appendNormalsBand(points, indBeginBand, normals);
    prv_appendIndexsBand(points, indBeginBand, indexsTriangleStrip);
}

// ----------------------------------------------------------------------------

static class CMesh *prv_generateMeshByPath(
                    const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints,
                    bool isPathClosed)
{
    class CMesh *mesh;
    unsigned long numPolylines, numPoints, numEdges;
    class CArrPoint3d *points;
    class CArrPoint3d *normals;
    class CArray<CArrayULong> *indexsTriangleStrip;

    numPolylines = polylinesInPathWithSamePoints->size();
    assert(numPolylines > 1);

    numPoints = prv_numPointsPolylines(polylinesInPathWithSamePoints);
    assert(numPoints > 1);

    numEdges = numPoints - 1;

    points = new CArrPoint3d;
    normals = new CArrPoint3d;
    indexsTriangleStrip = new CArray<CArrayULong>;

    for (unsigned long i = 0; i < numEdges; i++)
        prv_appendTriangleStripBand(
                    polylinesInPathWithSamePoints, i, isPathClosed,
                    points, normals, indexsTriangleStrip);

    mesh = new CMesh();
    mesh->appendMultipleTriangleStripBands(&points, &normals, &indexsTriangleStrip);

    return mesh;
}

// ---------------------------------------------------------------------------

static void prv_calculateNormalPolyline(const class CArrPoint3d *polyline, double *Nx, double *Ny, double *Nz)
{
    double A, B, C, D;
    unsigned long numPoints;
    bool thereIsAPlane;

    thereIsAPlane = false;
    A = 0.; B = 0.; C = 0.;

    numPoints = polyline->size();

    for (unsigned long i = 0; i < numPoints; i++)
    {
        double distanceToPlane;
        double x, y, z;

        polyline->get(i, &x, &y, &z);

        if (thereIsAPlane == false)
        {
            double x2, y2, z2, x3, y3, z3;

            polyline->get(i + 1, &x2, &y2, &z2);
            polyline->get(i + 2, &x3, &y3, &z3);

            if (CGeometry::areCollinearPoints(x, y, z, x2, y2, z2, x3, y3, z3, PRV_PRECISION) == false)
            {
                CGeometry::calculateCoefEcuationPlane(x, y, z, x2, y2, z2, x3, y3, z3, &A, &B, &C, &D);
                thereIsAPlane = true;
            }
        }
        else
        {
            distanceToPlane = CGeometry::distanceToPlane(A, B, C, D, x, y, z);
            assert(distanceToPlane < PRV_PRECISION);
        }
    }

    assert(thereIsAPlane == true);

    *Nx = A;
    *Ny = B;
    *Nz = C;

    CVector::toUnitVector(Nx, Ny, Nz);
}

// ---------------------------------------------------------------------------

static void prv_appendTesselationPolylineToMesh(
                        const class CArrPoint3d *polyline,
                        class CMesh *mesh)
{
    class CArrayRef<CArrPoint3d> *contour;
    double Nx, Ny, Nz;
    class CArrPoint3d *points;
    class CArrPoint3d *normals;
    class CArrayULong *indexsTriangles;
    class CArray<CArrayULong> *indexsTriangleStrip;
    class CArray<CArrayULong> *indexsTriangleFan;

    assert_no_null(mesh);

    contour = new CArrayRef<CArrPoint3d>(1);
    contour->set(0, polyline);

    prv_calculateNormalPolyline(polyline, &Nx, &Ny, &Nz);

    CTesselator::tesselationContour(
                        contour,
                        Nx, Ny, Nz,
                        PRV_PRECISION,
                        &points, &normals,
                        &indexsTriangles, &indexsTriangleStrip, &indexsTriangleFan);

    mesh->appendTessetation(&points, &normals, &indexsTriangles, &indexsTriangleStrip, &indexsTriangleFan);

    delete contour;
}

// ---------------------------------------------------------------------------

static class CMesh *prv_calculateMeshCaps(
                        const class CArrPoint3d *firstPolyline,
                        const class CArrPoint3d *lastPolyline)
{
    class CMesh *mesh;

    assert_no_null(firstPolyline);
    assert_no_null(lastPolyline);

    mesh = new CMesh();

    prv_appendTesselationPolylineToMesh(firstPolyline, mesh);
    prv_appendTesselationPolylineToMesh(lastPolyline, mesh);

    return mesh;
}

// ---------------------------------------------------------------------------

static class CMesh *prv_generatePathCaps(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints)
{
    class CMesh *mesh;
    unsigned long numPolylines;
    const class CArrPoint3d *firstPolyline, *lastPolyline;
    class CArrPoint3d *firstPolylineInverted;

    numPolylines = polylinesInPathWithSamePoints->size();
    assert(numPolylines > 1);

    firstPolyline = polylinesInPathWithSamePoints->get(0);
    lastPolyline = polylinesInPathWithSamePoints->get(numPolylines - 1);

    firstPolylineInverted = firstPolyline->invert();

    mesh = prv_calculateMeshCaps(firstPolylineInverted, lastPolyline);

    delete firstPolylineInverted;

    return mesh;
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorModel::createWithPathOpen(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints)
{
    class CMesh *mesh, *meshCaps;
    bool isPathClosed;

    isPathClosed = false;
    meshCaps = prv_generatePathCaps(polylinesInPathWithSamePoints);
    mesh = prv_generateMeshByPath(polylinesInPathWithSamePoints, isPathClosed);

    mesh->concatenate(meshCaps);

    delete meshCaps;

    return mesh;
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorModel::createWithPathOpenWithoutCaps(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints)
{
    bool isPathClosed;

    isPathClosed = false;
    return prv_generateMeshByPath(polylinesInPathWithSamePoints, isPathClosed);
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorModel::createWithPathClosed(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints)
{
    bool isPathClosed;

    isPathClosed = true;
    return prv_generateMeshByPath(polylinesInPathWithSamePoints, isPathClosed);
}

// ----------------------------------------------------------------------------

static double prg_getHeight(
                        const class CImg *image,
                        const class CScaleColorWithHeight *colorsAndHeights,
                        unsigned long indX, unsigned long indY)
{
    unsigned char r, b, g;

    assert_no_null(image);
    assert_no_null(colorsAndHeights);

    image->getPixelRGB(indX, indY, &r, &g, &b);

    return colorsAndHeights->getHeight(r, g, b);
}

// ----------------------------------------------------------------------------

static void prv_appendToMesh(const class CArrPoint3d *points, const class CArrPoint3d *pointsNext, class CMesh *mesh)
{
    class CMesh *meshRow, *meshCaps;
    class CArrayRef<CArrPoint3d> *polylinesInPath;
    bool isPathClosed;

    polylinesInPath = new CArrayRef<CArrPoint3d>;

    polylinesInPath->add(points);
    polylinesInPath->add(pointsNext);

    isPathClosed = false;
    meshRow = prv_generateMeshByPath(polylinesInPath, isPathClosed);
    meshCaps = prv_generatePathCaps(polylinesInPath);

    mesh->concatenate(meshCaps);
    mesh->concatenate(meshRow);

    delete polylinesInPath;
    delete meshRow;
    delete meshCaps;
}

// ----------------------------------------------------------------------------

static void prv_closePoints(double heightGrid, class CArrPoint3d *points)
{
    double xFirst, yFirst, zFirst;
    double xLast, yLast, zLast;

    points->get(0, &xFirst, &yFirst, &zFirst);
    points->getLastPoint(&xLast, &yLast, &zLast);

    points->add(xLast, yLast, -heightGrid);
    points->add(xFirst, yFirst, -heightGrid);
    points->add(xFirst, yFirst, zFirst);
}

// ----------------------------------------------------------------------------

static void prv_appendStripRow(
                        const class CImg *image,
                        const class CScaleColorWithHeight *colorsAndHeights,
                        unsigned long indY,
                        unsigned long widthImage, unsigned long heightImage,
                        double widthGrid, double heightGrid,
                        class CMesh *mesh)
{
    class CArrPoint3d *points, *pointsNext;
    double zPrevious;
    double yRow;
    unsigned long indYImage;

    assert(indY < heightImage);

    zPrevious = 0.;
    points = new CArrPoint3d;

    indYImage = heightImage - 1 - indY;
    yRow = heightGrid * indY;

    for (unsigned long indX = 0; indX < widthImage; indX++)
    {
        double x;
        double z;

        z = prg_getHeight(image, colorsAndHeights, indX, indYImage);

        x = widthGrid * indX;

        if (indX > 0 && CMath::isEqual(z, zPrevious, PRV_PRECISION) == false)
            points->add(x, yRow, zPrevious);

        points->add(x, yRow, z);

        zPrevious = z;
    }

    points->add(widthGrid * widthImage, yRow, zPrevious);

    prv_closePoints(heightGrid, points);

    pointsNext = new CArrPoint3d(points);
    CPolylines::move3d(pointsNext, 0., heightGrid, 0.);

    prv_appendToMesh(points, pointsNext, mesh);

    delete points;
    delete pointsNext;
}

// ----------------------------------------------------------------------------

class CMesh *CGeneratorModel::createGridWithImage(
                            const class CImg *image,
                            const class CScaleColorWithHeight *colorsAndHeights,
                            double widthGrid, double heightGrid)
{
    class CMesh *mesh;
    unsigned long width, height;

    assert_no_null(image);

    image->size(&width, &height);

    mesh = new CMesh;

    for (unsigned long indY = 0; indY < height; indY++)
        prv_appendStripRow(image, colorsAndHeights, indY, width, height, widthGrid, heightGrid, mesh);

    return mesh;
}
