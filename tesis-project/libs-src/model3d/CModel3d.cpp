/* -----------------------------------------------------------------------------
 *  CModel3d.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CModel3d.hpp"
#include "CMaterial.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "typebas.h"

#include "CMath.hpp"
#include "CString.hpp"
#include "CArray.hpp"
#include "CMesh.hpp"
#include "CStackTransformation.hpp"
#include "CGeometry.hpp"
#include "CGraphicsRect.hpp"
#include "IGraphics.hpp"

struct SPrvBody
{
    SPrvBody(const char *nameMaterial, unsigned long idBlockGraphics_param, class CMesh **meshs);
    ~SPrvBody();

    char *nameMaterial;
    unsigned long idBlockGraphics;
    class CMesh *mesh;
};

struct SDataPrivateModel3d
{
    bool isInLimits;

    class CArray<CMaterial> *materials;
    class CArray<SPrvBody> *bodies;
};

static const unsigned long PRV_UNDEFINED_BLOCKGRAPHICS = ULONG_MAX;
static const double PRV_PRECISION = 1e-5;
static const double PRV_INFINITE = 1e100;

//-----------------------------------------------------------------------

SPrvBody::SPrvBody(const char *nameMaterial_param, unsigned long idBlockGraphics_param, class CMesh **mesh_param)
{
    nameMaterial = CString::copy(nameMaterial_param);
    idBlockGraphics = idBlockGraphics_param;
    mesh = ASSIGN_PP_NO_NULL(mesh_param, class CMesh);
}

//-----------------------------------------------------------------------

SPrvBody::~SPrvBody()
{
    CString::free(&nameMaterial);
    delete mesh;
}

//-----------------------------------------------------------------------

static bool prv_isMaterialBody(const struct SPrvBody *body, const char *nameMaterial)
{
    assert_no_null(body);
    return CString::equal(body->nameMaterial, nameMaterial);
}

//-----------------------------------------------------------------------

static void prv_defineBlock(struct SPrvBody *body, class IGraphics *graphics)
{
    assert_no_null(body);
    assert_no_null(graphics);

    assert(body->idBlockGraphics == PRV_UNDEFINED_BLOCKGRAPHICS);

    body->idBlockGraphics = graphics->beginBlock();
    body->mesh->draw(graphics);
    graphics->endBlock(body->idBlockGraphics);
}

//-----------------------------------------------------------------------

static void prv_drawBody(const struct SPrvBody *body, class IGraphics *graphics)
{
    assert_no_null(body);
    assert_no_null(graphics);

    graphics->setMaterial(body->nameMaterial);
    assert(body->idBlockGraphics != PRV_UNDEFINED_BLOCKGRAPHICS);
    graphics->drawBlock(body->idBlockGraphics);
}

//-----------------------------------------------------------------------

static void prv_integrity(const struct SDataPrivateModel3d *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->materials);
    assert_no_null(dataPrivate->bodies);
}

//-----------------------------------------------------------------------

static struct SDataPrivateModel3d *prv_createModel3d(
                        bool isInLimits,
                        class CArray<CMaterial> **materials,
                        class CArray<SPrvBody> **bodies)
{
    struct SDataPrivateModel3d *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateModel3d);

    dataPrivate->isInLimits = isInLimits;

    dataPrivate->materials = ASSIGN_PP_NO_NULL(materials, class CArray<CMaterial>);
    dataPrivate->bodies = ASSIGN_PP_NO_NULL(bodies, class CArray<SPrvBody>);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyModel3d(struct SDataPrivateModel3d **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->materials;
    delete (*dataPrivate)->bodies;

    FREE_T(dataPrivate, struct SDataPrivateModel3d);
}

//-----------------------------------------------------------------------

CModel3d::CModel3d(void)
{
    class CArray<CMaterial> *materials;
    class CArray<SPrvBody> *bodies;
    bool isInLimits;

    isInLimits = true;
    materials = new CArray<CMaterial>;
    bodies = new CArray<SPrvBody>;
    m_dataPrivate = prv_createModel3d(isInLimits, &materials, &bodies);
}

//-----------------------------------------------------------------------

CModel3d::~CModel3d()
{
    prv_destroyModel3d(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static bool prv_isMaterial(const class CMaterial *material1, const class CMaterial *material2)
{
    assert_no_null(material1);
    return material1->isSameMaterial(material2);
}

//-----------------------------------------------------------------------

static void prv_appendMaterialIfNotExist(class CArray<CMaterial> *materials, const class CMaterial *material)
{
    assert_no_null(materials);

    if (materials->existOnlyOneElement(material, prv_isMaterial, NULL) == false)
    {
        class CMaterial *materialCopy;

        materialCopy = new CMaterial(material);
        materials->add(materialCopy);
    }
}

//-----------------------------------------------------------------------

static struct SPrvBody *prv_appendBodyWithMaterialIfNotExist(class CArray<SPrvBody> *bodies, const char *nameMaterial)
{
    struct SPrvBody *body;
    bool exist;
    unsigned long indPos;

    assert_no_null(bodies);

    exist = bodies->existOnlyOneElement(nameMaterial, prv_isMaterialBody, &indPos);

    if (exist == true)
        body = bodies->get(indPos);
    else
    {
        class CMesh *mesh;

        mesh = new CMesh;
        body = new SPrvBody(nameMaterial, PRV_UNDEFINED_BLOCKGRAPHICS, &mesh);
        bodies->add(body);
    }

    return body;
}

//-----------------------------------------------------------------------

void CModel3d::setIsInLimits(bool inLimits)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->isInLimits = inLimits;
}

//-----------------------------------------------------------------------

void CModel3d::appendMaterial(class CMaterial **material)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(material);

    prv_appendMaterialIfNotExist(m_dataPrivate->materials, *material);

    delete *material;
    *material = NULL;
}

//-----------------------------------------------------------------------

void CModel3d::appendMesh(const char *nameMaterial, const class CMesh *mesh)
{
    struct SPrvBody *body;

    prv_integrity(m_dataPrivate);

    body = prv_appendBodyWithMaterialIfNotExist(m_dataPrivate->bodies, nameMaterial);

    assert_no_null(body);
    assert_no_null(body->mesh);

    body->mesh->concatenate(mesh);
}

//-----------------------------------------------------------------------

void CModel3d::appendModel(const class CModel3d *model)
{
    assert_no_null(model);
    prv_integrity(m_dataPrivate);
    prv_integrity(model->m_dataPrivate);

    unsigned long num;

    num = model->m_dataPrivate->materials->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class CMaterial *material;

        material = model->m_dataPrivate->materials->get(i);
        assert_no_null(material);

        prv_appendMaterialIfNotExist(m_dataPrivate->materials, material);
    }

    num = model->m_dataPrivate->bodies->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const struct SPrvBody *bodyBegin;
        struct SPrvBody *bodyEnd;

        bodyBegin = model->m_dataPrivate->bodies->get(i);
        assert_no_null(bodyBegin);

        bodyEnd = prv_appendBodyWithMaterialIfNotExist(m_dataPrivate->bodies, bodyBegin->nameMaterial);
        assert_no_null(bodyEnd);

        bodyEnd->mesh->concatenate(bodyBegin->mesh);
    }
}


//-----------------------------------------------------------------------

void CModel3d::appendTriangle(
                            const char *nameMaterial,
                            double x1, double y1, double z1,
                            double Nx1, double Ny1, double Nz1,
                            double x2, double y2, double z2,
                            double Nx2, double Ny2, double Nz2,
                            double x3, double y3, double z3,
                            double Nx3, double Ny3, double Nz3)
{
    struct SPrvBody *body;

    prv_integrity(m_dataPrivate);

    body = prv_appendBodyWithMaterialIfNotExist(m_dataPrivate->bodies, nameMaterial);

    assert_no_null(body);
    assert_no_null(body->mesh);

    body->mesh->appendTriangle(
                            x1, y1, z1, Nx1, Ny1, Nz1,
                            x2, y2, z2, Nx2, Ny2, Nz2,
                            x3, y3, z3, Nx3, Ny3, Nz3);
}

//-----------------------------------------------------------------------

void CModel3d::appendTriangleWithTexture(
                            const char *nameMaterial,
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
    struct SPrvBody *body;

    prv_integrity(m_dataPrivate);

    body = prv_appendBodyWithMaterialIfNotExist(m_dataPrivate->bodies, nameMaterial);

    assert_no_null(body);
    assert_no_null(body->mesh);

    body->mesh->appendTriangleWithTexture(
            x1, y1, z1, Nx1, Ny1, Nz1, texx1, texy1,
            x2, y2, z2, Nx2, Ny2, Nz2, texx2, texy2,
            x3, y3, z3, Nx3, Ny3, Nz3, texx3, texy3);
}

//-----------------------------------------------------------------------

static void prv_drawBodies(const class CArray<SPrvBody> *bodies, class IGraphics *graphics)
{
    unsigned long num;

    num = bodies->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const struct SPrvBody *body;

        body = bodies->get(i);
        assert_no_null(body);
        assert_no_null(body->mesh);

        graphics->setMaterial(body->nameMaterial);
        body->mesh->draw(graphics);
    }
}

//-----------------------------------------------------------------------

static void prv_limitsModel(
                        const class CArray<SPrvBody> *bodies,
                        double *xmin, double *ymin, double *zmin,
                        double *xmax, double *ymax, double *zmax)
{
    class CGraphicsRect *graphicsRect;
    bool hasLimits;

    graphicsRect = new CGraphicsRect;

    prv_drawBodies(bodies, graphicsRect);

    hasLimits = graphicsRect->getLimits(xmin, ymin, zmin, xmax, ymax, zmax);
    assert(hasLimits == true);

    delete graphicsRect;
}

//---------------------------------------------------------------

static double prv_factor(double dimension, double minimum, double maximum)
{
    double factor;
    double size;

    size = maximum - minimum;

    if (CMath::isZero(size, PRV_PRECISION) == true)
        factor = PRV_INFINITE;
    else
    {
        assert(size > 0.);
        factor = dimension / size;
    }

    return factor;
}

//---------------------------------------------------------------

static double prv_calculateFactor(
                        double dimensionX, double dimensionY, double dimensionZ,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    double factorMin, factor;

    factorMin = PRV_INFINITE;

    factor = prv_factor(dimensionX, xmin, xmax);
    factorMin = MIN(factor, factorMin);

    factor = prv_factor(dimensionY, ymin, ymax);
    factorMin = MIN(factor, factorMin);

    factor = prv_factor(dimensionZ, zmin, zmax);
    factorMin = MIN(factor, factorMin);

    return factorMin;
}

//-----------------------------------------------------------------------

static void prv_applyTransformationToBodies(
                            const class CArray<SPrvBody> *bodies,
                            const class CStackTransformation *stackTransformation)
{
    unsigned long num;

    num = bodies->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const struct SPrvBody *body;

        body = bodies->get(i);
        assert_no_null(body);
        assert_no_null(body->mesh);

        body->mesh->applyTransformation(stackTransformation);
    }
}

//-----------------------------------------------------------------------

void CModel3d::insideCube(double dimensionX, double dimensionY, double dimensionZ)
{
    class CStackTransformation *stackTransformation;
    double factor;
    double xmin, ymin, zmin, xmax, ymax, zmax;
    double xcenter, ycenter, zcenter;

    assert_no_null(m_dataPrivate);

    prv_limitsModel(m_dataPrivate->bodies, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax);

    CGeometry::calculateCenter(xmin, ymin, zmin, xmax, ymax, zmax, &xcenter, &ycenter, &zcenter);

    factor = prv_calculateFactor(dimensionX, dimensionY, dimensionZ, xmin, ymin, zmin, xmax, ymax, zmax);

    stackTransformation = new CStackTransformation;

    stackTransformation->appendScale(factor, factor, factor);
    stackTransformation->appendTranslation(-xcenter, -ycenter, -zmin);

    prv_applyTransformationToBodies(m_dataPrivate->bodies, stackTransformation);

    delete stackTransformation;
}

//-----------------------------------------------------------------------

void CModel3d::applyTransformation(const class CStackTransformation *transformation)
{
    prv_integrity(m_dataPrivate);
    prv_applyTransformationToBodies(m_dataPrivate->bodies, transformation);
}

//-----------------------------------------------------------------------

void CModel3d::defineMaterialsInGraphics(class IGraphics *graphics)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->materials->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class CMaterial *material;

        material = m_dataPrivate->materials->get(i);
        assert_no_null(material);
        graphics->defineMaterial(material);
    }
}

//-----------------------------------------------------------------------

static void prv_defineIdBlocksBodies(const class CArray<SPrvBody> *bodies, class IGraphics *graphics)
{
    unsigned long num;

    assert_no_null(bodies);

    num = bodies->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class SPrvBody *body;

        body = bodies->get(i);
        prv_defineBlock(body, graphics);
    }
}

//-----------------------------------------------------------------------

void CModel3d::defineBlock(class IGraphics *graphics)
{
    prv_integrity(m_dataPrivate);

    prv_defineIdBlocksBodies(m_dataPrivate->bodies, graphics);
}

//-----------------------------------------------------------------------

void CModel3d::draw(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    prv_drawBodies(m_dataPrivate->bodies, graphics);
}

//-----------------------------------------------------------------------

static void prv_drawIdBlocksBodies(const class CArray<SPrvBody> *bodies, class IGraphics *graphics)
{
    unsigned long num;

    num = bodies->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class SPrvBody *body;

        body = bodies->get(i);
        prv_drawBody(body, graphics);
    }
}

//-----------------------------------------------------------------------

void CModel3d::drawIdBlock(class IGraphics *graphics) const
{
    prv_integrity(m_dataPrivate);
    assert_no_null(graphics);

    if (graphics->hasManagerBlocks() == true)
        prv_drawIdBlocksBodies(m_dataPrivate->bodies, graphics);
    else
    {
        bool isDrawable;

        if (m_dataPrivate->isInLimits == true)
            isDrawable = true;
        else
        {
            if (dynamic_cast<class CGraphicsRect *>(graphics) != NULL)
                isDrawable = false;
            else
                isDrawable = true;
        }

        if (isDrawable == true)
            prv_drawBodies(m_dataPrivate->bodies, graphics);
    }
}
