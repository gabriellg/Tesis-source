/* -----------------------------------------------------------------------------
 *  CMesh.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CMesh.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"
#include "CArrPoint3d.hpp"
#include "CArrPoint2d.hpp"
#include "CMatrix4x4.hpp"
#include "CArraysTypes.hpp"
#include "CStackTransformation.hpp"
#include "CMath.hpp"
#include "CVector.hpp"
#include "CGeometry.hpp"
#include "IGraphics.hpp"

enum EPrvTypeTexture
{
    PRV_UNDEFINED_TEXTURE,
    PRV_NO_TEXTURE,
    PRV_TEXTURE
};

struct SPrvVertex
{
    SPrvVertex(float parm_x, float parm_y, float parm_z, float parm_Nx, float parm_Ny, float parm_Nz, float parm_texx, float parm_texy);
    SPrvVertex(const struct SPrvVertex *vertex);
    ~SPrvVertex() {;}

    float x, y, z;
    float Nx, Ny, Nz;
    float texx, texy;
};

struct SDataPrivateMesh
{
    enum EPrvTypeTexture typeTexture;

    class CArray<SPrvVertex> *vertexs;

    class CArray<CArrayULong> *indexTriangleFan;
    class CArray<CArrayULong> *indexTriangleStrip;
    class CArrayULong *indexTriangles;
};

static const double PRV_PRECISION = 1e-5;

//-----------------------------------------------------------------------

static void prv_integrityVertex(const struct SPrvVertex *vertex)
{
    assert_no_null(vertex);
    assert(CVector::isUnitVector(vertex->Nx, vertex->Ny, vertex->Nz, PRV_PRECISION) == true);
}

//-----------------------------------------------------------------------

static void prv_initialitionVertex(
                    float parm_x, float parm_y, float parm_z,
                    float parm_Nx, float parm_Ny, float parm_Nz,
                    float parm_texx, float parm_texy,
                    float *x, float *y, float *z,
                    float *Nx, float *Ny, float *Nz,
                    float *texx, float *texy)
{
    assert_no_null(x);
    assert_no_null(y);
    assert_no_null(z);

    assert_no_null(Nx);
    assert_no_null(Ny);
    assert_no_null(Nz);

    assert_no_null(texx);
    assert_no_null(texy);

    *x = parm_x;
    *y = parm_y;
    *z = parm_z;

    *Nx = parm_Nx;
    *Ny = parm_Ny;
    *Nz = parm_Nz;

    *texx = parm_texx;
    *texy = parm_texy;
}

//-----------------------------------------------------------------------

SPrvVertex::SPrvVertex(
                float parm_x, float parm_y, float parm_z,
                float parm_Nx, float parm_Ny, float parm_Nz, float parm_texx, float parm_texy)
{
    prv_initialitionVertex(
                parm_x, parm_y, parm_z,
                parm_Nx, parm_Ny, parm_Nz,
                parm_texx, parm_texy,
                &x, &y, &z,
                &Nx, &Ny, &Nz,
                &texx, &texy);

    prv_integrityVertex(this);
}

//-----------------------------------------------------------------------

SPrvVertex::SPrvVertex(const struct SPrvVertex *vertex)
{
    assert_no_null(vertex);
    prv_initialitionVertex(
                vertex->x, vertex->y, vertex->z,
                vertex->Nx, vertex->Ny, vertex->Nz,
                vertex->texx, vertex->texy,
                &x, &y, &z,
                &Nx, &Ny, &Nz,
                &texx, &texy);
}

//-----------------------------------------------------------------------

static enum EPrvTypeTexture prv_defineTexture(
                    enum EPrvTypeTexture typeCurrentTexture,
                    enum EPrvTypeTexture typeTextureVertex)
{
    enum EPrvTypeTexture typeTexture;

    switch(typeCurrentTexture)
    {
        case PRV_UNDEFINED_TEXTURE:

            typeTexture = typeTextureVertex;
            break;

        case PRV_NO_TEXTURE:

            assert(typeTextureVertex == PRV_NO_TEXTURE);
            typeTexture = typeTextureVertex;
            break;

        case PRV_TEXTURE:

            assert(typeTextureVertex == PRV_NO_TEXTURE);
            typeTexture = typeTextureVertex;
            break;

        default_error();
    }

    return typeTexture;
}

//-----------------------------------------------------------------------

static void prv_integrity(struct SDataPrivateMesh *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->indexTriangles);
    assert_no_null(dataPrivate->indexTriangleFan);
    assert_no_null(dataPrivate->indexTriangleStrip);
    assert_no_null(dataPrivate->vertexs);
}

//-----------------------------------------------------------------------

static struct SDataPrivateMesh *prv_createMesh(
                    enum EPrvTypeTexture typeTexture,
                    class CArray<SPrvVertex> **vertexs,
                    class CArray<CArrayULong> **indexTriangleFan,
                    class CArray<CArrayULong> **indexTriangleStrip,
                    class CArrayULong **indexTriangles)
{
    struct SDataPrivateMesh *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateMesh);

    dataPrivate->typeTexture = typeTexture;
    dataPrivate->vertexs = ASSIGN_PP_NO_NULL(vertexs, CArray<SPrvVertex>);
    dataPrivate->indexTriangleFan = ASSIGN_PP_NO_NULL(indexTriangleFan, class CArray<CArrayULong>);
    dataPrivate->indexTriangleStrip = ASSIGN_PP_NO_NULL(indexTriangleStrip, class CArray<CArrayULong>);
    dataPrivate->indexTriangles = ASSIGN_PP_NO_NULL(indexTriangles, class CArrayULong);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyMesh(struct SDataPrivateMesh **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->vertexs;

    delete (*dataPrivate)->indexTriangleFan;
    delete (*dataPrivate)->indexTriangleStrip;
    delete (*dataPrivate)->indexTriangles;

    FREE_T(dataPrivate, struct SDataPrivateMesh);
}

//-----------------------------------------------------------------------

CMesh::CMesh(void)
{
    class CArray<CArrayULong> *indexTriangleFan;
    class CArray<CArrayULong> *indexTriangleStrip;
    class CArrayULong *indexTriangles;
    class CArray<SPrvVertex> *vertexs;

    vertexs = new CArray<SPrvVertex>;
    indexTriangleFan = new CArray<CArrayULong>;
    indexTriangleStrip = new CArray<CArrayULong>;
    indexTriangles = new CArrayULong;

    m_dataPrivate = prv_createMesh(
                        PRV_UNDEFINED_TEXTURE,
                        &vertexs, &indexTriangleFan, &indexTriangleStrip, &indexTriangles);
}

//-----------------------------------------------------------------------

CMesh::~CMesh()
{
    prv_destroyMesh(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static void prv_appendVertexs(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrPoint2d **texture_opt,
                        class CArray<SPrvVertex> *vertexsMesh)
{
    class CArrPoint3d *points_loc, *normals_loc;
    class CArrPoint2d *texture_loc;
    unsigned long numNewVertexs;

    assert_no_null(points);
    assert_no_null(*points);
    assert_no_null(normals);
    assert_no_null(*normals);

    points_loc = *points;
    normals_loc = *normals;

    if (texture_opt != NULL)
    {
        texture_loc = *texture_opt;
        assert_no_null(texture_loc);
    }
    else
        texture_loc = NULL;

    numNewVertexs = points_loc->size();
    assert(numNewVertexs == normals_loc->size());
    if (texture_loc != NULL)
        assert(numNewVertexs == (*texture_opt)->size());

    for (unsigned long i = 0; i < numNewVertexs; i++)
    {
        struct SPrvVertex *vertex;
        double x, y, z, Nx, Ny, Nz, texx, texy;

        points_loc->get(i, &x, &y, &z);
        normals_loc->get(i, &Nx, &Ny, &Nz);
        if (texture_loc != NULL)
            texture_loc->get(i, &texx, &texy);
        else
        {
            texx = 0.; texy = 0.;
        }

        vertex = new SPrvVertex(x, y, z, Nx, Ny, Nz, texx, texy);
        vertexsMesh->add(vertex);
    }

    delete *points;
    points = NULL;

    delete *normals;
    normals = NULL;

    if (texture_opt != NULL)
    {
        delete *texture_opt;
        texture_opt = NULL;
    }
}

//-----------------------------------------------------------------------

static void prv_concatenateIndexs(
                        const class CArrayULong *indexs,
                        unsigned long indLastVertexs,
                        class CArrayULong *indexMesh)
{
    unsigned long numIndexs;
    class CArrayULong *indexsCopy;

    indexsCopy = new CArrayULong(indexs);

    numIndexs = indexsCopy->size();

    for (unsigned long i = 0; i < numIndexs; i++)
    {
        unsigned long indVertex;

        indVertex = indexsCopy->get(i);
        indexsCopy->set(i, indVertex + indLastVertexs);
    }

    indexMesh->concatenate(indexsCopy);
    delete indexsCopy;
}

//-----------------------------------------------------------------------

static void prv_appendIndexsBand(
                        class CArray<CArrayULong> *indexMesh,
                        unsigned long indLastVertexs,
                        class CArrayULong **indexsBand)
{
    unsigned long numIndexs;
    class CArrayULong *indexsBand_loc;

    assert_no_null(indexsBand);
    assert_no_null(*indexsBand);

    indexsBand_loc = *indexsBand;

    numIndexs = indexsBand_loc->size();

    for (unsigned long i = 0; i < numIndexs; i++)
    {
        unsigned long indVertex;

        indVertex = indexsBand_loc->get(i);
        indexsBand_loc->set(i, indVertex + indLastVertexs);
    }

    indexMesh->add(indexsBand_loc);
    indexsBand = NULL;
}

//-----------------------------------------------------------------------

static void prv_appendAllIndexsBands(
                        const class CArray<CArrayULong> *allIndexBand,
                        class CArray<CArrayULong> *indexMesh,
                        unsigned long indLastVertexs)
{
    unsigned long numBands;

    assert_no_null(allIndexBand);

    numBands = allIndexBand->size();

    for (unsigned long i = 0; i < numBands; i++)
    {
        const class CArrayULong *indexsBand;
        class CArrayULong *indexsBandCopy;

        indexsBand = allIndexBand->get(i);
        indexsBandCopy = new CArrayULong(indexsBand);
        prv_appendIndexsBand(indexMesh, indLastVertexs, &indexsBandCopy);
    }
}

//-----------------------------------------------------------------------

static void prv_appendVertexsAndIndexsBand(
                    class CArrPoint3d **points,
                    class CArrPoint3d **normals,
                    class CArrPoint2d **texture_opt,
                    class CArrayULong **indexsBand,
                    class CArray<SPrvVertex> *vertexsMesh,
                    class CArray<CArrayULong> *indexsBandMesh)
{
    unsigned long indLastVertexs;

    indLastVertexs = vertexsMesh->size();

    prv_appendVertexs(points, normals, texture_opt, vertexsMesh);
    prv_appendIndexsBand(indexsBandMesh, indLastVertexs, indexsBand);
}

//-----------------------------------------------------------------------

void CMesh::concatenate(const class CMesh *mesh)
{
    unsigned long indLastVertexs;

    assert_no_null(mesh);
    prv_integrity(m_dataPrivate);
    prv_integrity(mesh->m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, mesh->m_dataPrivate->typeTexture);

    indLastVertexs = m_dataPrivate->vertexs->size();
    m_dataPrivate->vertexs->concatenate(mesh->m_dataPrivate->vertexs);

    prv_appendAllIndexsBands(
                        mesh->m_dataPrivate->indexTriangleFan,
                        m_dataPrivate->indexTriangleFan,
                        indLastVertexs);

    prv_appendAllIndexsBands(
                        mesh->m_dataPrivate->indexTriangleStrip,
                        m_dataPrivate->indexTriangleStrip,
                        indLastVertexs);

    prv_concatenateIndexs(
                        mesh->m_dataPrivate->indexTriangles,
                        indLastVertexs,
                        m_dataPrivate->indexTriangles);
}

//-----------------------------------------------------------------------

static unsigned long prv_appendVertex(
                class CArray<SPrvVertex> *vertexs,
                double x, double y, double z,
                double Nx, double Ny, double Nz,
                double texx, double texy)
{
    unsigned long indPos;
    struct SPrvVertex *vertex;

    vertex = new SPrvVertex(x, y, z, Nx, Ny, Nz, texx, texy);

    indPos = vertexs->size();
    vertexs->add(vertex);

    return indPos;
}

//-----------------------------------------------------------------------

static void prv_appendTriangle(
                class CArrayULong *indexTriangles,
                class CArray<SPrvVertex> *vertexs,
                double x1, double y1, double z1,
                double Nx1, double Ny1, double Nz1,
                double texx1, double texy1,
                double x2, double y2, double z2,
                double Nx2, double Ny2, double Nz2,
                double texx2, double texy2,
                double x3, double y3, double z3,
                double Nx3, double Ny3, double Nz3,
                double texx3, double texy3)
{
    unsigned long indexVertex1, indexVertex2, indexVertex3;

    indexVertex1 = prv_appendVertex(vertexs, x1, y1, z1, Nx1, Ny1, Nz1, texx1, texy1);
    indexVertex2 = prv_appendVertex(vertexs, x2, y2, z2, Nx2, Ny2, Nz2, texx2, texy2);
    indexVertex3 = prv_appendVertex(vertexs, x3, y3, z3, Nx3, Ny3, Nz3, texx3, texy3);

    indexTriangles->add(indexVertex1);
    indexTriangles->add(indexVertex2);
    indexTriangles->add(indexVertex3);
}

//-----------------------------------------------------------------------

void CMesh::appendQuad(
                    double x1, double y1, double z1,
                    double Nx1, double Ny1, double Nz1,
                    double x2, double y2, double z2,
                    double Nx2, double Ny2, double Nz2,
                    double x3, double y3, double z3,
                    double Nx3, double Ny3, double Nz3,
                    double x4, double y4, double z4,
                    double Nx4, double Ny4, double Nz4)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_NO_TEXTURE);

    prv_appendTriangle(
                    m_dataPrivate->indexTriangles,
                    m_dataPrivate->vertexs,
                    x4, y4, z4, Nx4, Ny4, Nz4, 0., 0.,
                    x1, y1, z1, Nx1, Ny1, Nz1, 0., 0.,
                    x3, y3, z3, Nx3, Ny3, Nz3, 0., 0.);

    prv_appendTriangle(
                    m_dataPrivate->indexTriangles,
                    m_dataPrivate->vertexs,
                    x1, y1, z1, Nx1, Ny1, Nz1, 0., 0.,
                    x2, y2, z2, Nx2, Ny2, Nz2, 0., 0.,
                    x3, y3, z3, Nx3, Ny3, Nz3, 0., 0.);
}

//-----------------------------------------------------------------------

void CMesh::appendTriangle(
                    double x1, double y1, double z1,
                    double Nx1, double Ny1, double Nz1,
                    double x2, double y2, double z2,
                    double Nx2, double Ny2, double Nz2,
                    double x3, double y3, double z3,
                    double Nx3, double Ny3, double Nz3)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_NO_TEXTURE);

    prv_appendTriangle(
                    m_dataPrivate->indexTriangles,
                    m_dataPrivate->vertexs,
                    x1, y1, z1, Nx1, Ny1, Nz1, 0., 0.,
                    x2, y2, z2, Nx2, Ny2, Nz2, 0., 0.,
                    x3, y3, z3, Nx3, Ny3, Nz3, 0., 0.);
}

//-----------------------------------------------------------------------

void CMesh::appendTriangleWithTexture(
                    double x1, double y1, double z1,
                    double Nx1, double Ny1, double Nz1,
                    double texx1, double texy1,
                    double x2, double y2, double z2,
                    double Nx2, double Ny2, double Nz2,
                    double texx2, double texy2,
                    double x3, double y3, double z3,
                    double Nx3, double Ny3, double Nz3,
                    double texx3, double texy3)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_TEXTURE);

    prv_appendTriangle(
                m_dataPrivate->indexTriangles,
                m_dataPrivate->vertexs,
                x1, y1, z1, Nx1, Ny1, Nz1, texx1, texy1,
                x2, y2, z2, Nx2, Ny2, Nz2, texx2, texy2,
                x3, y3, z3, Nx3, Ny3, Nz3, texx3, texy3);
}

//-----------------------------------------------------------------------

void CMesh::appendTessetation(
                class CArrPoint3d **points,
                class CArrPoint3d **normals,
                class CArrayULong **indexsTriangles,
                class CArray<CArrayULong> **indexsTriangleStrip,
                class CArray<CArrayULong> **indexsTriangleFan)
{
    unsigned long indLastVertexs;

    prv_integrity(m_dataPrivate);
    assert_no_null(indexsTriangles);
    assert_no_null(indexsTriangleStrip);
    assert_no_null(indexsTriangleFan);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_NO_TEXTURE);

    indLastVertexs = m_dataPrivate->vertexs->size();
    prv_appendVertexs(points, normals, NULL, m_dataPrivate->vertexs);

    prv_appendAllIndexsBands(*indexsTriangleStrip, m_dataPrivate->indexTriangleStrip, indLastVertexs);
    prv_appendAllIndexsBands(*indexsTriangleFan, m_dataPrivate->indexTriangleFan, indLastVertexs);
    prv_concatenateIndexs(*indexsTriangles, indLastVertexs, m_dataPrivate->indexTriangles);

    delete *indexsTriangles;
    *indexsTriangles = NULL;

    delete *indexsTriangleStrip;
    *indexsTriangleStrip = NULL;

    delete *indexsTriangleFan;
    *indexsTriangleFan = NULL;
}

//-----------------------------------------------------------------------

void CMesh::appendTriangleFan(
                    class CArrPoint3d **points,
                    class CArrPoint3d **normals,
                    class CArrayULong **indexsTriangleFan)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_NO_TEXTURE);

    prv_appendVertexsAndIndexsBand(
                    points, normals, NULL, indexsTriangleFan,
                    m_dataPrivate->vertexs, m_dataPrivate->indexTriangleFan);
}

//-----------------------------------------------------------------------

void CMesh::appendTriangleFanWithTexture(
                class CArrPoint3d **points,
                class CArrPoint3d **normals,
                class CArrPoint2d **texture,
                class CArrayULong **indexsTriangleFan)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(texture);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_TEXTURE);

    prv_appendVertexsAndIndexsBand(
                points, normals, texture, indexsTriangleFan,
                m_dataPrivate->vertexs, m_dataPrivate->indexTriangleFan);
}

//-----------------------------------------------------------------------

void CMesh::appendTriangleStrip(
                class CArrPoint3d **points,
                class CArrPoint3d **normals,
                class CArrayULong **indexsTriangleStrip)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_NO_TEXTURE);

    prv_appendVertexsAndIndexsBand(
                points, normals, NULL, indexsTriangleStrip,
                m_dataPrivate->vertexs, m_dataPrivate->indexTriangleStrip);
}

//-----------------------------------------------------------------------

void CMesh::appendMultipleTriangleStripBands(
                class CArrPoint3d **points,
                class CArrPoint3d **normals,
                class CArray<CArrayULong> **indexsTriangleStrip)
{
    unsigned long indLastVertexs;

    prv_integrity(m_dataPrivate);
    assert_no_null(indexsTriangleStrip);
    assert_no_null(*indexsTriangleStrip);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_NO_TEXTURE);

    indLastVertexs = m_dataPrivate->vertexs->size();
    prv_appendVertexs(points, normals, NULL, m_dataPrivate->vertexs);

    prv_appendAllIndexsBands(*indexsTriangleStrip, m_dataPrivate->indexTriangleStrip, indLastVertexs);

    delete *indexsTriangleStrip;
    *indexsTriangleStrip = NULL;
}

//-----------------------------------------------------------------------

void CMesh::appendTriangleStripWithTexture(
                class CArrPoint3d **points,
                class CArrPoint3d **normals,
                class CArrPoint2d **texture,
                class CArrayULong **indexsTriangleStrip)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->typeTexture = prv_defineTexture(m_dataPrivate->typeTexture, PRV_TEXTURE);

    prv_appendVertexsAndIndexsBand(
                points, normals, texture, indexsTriangleStrip,
                m_dataPrivate->vertexs, m_dataPrivate->indexTriangleStrip);
}

//-----------------------------------------------------------------------

void CMesh::applyTransformation(const class CStackTransformation *transformation)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);
    assert_no_null(transformation);

    num = m_dataPrivate->vertexs->size();

    for (unsigned long i = 0; i < num; i++)
    {
        struct SPrvVertex *vertex;

        vertex = m_dataPrivate->vertexs->get(i);
        assert_no_null(vertex);

        transformation->transformationPointF3d(vertex->x, vertex->y, vertex->z, &vertex->x, &vertex->y, &vertex->z);
        transformation->transformationUnitVectorF(vertex->Nx, vertex->Ny, vertex->Nz, &vertex->Nx, &vertex->Ny, &vertex->Nz);
    }
}

//-----------------------------------------------------------------------

void CMesh::applyMatrix(class CMatrix4x4 *matrixTransformation)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);
    assert_no_null(matrixTransformation);

    num = m_dataPrivate->vertexs->size();

    for (unsigned long i = 0; i < num; i++)
    {
        struct SPrvVertex *vertex;

        vertex = m_dataPrivate->vertexs->get(i);
        assert_no_null(vertex);

        matrixTransformation->transformationPointF3d(vertex->x, vertex->y, vertex->z, &vertex->x, &vertex->y, &vertex->z);
        matrixTransformation->transformationUnitVectorF(vertex->Nx, vertex->Ny, vertex->Nz, &vertex->Nx, &vertex->Ny, &vertex->Nz);
    }
}

//-----------------------------------------------------------------------

static void prv_minmaxTextures(
                            const class CArray<SPrvVertex> *vertexs,
                            float *xmin, float *ymin, float *xmax, float *ymax)
{
    unsigned long num;
    float xmin_loc, ymin_loc, xmax_loc, ymax_loc;

    assert_no_null(xmin);
    assert_no_null(ymin);
    assert_no_null(xmax);
    assert_no_null(ymax);

    num = vertexs->size();
    assert(num > 0);

    xmin_loc = CMath::maxFloat();
    ymin_loc = CMath::maxFloat();
    xmax_loc = -CMath::maxFloat();
    ymax_loc = -CMath::maxFloat();

    for (unsigned long i = 0; i < num; i++)
    {
        const class SPrvVertex *vertex;

        vertex = vertexs->get(i);
        assert_no_null(vertex);

        xmin_loc = MIN(vertex->texx, xmin_loc);
        ymin_loc = MIN(vertex->texy, ymin_loc);
        xmax_loc = MAX(vertex->texx, xmax_loc);
        ymax_loc = MAX(vertex->texy, ymax_loc);
    }

    *xmin = xmin_loc;
    *ymin = ymin_loc;
    *xmax = xmax_loc;
    *ymax = ymax_loc;
}

//-----------------------------------------------------------------------

static void prv_normalizeCoordTexture(class CArray<SPrvVertex> *vertexs)
{
    unsigned long num;
    float xmin, ymin, xmax, ymax, sizeX, sizeY;

    prv_minmaxTextures(vertexs, &xmin, &ymin, &xmax, &ymax);

    sizeX = xmax - xmin;
    sizeY = ymax - ymin;

    assert(sizeX > 0.);
    assert(sizeY > 0.);

    num = vertexs->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class SPrvVertex *vertex;

        vertex = vertexs->get(i);
        assert_no_null(vertex);

        vertex->texx = (vertex->texx - xmin) / sizeX;
        vertex->texy = (vertex->texy - ymin) / sizeY;
    }
}

// ---------------------------------------------------------------------------

void CMesh::calculateCoordsTexturesInPlane(enum CMesh::ETypeTexture typeTexture)
{
    unsigned long i, numPoints;

    prv_integrity(m_dataPrivate);

    numPoints = m_dataPrivate->vertexs->size();

    for (i = 0; i < numPoints; i++)
    {
        struct SPrvVertex *vertex;
        double xTexture, yTexture;
        double Ux, Uy, Uz, Vx, Vy, Vz;

        vertex = m_dataPrivate->vertexs->get(i);
        assert_no_null(vertex);

        CGeometry::horizontalAxisFromThePlaneNormal(vertex->Nx, vertex->Ny, vertex->Nz, &Ux, &Uy, &Uz, &Vx, &Vy, &Vz);
        CGeometry::point3dProjectedTo2D(
                                0., 0., 0.,
                                Ux, Uy, Uz,
                                Vx, Vy, Vz,
                                vertex->x, vertex->y, vertex->z,
                                &xTexture, &yTexture);

        vertex->texx = (float)xTexture;
        vertex->texy = (float)yTexture;
    }

    switch(typeTexture)
    {
        case TEXTURE_REPEAT:

            break;

        case TEXTURE_DECAL:

            prv_normalizeCoordTexture(m_dataPrivate->vertexs);
            break;

        default_error();
    }

    m_dataPrivate->typeTexture = PRV_TEXTURE;
}

// ---------------------------------------------------------------------------

void CMesh::calculateCoordsTexturesXY(enum CMesh::ETypeTexture typeTexture)
{
    unsigned long i, numPoints;

    prv_integrity(m_dataPrivate);

    numPoints = m_dataPrivate->vertexs->size();

    for (i = 0; i < numPoints; i++)
    {
        struct SPrvVertex *vertex;

        vertex = m_dataPrivate->vertexs->get(i);
        assert_no_null(vertex);

        vertex->texx = (float)vertex->x;
        vertex->texy = (float)vertex->y;
    }

    switch(typeTexture)
    {
        case TEXTURE_REPEAT:

            break;

        case TEXTURE_DECAL:

            prv_normalizeCoordTexture(m_dataPrivate->vertexs);
            break;

        default_error();
    }

    m_dataPrivate->typeTexture = PRV_TEXTURE;
}


//-----------------------------------------------------------------------

static class CArrPoint2d *prv_createTexture(enum EPrvTypeTexture typeTexture, unsigned long numVertexs)
{
    class CArrPoint2d *textCoord;

    switch(typeTexture)
    {
        case PRV_NO_TEXTURE:

            textCoord = NULL;
            break;

        case PRV_TEXTURE:

            textCoord = new CArrPoint2d(numVertexs);
            break;

        case PRV_UNDEFINED_TEXTURE:
        default_error();
    }

    return textCoord;
}

//-----------------------------------------------------------------------

static void prv_dataGraphics(
                const class CArray<SPrvVertex> *dataVertexs,
                enum EPrvTypeTexture typeTexture,
                class CArrPoint3d **vertexs, class CArrPoint3d **normals, class CArrPoint2d **textCoord)
{
    unsigned long numVertexs;
    class CArrPoint3d *vertexs_loc, *normals_loc;
    class CArrPoint2d *textCoord_loc;

    assert_no_null(vertexs);
    assert_no_null(normals);
    assert_no_null(textCoord);

    numVertexs = dataVertexs->size();

    vertexs_loc = new CArrPoint3d(numVertexs);
    normals_loc = new CArrPoint3d(numVertexs);
    textCoord_loc = prv_createTexture(typeTexture, numVertexs);

    for (unsigned long i = 0; i < numVertexs; i++)
    {
        const struct SPrvVertex *dataVertex;

        dataVertex = dataVertexs->get(i);

        vertexs_loc->set(i, dataVertex->x, dataVertex->y, dataVertex->z);
        normals_loc->set(i, dataVertex->Nx, dataVertex->Ny, dataVertex->Nz);

        if (textCoord_loc != NULL)
            textCoord_loc->set(i, dataVertex->texx, dataVertex->texy);
    }

    *vertexs = vertexs_loc;
    *normals = normals_loc;
    *textCoord = textCoord_loc;
}

//-----------------------------------------------------------------------

void CMesh::draw(class IGraphics *graphics) const
{
    class CArrPoint3d *points, *normals;
    class CArrPoint2d *textCoord;

    prv_integrity(m_dataPrivate);

    prv_dataGraphics(
                m_dataPrivate->vertexs,
                m_dataPrivate->typeTexture,
                &points, &normals, &textCoord);

    graphics->drawMesh(
                points, normals, textCoord,
                m_dataPrivate->indexTriangles,
                m_dataPrivate->indexTriangleFan,
                m_dataPrivate->indexTriangleStrip);

    delete points;
    delete normals;

    if (textCoord != NULL)
        delete textCoord;
}

