/* -----------------------------------------------------------------------------
 *  CReaderModel3d.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CReaderModel3d.hpp"

#include "libg3d.inl"

#include "asrtbas.h"
#include "CModel3d.hpp"
#include "CMaterial.hpp"
#include "CVector.hpp"
#include "CGeometry.hpp"
#include "CMatrix4x4.hpp"
#include "CStackTransformation.hpp"
#include "CImg.hpp"

static const double PRV_PRECSION = 1e-05;

//-----------------------------------------------------------------------

static void prv_appendMaterials(const G3DModel *model, class CModel3d *model3d)
{
    GSList *nodeMaterial;

    assert_no_null(model);
    assert_no_null(model3d);

    nodeMaterial = model->materials;

    while (nodeMaterial != NULL)
    {
        const G3DMaterial *materialData;
        class CMaterial *material;

        materialData = (G3DMaterial *)nodeMaterial->data;

        if (materialData->tex_image != NULL)
        {
            class CImg *image;

            image = new CImg(
                        materialData->tex_image->width,
                        materialData->tex_image->height,
                        materialData->tex_image->width,
                        materialData->tex_image->depth,
                        materialData->tex_image->pixeldata);

            material = new CMaterial(
                        materialData->name,
                        materialData->r, materialData->g, materialData->b, materialData->a,
                        &image);
        }
        else
            material = new CMaterial(
                        materialData->name,
                        materialData->r, materialData->g, materialData->b, materialData->a);

        model3d->appendMaterial(&material);

        nodeMaterial = nodeMaterial->next;
    }
}

//-----------------------------------------------------------------------

static void prv_getPoint3d(
                    const G3DVector *vertex,
                    unsigned long count, unsigned long ind, double *x, double *y, double *z)
{
    unsigned long offset;

    assert(ind < count);

    offset = 3 * ind;

    *x = vertex[offset];
    *y = vertex[offset + 1];
    *z = vertex[offset + 2];
}

//-----------------------------------------------------------------------

static void prv_getPoint3dTransformed(
                    const G3DVector *vertex,
                    const class CStackTransformation *transformation,
                    unsigned long count, unsigned long ind, double *x, double *y, double *z)
{
    assert_no_null(transformation);
    assert_no_null(x);
    assert_no_null(y);
    assert_no_null(z);

    prv_getPoint3d(vertex, count, ind, x, y, z);
    transformation->transformationPoint3d(*x, *y, *z, x, y, z);
}

//-----------------------------------------------------------------------

static void prv_getUnitVector3dTransformed(
                    const G3DVector *vertex,
                    const class CStackTransformation *transformation,
                    unsigned long count, unsigned long ind,
                    double *Nx, double *Ny, double *Nz)
{
    assert_no_null(transformation);
    assert_no_null(Nx);
    assert_no_null(Ny);
    assert_no_null(Nz);

    prv_getPoint3d(vertex, count, ind, Nx, Ny, Nz);
    transformation->transformationUnitVector(*Nx, *Ny, *Nz, Nx, Ny, Nz);
}

//-----------------------------------------------------------------------

static void prv_getDataVertex(
                        const G3DFace *face,
                        const class CStackTransformation *transformation,
                        guint32 vertex_count,
                        const G3DVector *vertex_data,
                        unsigned long ind,
                        bool withNormals, bool withTexture,
                        double *x, double *y, double *z,
                        double *Nx, double *Ny, double *Nz,
                        double *texx, double *texy)
{
    unsigned long indVertex;

    assert_no_null(face);
    assert(ind < face->vertex_count);

    indVertex = face->vertex_indices[ind];

    prv_getPoint3dTransformed(vertex_data, transformation, vertex_count, indVertex, x, y, z);

    if (withNormals == true)
        prv_getUnitVector3dTransformed(
                        face->normals,
                        transformation,
                        face->vertex_count, ind, Nx, Ny, Nz);

    if (withTexture == true)
    {
        double texzNotUsed;
        prv_getPoint3d(face->tex_vertex_data, face->tex_vertex_count, ind, texx, texy, &texzNotUsed);
    }
}

//-----------------------------------------------------------------------

static void prv_normalTriangle(
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double x3, double y3, double z3,
                        double *Nx, double *Ny, double *Nz)
{
    assert_no_null(Nx);
    assert_no_null(Ny);
    assert_no_null(Nz);

    if (CGeometry::areCollinearPoints(x1, y1, z1, x2, y2, z2, x3, y3, z3, PRV_PRECSION) == false)
    {
        CVector::crossProduct(x2 - x1, y2 - y1, z2 - z1, x3 - x1, y3 - y1, z3 - z1, Nx, Ny, Nz);
        CVector::toUnitVector(Nx, Ny, Nz);
    }
    else
    {
        *Nx = 0.;
        *Ny = 0.;
        *Nz = 1.;
    }
}

//-----------------------------------------------------------------------

static void prv_appendFaceQuads(
                        const class CStackTransformation *transformation,
                        const G3DFace *face,
                        guint32 vertex_count,
                        const G3DVector *vertex_data,
                        class CModel3d *model)
{
    bool withTexture, withNormals;
    unsigned long numQuads;

    assert_no_null(model);
    assert_no_null(face);
    assert_no_null(face->vertex_indices);
    assert_no_null(face->material);

    if (face->tex_vertex_count == 0)
        withTexture = false;
    else
        assert(face->tex_vertex_count == face->vertex_count);

    if (face->normals != NULL)
        withNormals = true;
    else
        withNormals = false;

    assert(face->vertex_count % 4 == 0);
    numQuads = face->vertex_count / 4;

    for (unsigned long i = 0; i < numQuads; i++)
    {
        double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
        double Nx1, Ny1, Nz1, Nx2, Ny2, Nz2, Nx3, Ny3, Nz3, Nx4, Ny4, Nz4;
        double texx1, texy1, texx2, texy2, texx3, texy3, texx4, texy4;
        unsigned long ind1, ind2, ind3, ind4;

        ind1 = 4 * i;
        ind2 = ind1 + 1;
        ind3 = ind1 + 2;
        ind4 = ind1 + 3;

        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind1, withNormals, withTexture, &x1, &y1, &z1, &Nx1, &Ny1, &Nz1, &texx1, &texy1);
        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind2, withNormals, withTexture, &x2, &y2, &z2, &Nx2, &Ny2, &Nz2, &texx2, &texy2);
        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind3, withNormals, withTexture, &x3, &y3, &z3, &Nx3, &Ny3, &Nz3, &texx3, &texy3);
        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind4, withNormals, withTexture, &x4, &y4, &z4, &Nx4, &Ny4, &Nz4, &texx4, &texy4);

        if (withNormals == false)
        {
            prv_normalTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, &Nx1, &Ny1, &Nz1);

            Nx2 = Nx1; Ny2 = Ny1; Nz2 = Nz1;
            Nx3 = Nx1; Ny3 = Ny1; Nz3 = Nz1;
            Nx4 = Nx1; Ny4 = Ny1; Nz4 = Nz1;
        }

        if (withTexture == true)
        {
            model->appendTriangleWithTexture(
                        face->material->name,
                        x1, y1, z1, Nx1, Ny1, Nz1, texx1, texy1,
                        x2, y2, z2, Nx2, Ny2, Nz2, texx2, texy2,
                        x3, y3, z3, Nx3, Ny3, Nz3, texx3, texy3);

            model->appendTriangleWithTexture(
                        face->material->name,
                        x1, y1, z1, Nx1, Ny1, Nz1, texx1, texy1,
                        x3, y3, z3, Nx3, Ny3, Nz3, texx3, texy3,
                        x4, y4, z4, Nx4, Ny4, Nz4, texx4, texy4);
        }
        else
        {
            model->appendTriangle(
                        face->material->name,
                        x1, y1, z1, Nx1, Ny1, Nz1,
                        x2, y2, z2, Nx2, Ny2, Nz2,
                        x3, y3, z3, Nx3, Ny3, Nz3);

            model->appendTriangle(
                        face->material->name,
                        x1, y1, z1, Nx1, Ny1, Nz1,
                        x3, y3, z3, Nx3, Ny3, Nz3,
                        x4, y4, z4, Nx4, Ny4, Nz4);
        }
    }
}

//-----------------------------------------------------------------------

static void prv_appendFaceTriangles(
                    const class CStackTransformation *transformation,
                    const G3DFace *face,
                    guint32 vertex_count,
                    const G3DVector *vertex_data,
                    class CModel3d *model)
{
    bool withTexture, withNormals;
    unsigned long numTriangles;

    assert_no_null(model);
    assert_no_null(face);
    assert_no_null(face->vertex_indices);
    assert_no_null(face->material);

    if (face->tex_vertex_count == 0)
        withTexture = false;
    else
        assert(face->tex_vertex_count == face->vertex_count);

    if (face->normals != NULL)
        withNormals = true;
    else
        withNormals = false;

    assert(face->vertex_count % 3 == 0);
    numTriangles = face->vertex_count / 3;

    for (unsigned long i = 0; i < numTriangles; i++)
    {
        double x1, y1, z1, x2, y2, z2, x3, y3, z3;
        double Nx1, Ny1, Nz1, Nx2, Ny2, Nz2, Nx3, Ny3, Nz3;
        double texx1, texy1, texx2, texy2, texx3, texy3;
        unsigned long ind1, ind2, ind3;

        ind1 = 3 * i;
        ind2 = ind1 + 1;
        ind3 = ind1 + 2;

        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind1, withNormals, withTexture, &x1, &y1, &z1, &Nx1, &Ny1, &Nz1, &texx1, &texy1);
        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind2, withNormals, withTexture, &x2, &y2, &z2, &Nx2, &Ny2, &Nz2, &texx2, &texy2);
        prv_getDataVertex(face, transformation, vertex_count, vertex_data, ind3, withNormals, withTexture, &x3, &y3, &z3, &Nx3, &Ny3, &Nz3, &texx3, &texy3);

        if (withNormals == false)
        {
            prv_normalTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, &Nx1, &Ny1, &Nz1);

            Nx2 = Nx1; Ny2 = Ny1; Nz2 = Nz1;
            Nx3 = Nx1; Ny3 = Ny1; Nz3 = Nz1;
        }

        if (withTexture == true)
        {
            model->appendTriangleWithTexture(
                        face->material->name,
                        x1, y1, z1, Nx1, Ny1, Nz1, texx1, texy1,
                        x2, y2, z2, Nx2, Ny2, Nz2, texx2, texy2,
                        x3, y3, z3, Nx3, Ny3, Nz3, texx3, texy3);
        }
        else
        {
            model->appendTriangle(
                        face->material->name,
                        x1, y1, z1, Nx1, Ny1, Nz1,
                        x2, y2, z2, Nx2, Ny2, Nz2,
                        x3, y3, z3, Nx3, Ny3, Nz3);
        }
    }
}

//-----------------------------------------------------------------------

static void prv_appendFaces(
                        const GSList *faces,
                        guint32 vertex_count,
                        const G3DVector *vertex_data,
                        const class CStackTransformation *transformation,
                        class CModel3d *model)
{
    const GSList *nodeFace;

    assert_no_null(model);
    assert_no_null(faces);

    nodeFace = faces;

    while (nodeFace != NULL)
    {
        const G3DFace *face;

        face = (const G3DFace *)nodeFace->data;

        //TODO:Optimizar.
        if (face->vertex_count % 4 == 0)
            prv_appendFaceQuads(transformation, face, vertex_count, vertex_data, model);
        else
            prv_appendFaceTriangles(transformation, face, vertex_count, vertex_data, model);

        nodeFace = nodeFace->next;
    }
}

//-----------------------------------------------------------------------

static void prv_appendFacesAllModel(
                    unsigned long numRecursivity,
                    const GSList *objects,
                    class CStackTransformation *transformation,
                    class CModel3d *model3d)
{
    const GSList *nodeObject;

    assert(numRecursivity < 100);
    numRecursivity++;

    assert_no_null(transformation);
    assert_no_null(model3d);
    assert_no_null(objects);

    nodeObject = objects;

    while (nodeObject != NULL)
    {
        const G3DObject *object3d;

        object3d = (const G3DObject *)nodeObject->data;

        if (object3d->hide == false)
        {
            if (object3d->transformation != NULL)
            {
                class CMatrix4x4 *matrix;

                transformation->pushTransformation();
                matrix = new CMatrix4x4(object3d->transformation->matrix);
                transformation->appendMatrix(matrix);
                delete matrix;
            }

            if (object3d->faces != NULL)
                prv_appendFaces(object3d->faces, object3d->vertex_count, object3d->vertex_data, transformation, model3d);

            if (object3d->transformation != NULL)
                transformation->popTransformation();
        }

        if (object3d->objects != NULL)
            prv_appendFacesAllModel(numRecursivity, object3d->objects, transformation, model3d);

        nodeObject = nodeObject->next;
    }
}

//-----------------------------------------------------------------------

class CModel3d *CReaderModel3d::readModel3d(const char *filename)
{
    G3DContext *context;
    G3DModel *model;
    class CModel3d *model3d;
    class CStackTransformation *transformation;

    context = g3d_context_new();
    model = g3d_model_load_full(context, filename, 0x0000);
    assert_no_null(model);

    model3d = new CModel3d;
    transformation = new CStackTransformation;

    prv_appendMaterials(model, model3d);
    prv_appendFacesAllModel(0, model->objects, transformation, model3d);

    delete transformation;

    g3d_model_free(model);
    g3d_context_free(context);

    return model3d;
}

