/* -----------------------------------------------------------------------------
 *  CTesselator.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CTesselator.hpp"

#include "opengl.inl"

#include "memory.h"
#include "asrtbas.h"

#include "CArray.hpp"
#include "CArrPoint3d.hpp"
#include "CArrayRef.hpp"
#include "CArraysTypes.hpp"

struct PrvBand
{
    PrvBand(GLenum typePrimitive_param, class CArrPoint3d **points_param);
    ~PrvBand(void);

    GLenum typePrimitive;
    class CArrPoint3d *points;
};

struct prv_triangulation_t
{
    GLenum typePrimitiveCurrent;
    class CArrPoint3d *pointsTrianglesCurrent;

    class CArray<PrvBand> *bands;
};

//---------------------------------------------------------------

PrvBand::PrvBand(GLenum typePrimitive_param, class CArrPoint3d **points_param)
{
    typePrimitive = typePrimitive_param;
    points = ASSIGN_PP_NO_NULL(points_param, class CArrPoint3d);
}

// ---------------------------------------------------------------------------

PrvBand::~PrvBand(void)
{
    delete points;
}

// ---------------------------------------------------------------------------

static void APIENTRY prv_beginTriangle(GLenum typePrimitive, void *data)
{
    struct prv_triangulation_t *generationTriangles;

    assert_no_null(data);

    generationTriangles = (struct prv_triangulation_t *)data;
    assert(generationTriangles->pointsTrianglesCurrent == NULL);
    generationTriangles->typePrimitiveCurrent = typePrimitive;
    generationTriangles->pointsTrianglesCurrent = new CArrPoint3d;
}

// ---------------------------------------------------------------------------

static void APIENTRY prv_appendVertex(void *dataPoint, void *data)
{
    GLdouble *point;
    struct prv_triangulation_t *generationTriangles;

    assert_no_null(data);

    generationTriangles = (struct prv_triangulation_t *)data;

    point = (GLdouble *)dataPoint;
    generationTriangles->pointsTrianglesCurrent->add(point[0], point[1], point[2]);
}

// ---------------------------------------------------------------------------

static void APIENTRY prv_isEdge(GLboolean boundaryEdge, void *datos)
{
    //TODO: We have to implement when We want to use edges in openGL;
}

// ---------------------------------------------------------------------------

static void APIENTRY prv_endTriangles(void *data)
{
    struct PrvBand *band;
    struct prv_triangulation_t *generationTriangles;

    assert_no_null(data);

    generationTriangles = (struct prv_triangulation_t *)data;
    assert_no_null(generationTriangles->bands);

    band = new PrvBand(generationTriangles->typePrimitiveCurrent, &generationTriangles->pointsTrianglesCurrent);
    generationTriangles->bands->add(band);
}

// ---------------------------------------------------------------------------

static void APIENTRY prv_combineTriangle(
                        GLdouble coords[3],
                        GLdouble *vertex_data[4],
                        GLfloat weight[4],
                        GLdouble **dataOut)
{
    static GLdouble vertex[6];

    assert_no_null(dataOut);

    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];

    for(unsigned short i = 3; i < 6; i++)
        vertex[i] = 0.;

    *dataOut = vertex;
}

// ---------------------------------------------------------------------------

static void APIENTRY prv_errorTriangle(GLenum typeError)
{
    const char *textError;

    textError = (const char *)gluErrorString(typeError);
    asrtbas_assert(0, "", textError, __FILE__, __LINE__);
}

// ---------------------------------------------------------------------------

static void prv_setupFunction(GLUtesselator *tesselator)
{
    gluTessCallback(tesselator, GLU_TESS_BEGIN_DATA, (_GLUfuncptr)prv_beginTriangle);
    gluTessCallback(tesselator, GLU_TESS_VERTEX_DATA, (_GLUfuncptr)prv_appendVertex);
    gluTessCallback(tesselator, GLU_TESS_EDGE_FLAG_DATA, (_GLUfuncptr)prv_isEdge);
    gluTessCallback(tesselator, GLU_TESS_END_DATA, (_GLUfuncptr)prv_endTriangles);
    gluTessCallback(tesselator, GLU_TESS_COMBINE, (_GLUfuncptr)prv_combineTriangle);
    gluTessCallback(tesselator, GLU_TESS_ERROR, (_GLUfuncptr)prv_errorTriangle);
}

// ---------------------------------------------------------------------------

static void prv_appendPolygon(
                        GLdouble *poligon,
                        unsigned long numPoints,
                        GLUtesselator *tesselator)
{
    gluTessBeginContour(tesselator);

    for (unsigned long i = 0; i < numPoints; i++)
    {
        unsigned long ind;

        ind = 3 * i;
        gluTessVertex(tesselator, &(poligon[ind]), &(poligon[ind]));
    }

    gluTessEndContour(tesselator);
}

// ---------------------------------------------------------------------------

static void prv_destroyPoints(GLdouble ***polygons, unsigned long numPolygons)
{
    unsigned long i;

    assert_no_null(polygons);

    for (i = 0; i < numPolygons; i++)
        FREE_T(&(*polygons)[i], GLdouble);

    FREE_T(polygons, GLdouble *);
}

//---------------------------------------------------------------

static class CArray<PrvBand> *prv_tesselation(const class CArrayRef<CArrPoint3d> *contour, double tolerance)
{
    GLUtesselator *tesselator;
    struct prv_triangulation_t generationTriangles;
    unsigned long num;
    GLdouble **polygons;

    generationTriangles.bands = new CArray<PrvBand>;
    generationTriangles.pointsTrianglesCurrent = NULL;
    generationTriangles.typePrimitiveCurrent = (GLenum)0;

    tesselator = gluNewTess();

    prv_setupFunction(tesselator);

    gluTessProperty(tesselator, GLU_TESS_TOLERANCE, tolerance);

    gluTessBeginPolygon(tesselator, &generationTriangles);

    num = contour->size();
    polygons = CALLOC(num, GLdouble *);

    for (unsigned long i = 0; i < num; i++)
    {
        const class CArrPoint3d *points3d;
        unsigned long numPoints;

        points3d = contour->get(i);
        polygons[i] = points3d->getArrayDoubleC();

        numPoints = points3d->size();

        if (points3d->isClosed(tolerance) == true)
            numPoints--;

        prv_appendPolygon(polygons[i], numPoints, tesselator);
    }

    gluTessEndPolygon(tesselator);

    gluDeleteTess(tesselator);
    prv_destroyPoints(&polygons, num);

    return generationTriangles.bands;
}

//---------------------------------------------------------------

static void prv_appendVertexs(
                    const class CArrPoint3d *pointsBand,
                    double tolerance,
                    class CArrPoint3d *points,
                    class CArrayULong **indexsVertex)
{
    unsigned long numPointsBand;
    class CArrayULong *indexsVertex_loc;

    numPointsBand = pointsBand->size();
    indexsVertex_loc = new CArrayULong(numPointsBand);

    for (unsigned long i = 0; i < numPointsBand; i++)
    {
        double x, y, z;
        unsigned long indPos;

        pointsBand->get(i, &x, &y, &z);

        if (points->existOnlyOnePoint3d(x, y, z, tolerance, &indPos) == false)
        {
            indPos = points->size();
            points->add(x, y, z);
        }

        indexsVertex_loc->set(i, indPos);
    }

    *indexsVertex = indexsVertex_loc;
}

//---------------------------------------------------------------

static void prv_appendQuadsAsTriangles(
                    const class CArrayULong *indexsBand,
                    class CArrayULong *indexsTriangles)
{
    unsigned long numPoints, numQuads;

    assert_no_null(indexsBand);
    assert_no_null(indexsTriangles);

    numPoints = indexsBand->size();
    assert(numPoints % 4 == 0);
    numQuads = numPoints / 4;

    for (unsigned long i = 0; i < numQuads; i++)
    {
        unsigned long indPto0, indPto1, indPto2, indPto3;

        indPto0 = 4 * i;
        indPto1 = indPto1 + 1;
        indPto2 = indPto1 + 2;
        indPto3 = indPto1 + 3;

        indexsTriangles->add(indPto1);
        indexsTriangles->add(indPto0);
        indexsTriangles->add(indPto3);

        indexsTriangles->add(indPto3);
        indexsTriangles->add(indPto2);
        indexsTriangles->add(indPto1);
    }
}

//---------------------------------------------------------------

static class CArrPoint3d *prv_createNormals(
                    const class CArrPoint3d *points,
                    double Nx, double Ny, double Nz)
{
    class CArrPoint3d *normals;
    unsigned long numPoints;

    assert_no_null(points);

    numPoints = points->size();
    normals = new CArrPoint3d(numPoints);

    for (unsigned long i = 0; i < numPoints; i++)
        normals->set(i, Nx, Ny, Nz);

    return normals;
}

//---------------------------------------------------------------

void CTesselator::tesselationContour(
                    const class CArrayRef<CArrPoint3d> *contour,
                    double Nx, double Ny, double Nz,
                    double tolerancePoints,
                    class CArrPoint3d **points,
                    class CArrPoint3d **normals,
                    class CArrayULong **indexsTriangles,
                    class CArray<CArrayULong> **indexsTriangleStrip,
                    class CArray<CArrayULong> **indexsTriangleFan)
{
    class CArray<PrvBand> *bands;
    unsigned long numBands;
    class CArrPoint3d *points_loc, *normals_loc;
    class CArrayULong *indexsTriangles_loc;
    class CArray<CArrayULong> *indexsTriangleStrip_loc;
    class CArray<CArrayULong> *indexsTriangleFan_loc;

    assert_no_null(points);
    assert_no_null(normals);
    assert_no_null(indexsTriangles);
    assert_no_null(indexsTriangleStrip);
    assert_no_null(indexsTriangleFan);

    points_loc = new CArrPoint3d;
    indexsTriangles_loc = new CArrayULong;
    indexsTriangleStrip_loc = new CArray<CArrayULong>;
    indexsTriangleFan_loc = new CArray<CArrayULong>;

    bands = prv_tesselation(contour, tolerancePoints);

    numBands = bands->size();

    for (unsigned long i = 0; i < numBands; i++)
    {
        struct PrvBand *band;
        class CArrayULong *indexsBand;

        band = bands->get(i);

        prv_appendVertexs(band->points, tolerancePoints, points_loc, &indexsBand);

        switch(band->typePrimitive)
        {
            case GL_TRIANGLES:

                indexsTriangles_loc->concatenate(indexsBand);
                break;

            case GL_TRIANGLE_STRIP:

                indexsTriangleStrip_loc->add(indexsBand);
                break;

            case GL_TRIANGLE_FAN:

                indexsTriangleFan_loc->add(indexsBand);
                break;

            case GL_QUADS:

                prv_appendQuadsAsTriangles(indexsBand, indexsTriangles_loc);
                break;

            case GL_QUAD_STRIP:

                indexsTriangleStrip_loc->add(indexsBand);
                break;

            default_error();
        }

        delete indexsBand;
    }

    normals_loc = prv_createNormals(points_loc, Nx, Ny, Nz);

    delete bands;

    *points = points_loc;
    *normals = normals_loc;
    *indexsTriangles = indexsTriangles_loc;
    *indexsTriangleStrip = indexsTriangleStrip_loc;
    *indexsTriangleFan = indexsTriangleFan_loc;
}
